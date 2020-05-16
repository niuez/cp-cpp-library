#include <unistd.h>

namespace niu {

  struct fastin {
    static const int bufsize = 1 << 24;
    char buf[bufsize];
    char* iter;
    fastin() {
      iter = buf;
      for(int t = 0, k; (k = read(STDIN_FILENO, buf + t, sizeof(buf)) - t) > 0; t += k);
    }
    fastin& operator>>(int& num) {
      num = 0;
      bool neg = false;
      while(*iter < '+') iter++;
      if(*iter == '-') { neg = true; iter++; }
      else if(*iter == '+') iter++;
      while(*iter >= '0') num = 10 * num + *(iter++) - '0';
      if(neg) num = -num;
      return *this;
    } 
  } fin;
  struct fastout {
    static const int bufsize = 1 << 24;
    char buf[bufsize];
    char* iter;
    fastout() {
      iter = buf;
    }
    ~fastout() {
      for(int t = 0, k; (k = write(STDOUT_FILENO, buf + t, iter - buf - t)) > 0; t += k);
    }
    fastout& operator<<(int num) {
      static char tmp[20];
      if(num == 0) {
        *(iter++) = '0';
        return *this;
      }
      if(num < 0) {
        *(iter++) = '-';
        num = -num;
      }
      int i = 0;
      while(num) {
        tmp[i++] = num % 10;
        num /= 10;
      }
      while(i--) {
        *(iter++) = tmp[i] + '0';
      }
      return *this;
    }
    fastout& operator<<(char c) {
      *(iter++) = c;
      return *this;
    }
  } fout;
}
