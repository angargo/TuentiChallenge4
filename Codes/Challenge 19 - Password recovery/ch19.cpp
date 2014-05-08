#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
using namespace std;

#define w uint8_t
#define d uint16_t
#define q putchar

uint8_t r[16], s[7], p;

uint16_t m[128] = {
  0x0090, 0x0391, 0x0392, 0x0393, 0x0394, 0x0395, 0x0396, 0x0397, 0x1832, 0x1f48,
  0x085f, 0x00f7, 0x1e0f, 0x06fe, 0x1d04, 0x0afd, 0x0b7d, 0x067e, 0x077f, 0x1f0d,
  0x054f, 0x0266, 0x00f3, 0x03f7, 0x08f1, 0x03f4, 0x080f, 0x0133, 0x0320, 0x1f80,
  0x0c4f, 0x2622, 0x1f23, 0x031f, 0x0d07, 0x0d16, 0x0d25, 0x0d34, 0x1f01, 0x048f,
  0x1f00, 0x0c8f, 0x2209, 0x1fdb, 0x0c0f, 0x2256, 0x1f12, 0x0c1f, 0x2256, 0x1fb5,
  0x0c2f, 0x2256, 0x1f14, 0x0c3f, 0x2256, 0x1f23, 0x0c4f, 0x2256, 0x1ff4, 0x0c5f,
  0x2256, 0x1f38, 0x0c6f, 0x2256, 0x1f68, 0x0c7f, 0x2256, 0x1f92, 0x08f9, 0x3f00,
  0x1fa9, 0x08f9, 0x3f00, 0x1fa7, 0x08f9, 0x3f00, 0x1fa8, 0x08f9, 0x3f00, 0x1fb4,
  0x08f9, 0x3f00, 0x1fe1, 0x08f9, 0x3f00, 0xffff, 0x193c, 0x1f6b, 0x08f9, 0x3f00,
  0x1f4e, 0x08f9, 0x3f00, 0x1f53, 0x08f9, 0x3f00, 0x1f52, 0x08f9, 0x3f00, 0x1f5b,
  0x08f9, 0x3f00, 0x1f1d, 0x08f9, 0x3f00, 0xffff
};

//Print the action performed in each state
void g() {
  uint16_t i;
  uint8_t w,x,y,z;
  int count = 0;
  while(1) {
    ++count;
    i=m[p];
    w=i>>12;
    x=i>>8&0xf;
    y=i>>4&0xf;
    z=i&0xf;
    if(w==0) {
      if(x==0) {
        cerr << "r[" << int(y) << "] = r[" << int(z) << "];" << endl;
        r[y]=r[z];
      }
      if(x==1) {
        cerr << "r[" << int(y) << "] = ~r[" << int(z) << "];" << endl;
        r[y]=~r[z];
      }
      if(x==2) {
        cerr << "r[" << int(y) << "] = -r[" << int(z) << "];" << endl;
        r[y]=-r[z];
      }
      if(x==3) {
        cerr << "r[" << int(y) << "] += r[" << int(z) << "];" << endl;
        r[y]+=r[z];
      }
      if(x==4) {
        cerr << "r[" << int(y) << "] -= r[" << int(z) << "];" << endl;
        r[y]-=r[z];
      }
      if(x==5) {
        cerr << "r[" << int(y) << "] *= r[" << int(z) << "];" << endl;
        r[y]*=r[z];
      }
        if(x==6) {
        cerr << "r[" << int(y) << "] &= r[" << int(z) << "];" << endl;
        r[y]&=r[z];
      }
      if(x==7) {
        cerr << "r[" << int(y) << "] |= r[" << int(z) << "];" << endl;
        r[y]|=r[z];
      }
      if(x==8) {
        cerr << "r[" << int(y) << "] ^= r[" << int(z) << "];" << endl;
        r[y]^=r[z];
      }
      if(x==9) {
        cerr << "r[" << int(y) << "] ^= r[" << int(z) << "];" << endl;
        r[y]^=r[z];
      }
      if(x==10) {
        cerr << "r[" << int(y) << "] <<= r[" << int(z) << "];" << endl;
        r[y]<<=r[z];
      }
      if(x==11) {
        cerr << "r[" << int(y) << "] >>= r[" << int(z) << "];" << endl;
        r[y]>>=r[z];
      }
      if(x==12) {
        cerr << "update s with r[" << int(y) << "] and r[" << int(z) << "]" << endl;
        s[0]=1;
        s[1]=r[y]==r[z];
        s[2]=r[y]!=r[z];
        s[3]=r[y]>r[z];
        s[4]=r[y]>=r[z];
        s[5]=r[y]<r[z];
        s[6]=r[y]<=r[z];
      }
      if(x==13) {
        cerr << "swap r[" << int(y) << "] y r[" << int(z) << "]" << endl;
        x=r[y];
        r[y]=r[z];
        r[z]=x;
      }
    }
    if(w==1) {
      cerr << "r[" << int(x) << "] = m[" << int(p) << "] = " << (0xff & m[p]) << endl;
      r[x]=m[p];
    }
    if(w==2 && s[x]) {
      cerr << "lolz " << int(x) << " " << count << " " << m[p] - 1 << " ";
      p=m[p]-1;
    }
    if(w==3) {
      cerr << "putchar " << int(x) << endl;
      putchar(r[x]);
    }
    if(w==15) {
      cerr << "fin" << endl;
      putchar(0xa);
      break;
    }
    p++;
  }
}

//The same function, but with more readable code
void g2() {
  p = 0;
  while (1) {
    if (p == 0) {
      r[9] = 0;
      for (int i = 0; i < 8; ++i) r[9] += r[i];
      r[8] = 50;
      p = 9;
    }
    else if (p == 9) {
      r[5] ^= 72;
      r[7] = (((r[7] >> 4) & 15) | ((r[7] & 15) << 4));
      r[4] *= 13;
      r[6] = -r[6];
      r[0] ^= (((r[3] + r[7]) ^ r[1]) + r[4]);
      r[3] = ~r[3];
      r[2] += r[0];
      if (r[4] > 128) r[1] += 35;
      p = 34;
    }
    else if (p == 34) {
      swap(r[0], r[7]);
      swap(r[1], r[6]);
      swap(r[2], r[5]);
      swap(r[3], r[4]);
      r[8] -= 1;
      if (r[8] != 0) p = 9; //40, 41, 42
      else if (r[0] != 219) p = 86; //43, 44, 45
      else if (r[1] != 18) p = 86; //46, 47, 48
      else if (r[2] != 181) p = 86; //49, 50, 51
      else if (r[3] != 20) p = 86; // 52, 53, 54
      else if (r[4] != 35) p = 86; //55, 56, 57
      else if (r[5] != 244) p = 86; //58, 59, 60
      else if (r[6] != 56) p = 86; //61, 62, 63
      else if (r[7] != 104) p = 86; //64, 65, 66
      else { //67 -> 85
        //The initial sum of the bytes must have a concrete value,
        //but there is no need to check that.
        cout << "Right!" << endl;
        return;
      }
    }
    else if (p == 86) { //86 -> 105
      cout << "Wrong!" << endl;
      return;
    }
  }
}

//Gets the password
void crack() {
  //Final values
  r[0] = 219;
  r[1] = 18;
  r[2] = 181;
  r[3] = 20;
  r[4] = 35;
  r[5] = 244;
  r[6] = 56;
  r[7] = 104;
  for (int i = 0; i < 50; ++i) {
    //Inverse operations
    swap(r[0], r[7]);
    swap(r[1], r[6]);
    swap(r[2], r[5]);
    swap(r[3], r[4]);
    if (r[4] > 128) r[1] -= 35;
    r[2] -= r[0];
    r[3] = ~r[3];
    r[0] ^= (((r[3] + r[7]) ^ r[1]) + r[4]);
    r[6] = -r[6];
    r[4] *= 197; //Inverse of 13 modulo 256
    r[7] = (((r[7] >> 4) & 15) | ((r[7] & 15) << 4));
    r[5] ^= 72;
  }
  //Initial values: the password
  for (int i = 0; i < 8; ++i) cout << hex << setw(2) << setfill('0') << int(r[i]);
  cout << endl;
}

int main(int argc, char* argv[]) {
  uint32_t h,l;
  crack();
}
