#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;


struct Poll {
  int gender, age, year;
  string studies;
};

//Maximum length of names and studies
const int SIZE = 50;

//Total order between the polls, so they can be used as key in maps
bool operator < (const Poll& a, const Poll& b) {
  bool result;
  if (a.age == b.age) {
    if (a.year == b.year) {
      if (a.gender == b.gender) result = a.studies < b.studies;
      else result = a.gender < b.gender;
    }
    else result = a.year < b.year;
  }
  else result = a.age < b.age;
  return result;
}

//Read the contents of a poll(gender, age, year, studies)
//Receives as parameter the stream from which it reads
Poll read_poll(istream& list_students) {
  char current_studies[SIZE];
  Poll current_poll;
  
  //Set gender as 1 if it is male and 0 otherwise
  current_poll.gender = (list_students.get() == 'M' ? 1 : 0);
  list_students.ignore(1); //Skip the ','
  
  list_students >> current_poll.age;
  list_students.ignore(1); //Skip the ','
  
  list_students.get(current_studies, SIZE, ',');
  current_poll.studies = string(current_studies);
  list_students.ignore(1); //Skip the ','
  
  list_students >> current_poll.year;
  list_students.ignore(1); //Skip the '\n'
  
  return current_poll;
}

//Read the data of a student (name and poll) from list_students
//and stores it in list, using the poll as key.
void read_student(ifstream& list_students, map<Poll, vector<string> >& list) {
  char current_name[SIZE];
  Poll current_poll;
  
  list_students.get(current_name, SIZE, ',');
  list_students.ignore(1); //Skip the ','
  
  current_poll = read_poll(list_students);
  
  //Set flags so that good() works correctly
  list_students.peek();
  
  //Add this name to the list of students whose answer is this poll
  list[current_poll].push_back(string(current_name));
}

int main() {
  //Create a stream to read from the given file
  ifstream list_students("students");
  
  //For each poll, this stores a list of the students which gave it as answer
  map<Poll, vector<string> > list;
  while (list_students.good()) read_student(list_students, list);
  list_students.close();
  
  //All of those lists are sorted lexicographically
  for (map<Poll, vector<string> >::iterator it = list.begin(); it != list.end(); ++it)
    sort(it->second.begin(), it->second.end());
  
  //Number of cases
  int T;
  cin >> T;
  cin.ignore(1); //Skip the '\n'
  
  for (int i = 0; i < T; ++i) {
    //Each case is computed here
    cout << "Case #" << i + 1 << ": ";
    
    Poll query_poll = read_poll(cin);
    int res = list[query_poll].size();
    
    //Check the size of poll list
    if (res == 0) cout << "NONE" << endl;
    else {
      //if the list is not empty, write its contents
      for (int j = 0; j < res; ++j) {
        if (j != 0) cout << ',';
        cout << list[query_poll][j];
      }
      cout << endl;
    }
  }
}