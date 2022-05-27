#include "dynBuffer.hpp"

#include <cassert>
#include <cstdint>
#include <exception>
#include <iostream>

int main() {

  uint64_t huge_allocation = 1e8;
  uint64_t sensible_allocation = 1e7;

  magpie::dynbuffer<double> a;

  assert(a.data() == nullptr);
  assert(a.size() == 0);

  // try huge allocation
  try {
    a = magpie::dynbuffer<double>(huge_allocation);
  } catch (std::exception const &e) {
    std::cout << e.what();
  }

  std::cout << "Now to allocate a sensible amount\n";
  auto b = magpie::dynbuffer<double>(sensible_allocation);

  auto ptr = b.data();
  auto sz = b.size();

  for (size_t i = 0; i < sz; ++i) {
    ptr[0] = 3.00 + i / 40000.0;
  }

  //  auto magpie::dynbuffer<double>
  return 0;
}