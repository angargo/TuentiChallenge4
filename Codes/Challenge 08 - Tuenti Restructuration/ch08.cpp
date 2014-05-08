#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>
#include <fstream>
using namespace std;

//Reads a name fron the input, and removes from the
//stream the whitespace between the ',' and the next name
string read() {
  char c;
  string name = "";
  
  //Read until ',' or '\n'
  while (cin.get(c) and c != ',' and c != '\n') {
    name += c;
  }
  if (c == ',') cin.ignore(1); //Remove whitespace
  
  return name;
}

//Number of permutations of nine elements
const int nine_factorial = 362880;


map<vector<int>, int> index;
vector<vector<int> > permutation;
vector<int> dist;

//Positions in the table
//0 1 2
//3 4 5
//6 7 8

//Pairs of adjacent positions
int sw[][2] = {
  {0, 1}, {1, 2}, {3, 4}, {4, 5}, {6, 7}, {7, 8},
  {0, 3}, {1, 4}, {2, 5}, {3, 6}, {4, 7}, {5, 8}
};

//Each case is solved by this function
void solve_table() {
  //Read the starting table
  vector<string> table(9);
  for (int i = 0; i < 9; ++i) table[i] = read();
  
  //Blank line between tables
  cin.ignore(1);
  
  vector<int> goal(9);
  for (int i = 0; i < 9; ++i) {
    //Read the ending table
    string name = read();
    
    //Which permutation converts the starting table in the ending table
    for (int j = 0; j < 9; ++j) if (table[j] == name) goal[i] = j;
  }
  
  //Print solution: distance from identity to goal permutation
  if (index.count(goal)) cout << dist[index[goal]] << endl;
  else cout << -1 << endl;
  
  //Blank line between cases
  cin.ignore(1);
}

int main() {
  //For each i, the i-th lexicographic lowest permutation
  //Its inverse is 'index'
  permutation = vector<vector<int> >(nine_factorial);
  
  //Start with identity permutation
  vector<int> p(9);
  for (int i = 0; i < 9; ++i) p[i] = i;
  
  //Fill 'permutation' and 'index' with all permutations
  int k = 0;
  do {
    permutation[k] = p;
    index[p] = k++;
  } while (next_permutation(p.begin(), p.end()));
  
  //With a breadth-first search, compute the shortest path
  //from identity to every other permutation
  dist = vector<int>(nine_factorial, -1);
  queue<int> q;
  q.push(0);
  dist[0] = 0;
  while (not q.empty()) {
    int u = q.front();
    q.pop();
    for (int i = 0; i < 12; ++i) {
      vector<int> p = permutation[u];
      swap(p[sw[i][0]], p[sw[i][1]]);
      int v = index[p];
      if (dist[v] == -1) {
        dist[v] = dist[u] + 1;
        q.push(v);
      }
    }
  }
  
  //Number of cases
  int t;
  cin >> t;
  
  //Skip blank line
  cin.ignore(2);
  
  //Cases
  for (int i = 0; i < t; ++i) solve_table();
}