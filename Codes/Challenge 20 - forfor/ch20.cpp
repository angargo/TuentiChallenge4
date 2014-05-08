#include <iostream>
#include <vector>
using namespace std;

const long long m = 3211123;

//Numerator of the coefficients
const long long num[] = {0, -3, 167, -553, 3829, -7, 119, -1, 1};

//Denominator of the coefficients
const long long den[] = {1, 4, 80, 240, 2880, 16, 1440, 120, 2880};

//Coefficients modulo m
long long coef[9];

//Given function
long long calc(int n) {
  long long ret = 0,a,b,c,d,e,f,g,M = 3211123;

  for(a = 1; a <= n; a++)
    for(b = 1; b <= n; b++)
      for(c = 1; c <= n; c++)
        for(d = 1; d <= n; d++)
          for(e=1; e <= n; e++)
            for(f=1; f <= n; f++)
              for(g=1; g <= n; g++)
                if(a + b + c + d + e + f + g == n)
                  ret = (ret + a*a + b*b + c*c + d*d + e*e + f*f + g*g) % M;
  return ret;
}

//Computes b^e modulo m
long long mpow(long long b, long long e) {
  if (e == 0) return 1;
  long long res = mpow(b, e / 2);
  res = (res * res) % m;
  if (e % 2 == 1) res = (res * b) % m;
  return res;
}

//Polynomial:
// x^8/2880-x^7/120+(119 x^6)/1440-(7 x^5)/16+(3829 x^4)/2880-(553 x^3)/240+(167 x^2)/80-(3 x)/4

//Computes the result of evaluation the polynomial in x = n
long long Calc(int n) {
  long long r = 0, N = 1;
  for (int i = 0; i < 9; ++i) {
    r = (r + coef[i] * N) % m;
    N = (N * n) % m;
  }
  return r;
}

int main() {
  //Compute the coefficients modulo m
  for (int i = 0; i < 9; ++i) coef[i] = (mpow(den[i], m - 2) * num[i]) % m;
  
  long long r = 0;
  
  //Read input and update result
  int n;
  while (cin >> n) r = (r + Calc(n)) % m;
  
  //Print result
  cout << r << endl;
}

