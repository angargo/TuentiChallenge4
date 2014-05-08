#include <iostream>
#include <vector>
using namespace std;

//Directions: right, down, up, left
const int dx[] = {1, 0, 0, -1};
const int dy[] = {0, 1, -1, 0};

int main() {
  //Read the track from input
  string track;
  getline(cin, track);
  
  //x, y, d are the current position and direction
  //xmin, ymin, xmax, ymax are the borders achieved
  int x, y, xmin, ymin, xmax, ymax, d;
  x = y = xmin = ymin = xmax = ymax = d = 0;
  
  int length = track.size();
  
  //Search for the starting point of the race, the '#'
  int start = 0;
  for (int i = 0; i < length; ++i) {
    if (track[i] == '#') {
      start = i;
      break;
    }
  }
  
  //Duplicate the string, so that start + length - 1 is still inside
  track += track;
  
  //First lap to the circuit, to obtain the dimensions
  for (int i = start; i < start + length; ++i) {
    //The directions are chosen so that this works
    if (track[i] == '/') d ^= 2;
    if (track[i] == '\\') d ^= 1;
    x += dx[d];
    y += dy[d];
    xmin = min(x, xmin);
    xmax = max(x, xmax);
    ymin = min(y, ymin);
    ymax = max(y, ymax);
  }
  
  //Dimensions
  int h = ymax - ymin + 1;
  int w = xmax - xmin + 1;
  
  //Set (-xmin, -ymin) as offset, to make all coordinates non-negative
  x = -xmin;
  y = -ymin;
  d = 0;
  
  //The circuit at bird's eye
  vector<vector<char> > map(h, vector<char>(w, ' '));
  
  //Second lap, to draw the circuit on map
  for (int i = start; i < start + length; ++i) {
  
    //Turn left or right
    if (track[i] == '/') d ^= 2;
    if (track[i] == '\\') d ^= 1;
    
    //Draw the circuit
    if (track[i] == '-') {
      if (d == 0 or d == 3) map[y][x] = '-';
      else map[y][x] = '|';
    }
    else map[y][x] = track[i];
    
    //Next position
    x += dx[d];
    y += dy[d];
  }
  
  //print the map
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) cout << map[i][j];
    cout << endl;
  }
}