#include <iostream>
#include <cmath>
using namespace std;

int main() {
  //Number of cases
  int N;
  cin >> N;
  
  for (int i = 0; i < N; ++i) {
    //Each case is solved here
    int X, Y;
    cin >> X >> Y;
    
    //Pythagorean theorem
    cout << round(100 * sqrt(double(X * X + Y * Y))) / 100 << endl;
  }
}