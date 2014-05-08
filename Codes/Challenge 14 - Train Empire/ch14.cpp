#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <sstream>
using namespace std;

const double eps = 1e-5;
const int inf = 1e8;

int s, r, f;
vector<vector<vector<double> > > dist;
vector<int> x, y, dest, value;

map<vector<int>, map<vector<int>, map<vector<double>, int> > > memo;

double c(double a) {
  return a * a;
}

//Tries all possibilities with wagons in position 'pos_wagon',
//trains in position 'pos_train' and each train has a certain
//'fuel' left. Returns the maximum of points achieved
int backtracking(vector<int>& pos_wagon, vector<int>& pos_train, vector<double>& fuel) {

  //If this solution is already computed, return solution
  if (memo[pos_wagon][pos_train].count(fuel) != 0) return memo[pos_wagon][pos_train][fuel];
  
  int res = 0;
  //Compute current points
  for (int i = 0; i < s; ++i) if (pos_wagon[i] == dest[i]) res += value[i];
  
  //Lets try to move every train
  for (int k = 0; k < r; ++k) {
    //To every position
    for (int i = 0; i < s; ++i) {
      //If it is possible to go to that position
      if (dist[k][pos_train[k]][i] <= fuel[k] + eps) {
        int pos = pos_train[k];
        fuel[k] -= dist[k][pos][i];
        pos_train[k] = i;
        
        //Try the train going without carrying a wagon
        res = max(res, backtracking(pos_wagon, pos_train, fuel));
        
        for (int j = 0; j < s; ++j) {
          if (pos_wagon[j] == pos) {
            //For every wagon which is in position 'pos',
            //try to get it carried by the train to 'i'
            pos_wagon[j] = i;
            res = max(res, backtracking(pos_wagon, pos_train, fuel));
            
            //Take it back
            pos_wagon[j] = pos;
          }
        }
        
        //Let everything like it was before starting
        pos_train[k] = pos;
        fuel[k] += dist[k][pos][i];
      }
    }
  }
  
  return memo[pos_wagon][pos_train][fuel] = res;
}

//Every case is solved by this function
void solve_case() {

  //Read input
  cin >> s;
  cin.ignore(1); //Skip ','
  cin >> r;
  cin.ignore(1); //Skip ','
  cin >> f;
  
  //Read coordinates and names of stations, and index them
  map<string, int> index;
  x = y = dest = value = vector<int>(s);
  vector<string> tmp_dest(s), name(s);
  for (int i = 0; i < s; ++i) {
    cin >> name[i] >> x[i];
    cin.ignore(1); //Skip ','
    cin >> y[i] >> tmp_dest[i] >> value[i];
    index[name[i]] = i;
  }
  
  //Assign to each wagon, the index of its destination station
  for (int i = 0; i < s; ++i) dest[i] = index[tmp_dest[i]];
  
  //Distances between stations by each line
  dist = vector<vector<vector<double> > >(r, vector<vector<double> >(s, vector<double>(s, inf)));
  
  cin.ignore(1); //Skip
  
  //Stores the starting positions of the trains
  vector<int> start(r);
  
  //Reads the lines
  for (int i = 0; i < r; ++i) {
    string line;
    getline(cin, line);
    stringstream ss(line);
    
    //Starting station
    string start_name;
    ss >> start_name;
    start[i] = index[start_name];
    
    //List of edges
    string edge;
    while (ss >> edge) {
      string first = "";
      int p = 0;
      while (edge[p] != '-') first += edge[p++];
      ++p;
      string second = "";
      while (p < int(edge.size())) second += edge[p++];
      
      //Improve distance between 'a' and 'b'
      int a = index[first], b = index[second];
      dist[i][a][b] = dist[i][b][a] = min(dist[i][a][b], sqrt(c(x[a] - x[b]) + c(y[a] - y[b])));
    }
  }
  
  //Starting stats
  vector<int> pos(s);
  vector<double> fuel(r);
  for (int i = 0; i < s; ++i) pos[i] = i;
  for (int i = 0; i < r; ++i) fuel[i] = f;
  
  //Empty memoization from previous case
  memo.clear();
  
  //Try all possibilities and print solution
  cout << backtracking(pos, start, fuel) << endl;
  
}

int main() {
  //Number of cases
  int n;
  cin >> n;
  
  for (int i = 0; i < n; ++i) solve_case();
}