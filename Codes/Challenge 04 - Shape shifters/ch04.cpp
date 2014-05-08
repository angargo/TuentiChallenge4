#include <iostream>
#include <vector>
#include <set>
#include <queue>
using namespace std;


//The size of the DNA sequences
int size;

//Show the path from the start to u
//previous indicates the previous node in the path and names
//stores the string asociated to each node
void show(int u, vector<int>& previous, vector<string>& names) {
  if (previous[u] != -2) {
    show(previous[u], previous, names);
    cout << "->";
  }
  cout << names[u];
}

//Given two DNA sequences a and b,
//returns if one can shift to the other
bool adjacent(string& a, string& b) {
  bool equal = true;
  for (int i = 0; i < size; ++i) {
    if (a[i] != b[i]) {
      if (equal) equal = false;
      else return false;
    }
  }
  return true;
}

int main() {
  //Read the starting and ending sequences
  string start, end;
  getline(cin, start);
  getline(cin, end);
  
  //Set the sizes of DNA sequences
  size = start.size();
  
  //Store in a set all safe states
  set<string> safe;
  string state;
  while (getline(cin, state)) safe.insert(state);
  
  //Convert it to vector, so the states are indexed
  vector<string> nodes(safe.begin(), safe.end());
  int n = nodes.size();
  
  //Create the adjacency list of the graph
  vector<vector<int> > list(n);
  int istart, iend;
  for (int i = 0; i < n; ++i) {
    //Save the indexes of starting and ending positions
    if (nodes[i] == start) istart = i;
    if (nodes[i] == end) iend = i;
    
    for (int j = i + 1; j < n; ++j) {
      if (adjacent(nodes[i], nodes[j])) {
        list[i].push_back(j);
        list[j].push_back(i);
      }
    }
  }
  
  //Find the shortest path with a breadth-first search
  vector<int> previo(n, -1);
  queue<int> q;
  q.push(istart);
  previo[istart] = -2;
  while (not q.empty()) {
    int u = q.front();
    q.pop();
    if (u == iend) break;
    int e = list[u].size();
    for (int i = 0; i < e; ++i) {
      int v = list[u][i];
      if (previo[v] == -1) {
        previo[v] = u;
        q.push(v);
      }
    }
  }
  
  //Print the path
  show(iend, previo, nodes);
  cout << endl;
}
