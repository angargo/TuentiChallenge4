#include <iostream>
#include <vector>
#include <map>
using namespace std;

//Directions of adjacent cells
int di[] = {1, 1, 1, 0, 0, -1, -1, -1};
int dj[] = {1, 0, -1, 1, -1, 1, 0, -1};

//Given current_grid (10x10 matrix) with the current
//generation, computes the next one and stores it in current_grid
void next_generation(vector<vector<int> >& current_grid) {
  vector<vector<int> > next_grid(10, vector<int>(10, 0));
  for (int i = 1; i <= 8; ++i) {
    for (int j = 1; j <= 8; ++j) {
      int alive = 0;
      for (int d = 0; d < 8; ++d) alive += current_grid[i + di[d]][j + dj[d]];
      if (current_grid[i][j] == 0) {
        if (alive == 3) next_grid[i][j] = 1;
      }
      else if (alive == 2 or alive == 3) next_grid[i][j] = 1;
    }
  }
  current_grid = next_grid;
}

int main() {
  //The grid is 10x10, with empty borders to simplify
  
  //Read the initial grid
  vector<vector<int> > grid(10, vector<int>(10, 0));
  for (int i = 1; i <= 8; ++i) {
    for (int j = 1; j <= 8; ++j) {
      char c;
      cin >> c;
      if (c == 'X') grid[i][j] = 1;
    }
  }
  
  //Create a map which, given a grid, return its generation number
  map<vector<vector<int> >, int> generation;
  
  //Simulate generations until there is a repetition
  int current_generation = 0;
  do {
    generation[grid] = current_generation;
    ++current_generation;
    next_generation(grid);
  } while (generation.count(grid) == 0);
  
  //Print result
  cout << generation[grid] << " " << current_generation - generation[grid] << endl;
}