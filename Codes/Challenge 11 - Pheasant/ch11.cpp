#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <queue>
#include <sstream>
#include <openssl\aes.h>
#include <openssl\rand.h>
using namespace std;

int keylength = 256;

size_t inputslength, encslength;

//Decrypts a block encoded in aes-256-ecb (encrypted) with 'aes_key' and stores text in 'decrypted'
void myDecrypt(unsigned char * encrypted, unsigned char * decrypted, unsigned char * aes_key) {
  memset(decrypted, 0, sizeof(decrypted));
  for (int i = 0; i < inputslength; i += AES_BLOCK_SIZE) {
    AES_KEY dec_key;
    unsigned char dec_out[AES_BLOCK_SIZE];
    memset(dec_out, 0, sizeof(dec_out));
    AES_set_decrypt_key(aes_key, keylength, &dec_key);
    AES_ecb_encrypt(encrypted + i, dec_out, &dec_key, AES_DECRYPT);
    if (i == 0) {
      if (not isdigit(dec_out[0]) or not isdigit(dec_out[1]) or not isdigit(dec_out[2]) or not isdigit(dec_out[3])) {
        decrypted[0] = '\0';
        //If it is not a digit, the password is wrong
        return;
      }
    }
    for (int j = 0; j < AES_BLOCK_SIZE and i + j < inputslength; ++j) decrypted[i + j] = dec_out[j];
  }
  
  //Sets the end of string mark
  for (int i = 0; i < inputslength; ++i) {
    if (decrypted[i] != '\n' and decrypted[i] != ' ' and not isdigit(decrypted[i])) decrypted[i] = '\0';
  }
  decrypted[inputslength] = '\0';
}

//Encrypts a block in aes-256-ecb (input) with 'aes_key' and stores text in 'encrypted'
void myEncrypt(unsigned char * input, unsigned char * encrypted, unsigned char * aes_key) {
  memset(encrypted, 0, sizeof(encrypted));
  for (int i = 0; i < inputslength; i += AES_BLOCK_SIZE) {
    AES_KEY enc_key;
    unsigned char enc_out[AES_BLOCK_SIZE];
    memset(enc_out, 0, sizeof(enc_out));
    AES_set_encrypt_key(aes_key, keylength, &enc_key);
    AES_ecb_encrypt(input + i, enc_out, &enc_key, AES_ENCRYPT);
    for (int j = 0; j < AES_BLOCK_SIZE; ++j) encrypted[i + j] = enc_out[j];
  }
  encrypted[encslength] = '\0';
}

//Converts a number n into a string
string str(int n) {
  string res = "";
  if (n < 10) res += char('0' + n);
  else {
    res = str(n / 10);
    res += char('0' + n % 10);
  }
  return res;
}

priority_queue<pair<int, int> > pq;

//Returns all the contents of the file 'filename'
string get_file_contents(string filename) {
  ifstream in(filename.c_str(), ios::in | ios::binary);
  if (in) {
    string contents;
    in.seekg(0, ios::end);
    contents.resize(in.tellg());
    in.seekg(0, ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
}

//Given an id and part of the password, decrypts the feed
//corresponding to that id and inserts the events found into 'pq'
void crack_id(int id, string fragment_key) {
  //Copies the key
  unsigned char key[32];
  for (int i = 0; i < 29; ++i) key[i] = fragment_key[i];
  
  //The id as string
  string sid = str(id);
  
  //Build path to feed file
  string feed_path = "encrypted\\";
  if (id % 100 < 10) feed_path += '0';
  feed_path += str(id % 100);
  feed_path += "\\";
  feed_path += sid;
  feed_path += ".feed";
  
  //Get encrypted content of feed file
  string in = get_file_contents(feed_path);
  
  encslength = in.size();
  inputslength = in.size();
  unsigned char encrypted[encslength + 1], decrypted[inputslength + 1];
  for (int i = 0; i < int(in.size()); ++i) encrypted[i] = in[i];
  
  //Tries every combination of the 3 characters left
  for (char c1 = 'A'; c1 <= 'z'; ++c1) {
    if (c1 > 'Z' and c1 < 'a') continue;
    key[29] = c1;
    for (char c2 = 'A'; c2 <= 'z'; ++c2) {
      if (c2 > 'Z' and c2 < 'a') continue;
      key[30] = c2;
      for (char c3 = 'A'; c3 <= 'z'; ++c3) {
        if (c3 > 'Z' and c3 < 'a') continue;
        key[31] = c3;
        
        //Tries decryption with characters c1, c2, c3
        myDecrypt(encrypted, decrypted, key);
        
        //Checks first five characters, as they must be the given id
        bool valid = true;
        for (int i = 0; i < 5; ++i) if (sid[i] != decrypted[i]) valid = false;
        
        //If the decryption was correct
        if (valid) {
          //Parses the content to a string
          int p = 0;
          string sfeed = "";
          while (decrypted[p] != '\0') sfeed += decrypted[p++];
          
          //Create a stream with it
          stringstream ss(sfeed);
          
          //Reads events from decrypted feed, adding them to 'pq'
          int user_id, timestamp, event_id;
          while (ss >> user_id >> timestamp >> event_id) {
            pq.push(make_pair(-timestamp, event_id));
          }
          return;
        }
      }
    }
  }
}


int main(int argc, char **argv) {
  //Faster input
  ios::sync_with_stdio(false);

  string line;
  while (getline(cin, line)) {
    stringstream ss(line);
    
    //Empties 'pq' from previous data
    while (not pq.empty()) pq.pop();
    
    //Number of events
    int n;
    ss >> n;
    
    ss.ignore(1); //Skip ';'
    
    vector<int> ids;
    vector<string> fragments;
    int id;
    
    //Queue to process users in optimal order
    priority_queue<pair<int, int> > user_queue;
    
    //Reads users
    while (ss >> id) {
      ss.ignore(1); //Skip ','
      
      //Fragment of key
      string fragment;
      ss >> fragment;
      
      ss.ignore(1); //Skip ';'
      
      //This user will have this index
      int index = ids.size();
      
      //Store user data
      ids.push_back(id);
      fragments.push_back(fragment);
      
      //Build path to timestamp files
      string time_path = "last_times\\";
      if (id % 100 < 10) time_path += '0';
      time_path += str(id % 100);
      time_path += "\\";
      time_path += str(id);
      time_path += ".timestamp";
      
      //Read user's last event time
      ifstream file_time(time_path.c_str());
      int last_time;
      file_time >> last_time;
      file_time.close();
      
      //Insert into queue, so that they are ordered by their last event
      user_queue.push(make_pair(last_time, index));
    }
    
    //Process users until there are n events and it is impossible to get
    //older events, or until there are no users left
    while (not user_queue.empty()) {
      //Next user's data
      int oldest = -user_queue.top().first;
      int index = user_queue.top().second;
      user_queue.pop();
      
      //If there are not events enough, insert user's events in 'pq'
      if (pq.size() < n) crack_id(ids[index], fragments[index]);
      else {
        //If the number of events is greater than 'n', some of them are useless.
        while (pq.size() > n and pq.top().first > oldest) pq.pop();
        
        //This user's events occur before or equal to 'oldest', so if
        //oldest timestamp is lower than n-th event, this user and all
        //the rest are useless and there's no need to process them
        if (pq.top().first > oldest) crack_id(ids[index], fragments[index]);
        else break;
      }
    }
    
    //Remove the surplus of events
    while (pq.size() > n) pq.pop();
    
    //Order them in inverse order
    vector<int> v(pq.size());
    for (int i = v.size() - 1; i >= 0; --i) {
      v[i] = pq.top().second;
      pq.pop();
    }
    
    //Print solution: event ids
    for (int i = 0; i < v.size(); ++i) {
      if (i) cout << " ";
      cout << v[i];
    }
    cout << endl;
  }
}