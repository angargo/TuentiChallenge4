#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
  //Read the numbers from site
  system("php-5.5.11-nts-Win32-VC11-x86\\php ch18.php < ch18.in > ch18.nums");
  
  //Computes the discrete logarithm
  system("py ch18.py < ch18.nums");
}