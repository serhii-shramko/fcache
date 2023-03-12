fcache
======
C++ function wrapper that cache result
## Example
```cpp
#include <iostream>
#include "cached_function.hpp"

// Ordinary recursive fibonacci implementation.
int fibonacci(int n) {
  if (n <= 1)
    return n;
  return fibonacci(n - 1) + fibonacci(n - 2);
}

// Cached wrapper of function.
// Attention: when call fibonacci_wrapper(n), only n argument
// will be cached. Call inside fibonacci will not be cached.
fcache::cached_function<int(int)> fibonacci_wrapper = fibonacci;

// If function call recursive use next lambda implementation.
fcache::cached_function<int(int)> cached_fibonacci = [](int n) {
  if (n <= 1)
    return n;
  return cached_fibonacci(n - 1) + cached_fibonacci(n - 2);
};

int main() {
  std::cout << "Recursive fibonacci started\n";
  std::cout << "fibonacci(42): " << fibonacci(42) << "\n";
  std::cout << "Recursive fibonacci ended\n\n";

  std::cout << "Recursive fibonacci_wrapper started\n";
  for (int i = 1; i <= 42; ++i)
    std::cout << "fibonacci_wrapper(" << i << "): " << fibonacci_wrapper(i)
              << "\n";
  std::cout << "Recursive fibonacci_wrapper ended\n\n";

  std::cout << "Recursive cached_fibonacci started\n";
  std::cout << "cached_fibonacci(42): " << cached_fibonacci(42) << "\n";
  std::cout << "Recursive cached_fibonacci ended\n\n";
}
```