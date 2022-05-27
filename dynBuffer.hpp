//
//
//
// An implementation of my dynBuffer/ dynarray
//
//
//

#ifndef MAGPIE_DYNBUFFER
#define MAGPIE_DYNBUFFER

// definition
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>

// declaration
#include <algorithm>

namespace magpie {
template <typename T, typename Allocator = std::allocator<T>> class dynbuffer {

public:
  // Types
  using value_type = T;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type &;
  using const_reference = value_type const &;
  using pointer = typename std::allocator_traits<T>::pointer;
  using const_pointer = typename std::allocator_traits<T>::const_pointer;
  using iterator = pointer;
  using const_iterator = const_pointer;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // Destructor
  ~dynbuffer() noexcept;

  // Constructors
  // Non init constructor
  explicit dynbuffer(size_type n);

  // Copy constructor
  dynbuffer(dynbuffer const &buf) = delete;

  // Move constructor
  dynbuffer(dynbuffer &&buf) noexcept;

  // Assignment Operator

  // Copy assignment
  auto operator=(dynbuffer const &buf) -> dynbuffer & = delete;

  // Move assignment
  auto operator=(dynbuffer &&buf) noexcept -> dynbuffer &;

  // ACCESS
  auto at(size_type pos) -> reference;
  auto at(size_type pos) const -> const_reference;

  auto operator[](size_type pos) -> reference;
  auto operator[](size_type pos) const -> const_reference;

  auto data() -> pointer;
  auto data() const -> const_pointer;

  // SIZE
  auto empty() const -> bool;
  auto size() const -> size_type;

  // Iterators
  auto begin() -> iterator;
  auto begin() const -> const_iterator;
  auto cbegin() const -> const_iterator;

  auto end() -> iterator;
  auto end() const -> const_iterator;
  auto cend() const -> const_iterator;

  auto rbegin() -> reverse_iterator;
  auto rbegin() const -> const_reverse_iterator;
  auto crbegin() const -> const_reverse_iterator;

  auto rend() -> reverse_iterator;
  auto rend() const -> const_reverse_iterator;
  auto crend() const -> const_reverse_iterator;

private:
  size_type _size;
  pointer _ptr;
};

} // namespace magpie

template <typename T, class Alloc>
magpie::dynbuffer<T, Alloc>::dynbuffer(size_type n)
    : _size(n), _ptr(Alloc::allocate(n)) {}

template <typename T, class Alloc>
magpie::dynbuffer<T, Alloc>::dynbuffer(dynbuffer &&b) noexcept
    : _size(b.size()), _ptr(b._ptr) {}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::operator=(dynbuffer &&b) noexcept
    -> dynbuffer & {
  std::swap(b._ptr, _ptr);
  std::swap(b._size, _size);
  return *this;
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::at(size_type pos) -> reference {
  if (pos >= size()) {
    using namespace std::string_literals;
    throw std::out_of_range{
        "cannot access element at position "s + std::to_string(pos) +
        " from a dynbuffer with size " + std::to_string(size())};
  }
  return _ptr[pos];
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::at(size_type pos) const -> const_reference {
  if (pos >= size()) {
    using namespace std::string_literals;
    throw std::out_of_range{
        "cannot access element at position "s + std::to_string(pos) +
        " from a dynbuffer with size " + std::to_string(size())};
  }
  return _ptr[pos];
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::operator[](size_type pos) -> reference {
  return _ptr[pos];
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::operator[](size_type pos) const
    -> const_reference {
  return _ptr[pos];
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::data() -> pointer {
  return _ptr;
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::data() const -> const_pointer {
  return _ptr;
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::empty() const -> bool {
  return _size == 0;
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::size() const -> size_type {
  return _size;
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::begin() -> iterator {
  return _ptr;
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::begin() const -> const_iterator {
  return _ptr;
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::cbegin() const -> const_iterator {
  return _ptr;
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::end() -> iterator {
  return _ptr + size();
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::end() const -> const_iterator {
  return _ptr + size();
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::cend() const -> const_iterator {
  return _ptr + size();
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::rbegin() -> reverse_iterator {
  return reverse_iterator{end()};
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::rbegin() const -> const_reverse_iterator {
  return reverse_iterator{end()};
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::crbegin() const -> const_reverse_iterator {
  return reverse_iterator{cend()};
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::rend() -> reverse_iterator {
  return reverse_iterator{begin()};
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::rend() const -> const_reverse_iterator {
  return reverse_iterator{begin()};
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::crend() const -> const_reverse_iterator {
  return reverse_iterator{cbegin()};
}

#endif