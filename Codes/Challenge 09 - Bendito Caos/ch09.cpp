#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <cstring>
#include <cstdlib>
using namespace std;

typedef long long ll;
typedef vector<int> VI;
typedef vector<VI> VVI;
const ll INF = 1000000000000000000LL;

#define VEI(w,e) ((E[e].u == w) ? E[e].v : E[e].u)
#define CAP(w,e) ((E[e].u == w) ? E[e].cap[0] - E[e].flow : E[e].cap[1] + E[e].flow)
#define ADD(w,e,f) E[e].flow += ((E[e].u == w) ? (f) : (-(f)))

struct Edge {
  int u, v;
  ll cap[2], flow;
};

VI d, act;

//Return if there is a path from 's' to 't' using non 
//saturated edges.
bool bfs(int s, int t, VVI& adj, vector<Edge>& E) {
  queue<int> Q;
  d = VI(adj.size(), -1);
  d[t] = 0;
  Q.push(t);
  while (not Q.empty()) {
    int u = Q.front(); Q.pop();
    for (int i = 0; i < adj[u].size(); ++i) {
      int e = adj[u][i], v = VEI(u, e);
      if (CAP(v, e) > 0 and d[v] == -1) {
        d[v] = d[u] + 1;
        Q.push(v);
      }
    }
  }
  return d[s] >= 0;
}

//Return how many flow can get from 'u' to 't', using 
//only shortest paths found by 'bfs' function
ll dfs(int u, int t, ll bot, VVI& adj, vector<Edge>& E) {
  if (u == t) return bot;
  for (; act[u] < adj[u].size(); ++act[u]) {
    int e = adj[u][act[u]];
    if (CAP(u, e) > 0 and d[u] == d[VEI(u, e)] + 1) {
      ll inc = dfs(VEI(u, e), t, min(bot, CAP(u, e)), adj, E);
      if (inc) {
        ADD(u, e, inc);
        return inc;
      }
    }
  }
  return 0;
}

//Computes the maximum flow from node 's' to node 't',
//in the graph defined by 'adj' adjacency list and
// 'E' as the list of edges.
ll maxflow(int s, int t, VVI& adj, vector<Edge>& E) {
  for (int i = 0; i < E.size(); ++i) E[i].flow = 0;
  ll flow = 0, bot;
  while (bfs(s, t, adj, E)) {
    act = VI(adj.size(), 0);
    while ((bot = dfs(s, t, INF, adj, E))) flow += bot;
  } 
  return flow;
}

//Adds edge (u, v) to the graph, with capacity 'cap'
void add_edge(int u, int v, int cap, vector<vector<int> >& adj, vector<Edge>& E) {
  Edge e;
  e.u = u;
  e.v = v;
  e.cap[0] = cap;
  e.cap[1] = 0;
  adj[e.u].push_back(E.size());
  adj[e.v].push_back(E.size());
  E.push_back(e);
}

int main() {
  //Number of cases (cities)
  int c;
  cin >> c;
  for (int k = 0; k < c; ++k) {
    //Input variables
    string city_name;
    int S, D, I, R;
    
    //Read input
    cin >> city_name >> S >> D >> I >> R;
    
    //Create list of edges and adjacency list
    vector<vector<int> > adj(I + 2);
    vector<Edge> E;
    
    //Read roads and add them to the graph
    for (int j = 0; j < R; ++j) {
      string a, b, type;
      int lanes;
      cin >> a >> b >> type >> lanes;
      
      //Converts input cities to indexes
      int u, v;
      if (a == city_name) u = I;
      else if (a == "AwesomeVille") u = I + 1;
      else u = atoi(a.c_str());
      
      if (b == city_name) v = I;
      else if (b == "AwesomeVille") v = I + 1;
      else v = atoi(b.c_str());
      
      //Computes the edge capacity
      int cap = 200 * lanes;
      if (type == "normal") cap *= S;
      else cap *= D;
      
      //Adds edge
      add_edge(u, v, cap, adj, E);
    }
    
    //Print solution: maximum flow from city_name to AwesomeVille
    cout << city_name << " " << maxflow(I, I + 1, adj, E) << endl;
  }
}