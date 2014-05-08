#include <iostream>
#include <vector>
#include <queue>
using namespace std;

//Directions
int di[] = {0, 1, 0, -1};
int dj[] = {1, 0, -1, 0};

//Each case is solved in this function
void solve_case() {
  //Read map dimensions
  int w, h;
  cin >> w >> h;
  
  vector<string> map(h);
  vector<int> start(3);
  
  //Read the map from input, recording the 'S' position
  for (int i = 0; i < h; ++i) {
    cin >> map[i];
    for (int j = 0; j < w; ++j) if (map[i][j] == 'S') {
      start[0] = i;
      start[1] = j;
    }
  }
  
  //queue and distances to do a breadth-first search
  queue<vector<int> > q;
  vector<vector<vector<int> > > dist(h, vector<vector<int> >(w, vector<int>(4, -1)));
  
  //There are four starting nodes: the starting position
  //with each of the four directions
  for (int d = 0; d < 4; ++d) {
    dist[start[0]][start[1]][d] = 0;
    start[2] = d;
    q.push(start);
  }
  
  while (not q.empty()) {
    vector<int> u = q.front();
    q.pop();
    
    //Check destination
    if (map[u[0]][u[1]] == 'X') {
      cout << dist[u[0]][u[1]][u[2]] << endl;
      return;
    }
    
    //Keep on with the same direction
    int ni = u[0] + di[u[2]], nj = u[1] + dj[u[2]];
    if (ni >= 0 and nj >= 0 and ni < h and nj < w and map[ni][nj] != '#' and dist[ni][nj][u[2]] == -1) {
      vector<int> v(3);
      v[0] = ni;
      v[1] = nj;
      v[2] = u[2];
      dist[ni][nj][v[2]] = dist[u[0]][u[1]][u[2]] + 1;
      q.push(v);
    }
    
    //Turn to the right (directions are chosen so that turning 
    //to the right is adding 1 to the current direction)
    int nd = (u[2] + 1) % 4;
    ni = u[0] + di[nd];
    nj = u[1] + dj[nd];
    if (ni >= 0 and nj >= 0 and ni < h and nj < w and map[ni][nj] != '#' and dist[ni][nj][nd] == -1) {
      vector<int> v(3);
      v[0] = ni;
      v[1] = nj;
      v[2] = nd;
      dist[ni][nj][nd] = dist[u[0]][u[1]][u[2]] + 1;
      q.push(v);
    }
  }
  
  //There was no solution
  cout << "ERROR" << endl;
}

int main() {
  //Number of cases
  int t;
  cin >> t;
  
  for (int i = 0; i < t; ++i) {
    //Solve case 'i+1'
    cout << "Case #" << i + 1 << ": ";
    solve_case();
  }
}