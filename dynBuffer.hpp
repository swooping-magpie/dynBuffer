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

namespace {

template <class Allocator, bool = std::is_empty<Allocator>::value>
class potentiallyEmptyAllocator;

template <class Allocator> class potentiallyEmptyAllocator<Allocator, false> {
  Allocator allocator_ = Allocator();

public:
  potentiallyEmptyAllocator(){};
  potentiallyEmptyAllocator(potentiallyEmptyAllocator const &other) = delete;
  potentiallyEmptyAllocator(potentiallyEmptyAllocator &&other) noexcept
      : allocator_(other.allocator_){};

  auto operator=(potentiallyEmptyAllocator &&other) noexcept
      -> potentiallyEmptyAllocator & {
    std::swap(other.allocator_, allocator_);
    return *this;
  }
  auto operator=(potentiallyEmptyAllocator const &other) noexcept
      -> potentiallyEmptyAllocator & = delete;

  ~potentiallyEmptyAllocator() = default;

  auto allocator() noexcept -> Allocator & { return allocator_; }
};

template <class Allocator>
class potentiallyEmptyAllocator<Allocator, true> : protected Allocator {
public:
  potentiallyEmptyAllocator(){};
  auto allocator() noexcept -> Allocator & {
    return static_cast<Allocator &>(*this);
  }
};

} // namespace

template <typename T, typename Allocator = std::allocator<T>>
class dynbuffer : potentiallyEmptyAllocator<Allocator> {

public:
  // Types
  using value_type = T;
  using allocator_type = Allocator;

  using size_type = typename std::allocator_traits<allocator_type>::size_type;
  using difference_type =
      typename std::allocator_traits<allocator_type>::difference_type;
  using pointer = typename std::allocator_traits<allocator_type>::pointer;
  using const_pointer =
      typename std::allocator_traits<allocator_type>::const_pointer;

  using reference = value_type &;
  using const_reference = value_type const &;
  using iterator = pointer;
  using const_iterator = const_pointer;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // Destructor
  ~dynbuffer() noexcept;

  dynbuffer();

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
  size_type size_ = 0;
  pointer ptr_ = nullptr;
};

} // namespace magpie

template <typename T, class Alloc> magpie::dynbuffer<T, Alloc>::dynbuffer() {}

template <typename T, class Alloc>
magpie::dynbuffer<T, Alloc>::~dynbuffer() noexcept {
  this->allocator().deallocate(ptr_, size_);
}

template <typename T, class Alloc>
magpie::dynbuffer<T, Alloc>::dynbuffer(size_type n)
    : size_(n), ptr_(this->allocator().allocate(n)) {}

template <typename T, class Alloc>
magpie::dynbuffer<T, Alloc>::dynbuffer(dynbuffer &&b) noexcept
    : size_(b.size()), ptr_(b.ptr_) {}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::operator=(dynbuffer &&b) noexcept
    -> dynbuffer & {
  std::swap(b.ptr_, ptr_);
  std::swap(b.size_, size_);
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
  return ptr_[pos];
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::at(size_type pos) const -> const_reference {
  if (pos >= size()) {
    using namespace std::string_literals;
    throw std::out_of_range{
        "cannot access element at position "s + std::to_string(pos) +
        " from a dynbuffer with size " + std::to_string(size())};
  }
  return ptr_[pos];
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::operator[](size_type pos) -> reference {
  return ptr_[pos];
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::operator[](size_type pos) const
    -> const_reference {
  return ptr_[pos];
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::data() -> pointer {
  return ptr_;
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::data() const -> const_pointer {
  return ptr_;
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::empty() const -> bool {
  return size_ == 0;
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::size() const -> size_type {
  return size_;
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::begin() -> iterator {
  return ptr_;
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::begin() const -> const_iterator {
  return ptr_;
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::cbegin() const -> const_iterator {
  return ptr_;
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::end() -> iterator {
  return ptr_ + size();
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::end() const -> const_iterator {
  return ptr_ + size();
}

template <typename T, class Alloc>
auto magpie::dynbuffer<T, Alloc>::cend() const -> const_iterator {
  return ptr_ + size();
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