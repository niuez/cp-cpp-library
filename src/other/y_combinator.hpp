#include <utility>
int main() {
  auto result = [g = [](auto f, int n) -> int {
    return n < 2 ? n : (f(f, n - 1) + f(f, n - 2));
  }](auto&&... args) {
    return g(g, std::forward<decltype(args)>(args)...);
  }(10);
}
