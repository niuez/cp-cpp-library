#include <stack>

struct swag {

  using T = int;
  static T ide() {
    return 0;
  }
  static T ope(const T& a, const T& b) {
    return a + b;
  }

  std::stack<T> left_v, right_v, left_f, right_f;

  swag() {
    left_f.push(ide());
    right_f.push(ide());
  }

  void push(T val) {
    left_f.push(ope(left_f.top(), val));
    left_v.push(std::move(val));
  }

  T fold() {
    return ope(right_f.top(), left_f.top());
  }

  void pop() {
    if(right_f.size() == 1) {
      while(left_f.size() > 1) {
        right_f.push(ope(left_v.top(), right_f.top()));
        right_v.push(left_v.top());
        left_f.pop();
        left_v.pop();
      }
    }
    right_f.pop();
    right_v.pop();
  }
};
