#include <cstdio>

namespace niu {
  char cur;
  struct FIN {
    static inline bool is_blank(char c) { return c <= ' '; }
    inline char next() { return cur = getc_unlocked(stdin); }
    inline char peek() { return cur; }
    inline void skip() { while(is_blank(next())){} }
    FIN& operator>>(int& n) {
      bool sign = 0;
      n = 0;
      skip();
      while(!is_blank(peek())) {
        if(peek() == '-') sign = 1;
        else n = (n << 1) + (n << 3) + (peek() & 0b1111);
        next();
      }
      if(sign) n = -n;
      return *this;
    }
  } fin;

  char tmp[128];
  struct FOUT {
    static inline bool is_blank(char c) { return c <= ' '; }
    inline void push(char c) { putc_unlocked(c, stdout); }
    FOUT& operator<<(char c) { push(c); return *this; }
    FOUT& operator<<(const char* s) { while(*s) push(*s++); return *this; }
    FOUT& operator<<(int n) {
      if(n) {
        char* p = tmp + 127; bool neg = 0;
        if(n < 0) neg = 1, n = -n;
        while(n) *--p = (n % 10) | 0b00110000, n /= 10;
        if(neg) *--p = '-';
        return (*this) << p;
      }
      else {
        push('0');
        return *this;
      }
    }
  } fout;
}
