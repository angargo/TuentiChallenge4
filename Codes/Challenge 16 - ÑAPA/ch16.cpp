#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
using namespace std;

vector<vector<vector<int> > > circle_box;
vector<int> x, y, radius;

//Directions
int di[] = {2, 1, 0, -1, -2, 2, 1, 0, -1, -2, 2, 1, 0, -1, -2, 2, 1, 0, -1, -2, 2, 1, 0, -1, -2};
int dj[] = {2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2};

//Goes to specified line in specified stream
ifstream& GotoLine(ifstream& file, int num){
  file.seekg(ios::beg);
  for(int i = 0; i < num - 1; ++i){
    file.ignore(numeric_limits<streamsize>::max(),'\n');
  }
  return file;
}

//Load 'n' circles from file, starting at circle 'f'
void load_circles(int f, int n) {
  x = y = radius = vector<int>(n);
  circle_box = vector<vector<vector<int> > >(200, vector<vector<int> >(200));
  
  //Open stream
  ifstream file("points");
  
  //Go to line 'f'
  GotoLine(file, f);
  
  //Reads 'n' circles, adding their index to their corresponding box
  for (int i = 0; i < n; ++i) {
    file >> x[i] >> y[i] >> radius[i];
    circle_box[x[i] / 500][y[i] / 500].push_back(i);
  }
  
  //Close stream
  file.close();
}

//Returns the square of number 'n'
int sq(int n) {
  return n * n;
}

//Tells if circle with index 'p' and circle with index 'q' intersect
bool intersection(int p, int q) {
  return sq(x[p] - x[q]) + sq(y[p] - y[q]) <= sq(radius[p] + radius[q]);
}

//Count the number of circles that intersect with circle 'p'
int check(int p) {
  int count = 0;
  
  //There is only need to check circles in circle_box at distance
  //at most 2, because every box has side 500, and the maximum
  //radius is 500.
  
  //For all boxes at distance 2 or less
  for (int d = 0; d < 25; ++d) {
    int i = x[p] / 500 + di[d], j = y[p] / 500 + dj[d];
    
    //Check index out of bounds
    if (i >= 0 and j >= 0 and i < 200 and j < 200) {
    
      //Try every circle in that box
      int e = circle_box[i][j].size();
      for (int k = 0; k < e; ++k) {
        int q = circle_box[i][j][k];
        if (q == p) continue;
        if (intersection(p, q)) ++count;
      }
    }
  }
  
  //Result
  return count;
}

int main() {
  //Faster input, output
  ios::sync_with_stdio(false);
  
  //Input
  int f, n;
  cin >> f;
  cin.ignore(1); //Skip ','
  cin >> n;
  
  //Load circles from file
  load_circles(f, n);
  
  //Check all circles
  int count = 0;
  for (int i = 0; i < n; ++i) count += check(i);
  
  //Print solution (divided by two because every intersection
  //is counted twice).
  cout << count / 2 << endl;
}