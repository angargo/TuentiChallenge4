#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

int main() {
  //Read keyphrase from input
  string keyphrase;
  getline(cin, keyphrase);
  
  //Print it in a file, to be read from myclient.js
  ofstream out("ch06.in");
  out << keyphrase;
  out.close();
  
  //Run myclient.js
  system("node myclient.js");
}