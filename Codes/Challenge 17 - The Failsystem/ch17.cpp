#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <cstdlib>
using namespace std;

void show(unsigned char attrib, int l) {
  if (l == 0) return;
  show(attrib / 2, l - 1);
  cout << attrib % 2;
}

//Names of the smaller parts of the file system
string filename[] = {"TUENTIDISK.BIN.001", "TUENTIDISK.BIN.002", "TUENTIDISK.BIN.003",
  "TUENTIDISK.BIN.004", "TUENTIDISK.BIN.005", "TUENTIDISK.BIN.006"};
  
ifstream file;

//Variables read from Partition and Volume ID
unsigned long long Partition_LBA_Begin = 0x00000800;
unsigned long long BPB_SecPerClus = 0x08;
unsigned long long Number_of_Reserved_Sectors = 0x0002;
unsigned long long Number_of_FATs = 0x01;
unsigned long long Sectors_Per_FAT = 0x000027f5;
unsigned long long BPB_RootClus = 0x00000002;
unsigned long long Corrupt_Cluster = 0x0ffffff7;

//Useful constants
unsigned long long fat_begin_lba = Partition_LBA_Begin + Number_of_Reserved_Sectors;
unsigned long long cluster_begin_lba = Partition_LBA_Begin + Number_of_Reserved_Sectors + (Number_of_FATs * Sectors_Per_FAT);
unsigned long long sectors_per_cluster = BPB_SecPerClus;
unsigned long long root_dir_first_cluster = BPB_RootClus;

map<string, unsigned long long> File, Directory, Size;

//Set the input stream in the specified position
//The file is divided in parts of 1GB
void go_to_pos(unsigned long long pos) {

  file.close();
  file.open(filename[pos / 1073741824].c_str(), ios::binary);
  file.seekg(int(pos % 1073741824), ios::beg);
}

//Returns the content of the cluster-th entry in the FAT
unsigned long long fat(unsigned long long cluster) {

  //Set the stream at correct position
  go_to_pos(fat_begin_lba * 0x200 + cluster * 4);
  
  //Read 4 bytes
  unsigned long long current_fat = 0;
  for (unsigned long long i = 0; i < 4; ++i) 
    current_fat += (((unsigned long long)1) << (8 * i)) * file.get();
  
  //First 4 bits must be ignored
  return (current_fat & 0x0fffffff);
}

//Returns the starting cluster of the f-th entry of the 
//directory recorded in cluster_lba
unsigned long long start_cluster(unsigned long long cluster_lba, int f) {

  unsigned long long cluster = 0;
  
  //Lower 2 bytes
  go_to_pos(cluster_lba * 0x200 + 0x20 * f + 0x1a);
  cluster += file.get() + 256LL * file.get();
  
  //Higher 2 bytes
  go_to_pos(cluster_lba * 0x200 + 0x20 * f + 0x14);
  cluster += 256LL * 256LL * (file.get() + 256LL * file.get());
  
  return cluster;
}

//Reads the name of the f-th entry of the directory
//recorded in cluster_lba
string read_name(unsigned long long cluster_lba, int f) {

  //Set the stream in correct position
  go_to_pos(cluster_lba * 0x200 + 0x20 * f);
  
  //Read 8 bytes
  string name = "";
  for (int i = 0; i < 8; ++i) {
    unsigned char c = file.get();
    if (c == 0x20) break;
    name += char(c);
  }
  
  return name;
}

//Reads the extension of the f-th entry of the directory
//recorded in cluster_lba
string read_extension(unsigned long long cluster_lba, int f) {

  //Set the stream in correct position
  go_to_pos(cluster_lba * 0x200 + 0x20 * f + 8);
  
  //Read 3 bytes
  string extension = "";
  for (int i = 0; i < 3; ++i) {
    unsigned char c = file.get();
    if (c == 0x20) break;
    extension += char(c);
  }
  
  return extension;
}

//Given the contents of a FAT, returns true if there is sign
//of it being incorrect
bool Corrupt(unsigned long long cluster) {
  return (cluster == Corrupt_Cluster or cluster < 2);
}

//Loads the directory recorded in 'cluster'
bool load_directory(unsigned long long cluster) {
  
  //Computes the LBA of 'cluster'
  unsigned long long cluster_lba = cluster_begin_lba + (cluster - 2) * sectors_per_cluster;
  
  //There are 128 entries in each cluster
  int f = 0;
  while (f < 128) {
  
    //Set the stream at the correct position
    go_to_pos(cluster_lba * 0x200 + 0x20 * f);
    
    //Check its first character: end-of-directory(0x00) or deleted-file(0xe5 or 0x05)
    unsigned char c = file.get();
    if (c == 0x00) return true;
    if (c != 0xe5 or c != 0x05) {
    
      //Read file or directory name
      string name = read_name(cluster_lba, f);
      string extension = read_extension(cluster_lba, f);
      if (extension != "") {
        name += '.';
        name += extension;
      }
      
      //Read attribute byte
      go_to_pos(cluster_lba * 0x200 + f * 0x20 + 11);
      unsigned char attrib = file.get();
      
      //Check if Directory
      if ((attrib & 16) != 0) Directory[name] = start_cluster(cluster_lba, f);
      
      //Or File
      else {
        File[name] = start_cluster(cluster_lba, f);
        
        //Read file size from last 4 bytes of current entry
        go_to_pos(cluster_lba * 0x200 + f * 0x20 + 0x1c);
        unsigned long long size = 0;
        for (int i = 0; i < 4; ++i) size += (1LL << (8 * i)) * file.get();
        Size[name] = size;
      }
    }
    ++f;
  }
  
  //Get next cluster from fat
  unsigned long long next_cluster = fat(cluster);
  
  //Check if it is a correct direction
  if (Corrupt(next_cluster)) return false;
  
  //Keep loading directory
  return load_directory(next_cluster);
}

//Returns if a file is corrupt
bool file_corrupt(unsigned long long cluster) {

  //Set of previous cluster in this file
  set<unsigned long long> past_clusters;
  
  //Follow the cluster chain until the end-of-file mark
  while (cluster < 0xffffff8) {
    //Bad direction
    if (Corrupt(cluster)) return true;
    
    //Cluster already in this file (cycle)
    if (past_clusters.count(cluster) == 1) return true;
    
    //Go to the next cluster
    past_clusters.insert(cluster);
    cluster = fat(cluster);
  }
  
  //Not corrupt
  return false;
}

//Loads a file named 'name' which is located in
//the last loaded directory. If there is no such file,
//or it is corrupt, prints "CORRUPT". Otherwise,
//creates a file with its contains in the folder 'files',
//names 'current_file'. Then prints its md5.
void load_file(string name) {

  //Checks if the file is correct
  if (File.count(name) == 0 or file_corrupt(File[name])) {
    cout << "CORRUPT" << endl;
    return;
  }
  
  //Opens the stream
  ofstream out("files\\current_file", ios::binary);
  
  //Starting cluster and size of the file
  unsigned long long cluster = File[name], size = Size[name];
  
  //Follow the cluster chain
  while (cluster < 0xffffff8) {
    
    //Computes LBA
    unsigned long long cluster_lba = cluster_begin_lba + (cluster - 2) * sectors_per_cluster;
    
    //Set stream in correct position
    go_to_pos(cluster_lba * 0x200);
    
    //Reads the whole cluster (or only the bytes left from the file)
    //and writes in 'out' stream
    int current = min((unsigned long long) 0x200 * 8, size);
    char* buffer = new char[current];
    file.read(buffer, current);
    out.write(buffer, current);
    delete [] buffer;
    
    //Update size and cluster
    size -= current;
    cluster = fat(cluster);
  }
  
  //Close stream
  out.close();
  
  //Computes file md5
  system("openssl md5 files\\current_file > ch17.out");
  
  //Stores it in a string
  ifstream in("ch17.out");
  string line;
  getline(in, line);
  in.close();
  
  //Prints the file md5
  int length = line.size();
  for (int i = length - 32; i < length; ++i) cout << line[i];
  cout << endl;
}

//Receives a list of directory names ended with
//the name of a file, and tries to load that file
void check_path(vector<string>& path) {
  
  //Removes data from previous directory
  File.clear();
  Size.clear();
  Directory.clear();
  
  //Loads root directory
  load_directory(root_dir_first_cluster);
  
  int size = path.size();
  for (int i = 0; i < size - 1; ++i) {
    //If there is not such directory, it is a corrupt path
    if (Directory.count(path[i]) == 0) {
      cout << "CORRUPT" << endl;
      return;
    }
    
    //Gets the cluster of the next directory
    unsigned long long cluster = Directory[path[i]];
    
    //Removes data from previous directory
    File.clear();
    Size.clear();
    Directory.clear();
    
    //Loads next directory
    load_directory(cluster);
  }
  
  //Loads the file
  load_file(path.back());
}

int main() {
  //Faster input, output
  ios::sync_with_stdio(false);
  file.open("TUENTIDISK.BIN", ios::binary);
  
  //Number of cases
  int n;
  cin >> n;
  cin.ignore(1); //Skip '\n'
  
  for (int i = 0; i < n; ++i) {
    //Reads path from input
    string path;
    getline(cin, path);
    
    vector<string> Path;
    int pos = 1, size = path.size();
    
    //Separates the path into file and directory names
    while (pos < size) {
      string current = "";
      while (pos < size and path[pos] != '/') current += path[pos++];
      Path.push_back(current);
      ++pos;
    }
    
    //Tries to load the file in 'path'
    check_path(Path);
  }
  
  //Close streams
  out.close();
  file.close();
}