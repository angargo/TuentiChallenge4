#include <iostream>
#include <vector>
using namespace std;

int m, ri, rj;
vector<vector<int> > board;
bool corner1, corner2, corner3, corner4;

//Valid directions
int di[] = {1, 1, 1, 0, 0, -1, -1, -1};
int dj[] = {1, 0, -1, 1, -1, 1, 0, -1};

//Checks if who's movement starting in (i,j)
//makes the tiles swap in direction d, and
//return the number of tiles that can be swapped
int valid(int i, int j, int d, int who) {
  int ni = i, nj = j;
  int c = 0;
  do {
    ni += di[d];
    nj += dj[d];
    
    if (ni > 7 or ni < 0 or nj > 7 or nj < 0) return 0;
    if (board[ni][nj] == 0) return 0;
    
    if (board[ni][nj] == 3 - who) ++c;
    else break;
    
  } while (board[ni][nj] != who);
  return c;
}

//Swaps c tiles in direction d starting from (i, j)
void flip(int i, int j, int d, int c) {
  int ni = i + di[d];
  int nj = j + dj[d];
  while (c > 0) {
    board[ni][nj] = 3 - board[ni][nj];
    ni += di[d];
    nj += dj[d];
    --c;
  }
}

//Tries every possibility with a min-max:
// -If 1 finds a winning move, he wins.
// -If 2 finds a losing move, he wins.
//Returns if there is a winning move for 1.
bool backtracking(int k, int who) {
  if (k == 2 * m - 1) {
    //End of backtracking. If there is a new corner under domain, 1 wins
    if (board[0][0] == 1 and not corner1) return true;
    if (board[7][0] == 1 and not corner2) return true;
    if (board[0][7] == 1 and not corner3) return true;
    if (board[7][7] == 1 and not corner4) return true;
    
    //Otherwise, he doesn't
    return false;
  }
  
  //Stores if there was a valid move
  bool play = false;
  
  for (int j = 0; j < 8; ++j) {
    for (int i = 0; i < 8; ++i) {
      if (board[i][j] == 0) { //Try every empty position
        
        //Check if this position is a valid move
        vector<pair<int, int> > dirs;
        for (int d = 0; d < 8; ++d) {
          int c = valid(i, j, d, who);
          if (c > 0) {
            //Found valid direction
            dirs.push_back(make_pair(d, c));
            
            //Flips enemy's tiles
            flip(i, j, d, c);
          }
        }
        
        //Puts its tile
        board[i][j] = who;
        bool winner;
        
        if (dirs.size() > 0) {
          //There is a valid movement
          play = true;
          
          //Try possibilities after this move, and store if it is a winning move
          winner = backtracking(k + 1, 3 - who);
        }
        
        //Undo the movement
        board[i][j] = 0;
        for (int id = 0; id < int(dirs.size()); ++id) {
          flip(i, j, dirs[id].first, dirs[id].second);
        }
        
        //If there was a movement, check if there is need to try more
        if (dirs.size() > 0) {
          //If it is winning move
          if (winner) {
            //And it is 1's turn, there is no need to try more. He wins.
            if (k % 2 == 0) {
              //If it is the first move, it is stored to print as solution
              if (k == 0) {
                ri = i;
                rj = j;
              }
              
              return true;
            }
          }
          //If it a losing move, and it is 2's turn, he wins.
          else if (k % 2 == 1) return false;
        }
      }
    }
  }
  
  //If there was not a valid move, it is compulsory to skip turn
  if (not play) return backtracking(k + 1, 3 - who);
  
  //There was no wining moves if it is 1's turn.
  if (k % 2 == 0) return false;
  
  //Or there was no losing moves if it is 2's turn.
  else return true;
}

//Every case is solved by this function
void solve() {
  
  string color;
  cin >> color;
  cin.ignore(4); //Skip " in "
  cin >> m;
  
  //Read board from input. The specified color is 1 and
  //the losing color is 2. Empty cells are marked with 0.
  board = vector<vector<int> >(8, vector<int>(8));
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      char c; cin >> c;
      if (c == '.') board[i][j] = 0;
      else if (c == 'X') {
        if (color == "White") board[i][j] = 2;
        else board[i][j] = 1;
      }
      else if (color == "White") board[i][j] = 1;
      else board[i][j] = 2;
    }
  }
  
  //Check which corners are already under domain
  corner1 = corner2 = corner3 = corner4 = false;
  if (board[0][0] == 1) corner1 = true;
  if (board[7][0] == 1) corner2 = true;
  if (board[0][7] == 1) corner3 = true;
  if (board[7][7] == 1) corner4 = true;
  
  //Winning position
  ri = rj = -1;
  
  //Try all possibilities and print solution
  if (backtracking(0, 1)) cout << char('a' + rj) << ri + 1 << endl;
  else cout << "Impossible" << endl;
}

int main() {
  //Number of cases
  int t;
  cin >> t;
  
  for (int i = 0; i < t; ++i) solve();
}