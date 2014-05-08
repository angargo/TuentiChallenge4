#include <iostream>
#include <vector>
#include <map>
#include <fstream>
using namespace std;

//Returns the root of the tree that contains u,
//and set all the middle nodes at distance 1 from it
int Root(int u, vector<int>& root) {
  int r;
  if (root[u] == u) r = u;
  else {
    r = Root(root[u], root);
    root[u] = r;
  }
  return r;
}

int main() {
  //Faster input
  ios::sync_with_stdio(false);
  
  //Mapping between the numbers in the input and {0, 1, 2,...}
  int k = 0;
  map<int, int> m;
  
  //List of edges in order of appearance
  vector<pair<int, int> > edge;
  
  //Read all edges from the given file
  ifstream list_edges("phone_call.log");
  int a, b;
  while (list_edges >> a >> b) {
    //The map is updated dynamically
    if (m.count(a) == 0) m[a] = k++;
    if (m.count(b) == 0) m[b] = k++;
    edge.push_back(make_pair(m[a], m[b]));
  }
  list_edges.close();
  
  //Number of edges
  int l = edge.size();
  
  //IDs and index of the two terrorist
  cin >> a >> b;
  int A = m[a], B = m[b];
  
  //At first, every tree is the trivial tree
  vector<int> root(k);
  for (int i = 0; i < k; ++i) root[i] = i;
  
  //Check the edges until A is in the same tree than B
  int p = 0;
  while (p < l and Root(A, root) != Root(B, root)) {
    int x = edge[p].first, y = edge[p].second;
    int rx = Root(x, root), ry = Root(y, root);
    
    //Join two trees
    root[rx] = ry;
    ++p;
  }
  
  //Print solution
  if (Root(A, root) == Root(B, root)) cout << "Connected at " << p - 1 << endl;
  else cout << "Not connected" << endl;
}