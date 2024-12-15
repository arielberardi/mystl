#pragma once

#include <initializer_list>
#include <memory>

namespace MySTL {

template <typename T, typename Allocator = std::allocator<T>>
class Vector {
 public:
  using value_type = T;
  using allocator_type = std::allocator_traits<Allocator>::allocator_type;
  using size_type = std::allocator_traits<Allocator>::size_type;
  using pointer = std::allocator_traits<Allocator>::pointer;
  using const_pointer = std::allocator_traits<Allocator>::const_pointer;
  using reference = value_type&;
  using const_reference = const value_type&;

  static_assert(std::is_same<typename allocator_type::value_type, value_type>::value,
                "Container must be same value_type as its allocator");

  struct Iterator {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    explicit Iterator(pointer ptr) noexcept : m_ptr(ptr) {}

    reference operator*() const { return *m_ptr; }
    pointer operator->() const { return m_ptr; }

    Iterator& operator++() {
      ++m_ptr;
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    Iterator& operator--() {
      --m_ptr;
      return *this;
    }

    Iterator operator--(int) {
      Iterator tmp = *this;
      --(*this);
      return tmp;
    }

    Iterator operator+(difference_type n) const { return Iterator(m_ptr + n); }
    Iterator operator-(difference_type n) const { return Iterator(m_ptr - n); }
    difference_type operator-(const Iterator& other) const { return m_ptr - other.m_ptr; }

    Iterator& operator+=(difference_type n) {
      m_ptr += n;
      return *this;
    }

    Iterator& operator-=(difference_type n) {
      m_ptr -= n;
      return *this;
    }

    bool operator==(const Iterator& other) const { return m_ptr == other.m_ptr; }
    bool operator!=(const Iterator& other) const { return m_ptr != other.m_ptr; }
    bool operator<(const Iterator& other) const { return m_ptr < other.m_ptr; }
    bool operator<=(const Iterator& other) const { return m_ptr <= other.m_ptr; }
    bool operator>(const Iterator& other) const { return m_ptr > other.m_ptr; }
    bool operator>=(const Iterator& other) const { return m_ptr >= other.m_ptr; }

   private:
    pointer m_ptr;
  };

  struct ConstIterator {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;

    explicit ConstIterator(pointer ptr) noexcept : m_ptr(ptr) {}

    reference operator*() const { return *m_ptr; }
    pointer operator->() const { return m_ptr; }

    ConstIterator& operator++() {
      ++m_ptr;
      return *this;
    }

    ConstIterator operator++(int) {
      ConstIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    ConstIterator& operator--() {
      --m_ptr;
      return *this;
    }

    ConstIterator operator--(int) {
      ConstIterator tmp = *this;
      --(*this);
      return tmp;
    }

    ConstIterator operator+(difference_type n) const { return ConstIterator(m_ptr + n); }
    ConstIterator operator-(difference_type n) const { return ConstIterator(m_ptr - n); }
    difference_type operator-(const ConstIterator& other) const { return m_ptr - other.ptr; }

    ConstIterator& operator+=(difference_type n) {
      m_ptr += n;
      return *this;
    }

    ConstIterator& operator-=(difference_type n) {
      m_ptr -= n;
      return *this;
    }

    bool operator==(const ConstIterator& other) const { return m_ptr == other.m_ptr; }
    bool operator!=(const ConstIterator& other) const { return m_ptr != other.m_ptr; }
    bool operator<(const ConstIterator& other) const { return m_ptr < other.m_ptr; }
    bool operator<=(const ConstIterator& other) const { return m_ptr <= other.m_ptr; }
    bool operator>(const ConstIterator& other) const { return m_ptr > other.m_ptr; }
    bool operator>=(const ConstIterator& other) const { return m_ptr >= other.m_ptr; }

   private:
    pointer m_ptr;
  };

  explicit constexpr Vector(const Allocator& alloc = Allocator()) noexcept : m_alloc(alloc) {};

  explicit constexpr Vector(size_t count, const T& item,
                            const Allocator& alloc = Allocator()) noexcept
      : m_alloc(alloc) {
    reserve(count);

    for (size_t i{}; i < count; ++i) {
      push_back(item);
    }
  }

  constexpr Vector(const std::initializer_list<T>& items,
                   const Allocator& alloc = Allocator()) noexcept
      : m_alloc(alloc) {
    reserve(items.size());

    for (auto&& item : items) {
      push_back(item);
    }
  };

  constexpr Vector(const Vector& other, const Allocator& alloc = Allocator{}) noexcept
      : m_alloc(alloc) {
    reserve(other.size());

    for (size_t i{}; i < other.size(); ++i) {
      push_back(other[i]);
    }
  };

  constexpr Vector(Vector&& other, const Allocator& alloc = Allocator{}) noexcept : m_alloc(alloc) {
    reserve(other.m_capacity);
    m_size = other.m_size;
    m_data = other.m_data;

    other.clear();

    if (other.m_data) {
      std::allocator_traits<Allocator>::deallocate(other.m_alloc, other.m_data, other.m_capacity);
    }

    other.m_capacity = 0;
    other.m_size = 0;
    other.m_data = nullptr;
  };

  constexpr ~Vector() noexcept {
    clear();

    if (m_data) {
      std::allocator_traits<Allocator>::deallocate(m_alloc, m_data, m_capacity);
    }

    m_data = nullptr;
    m_capacity = 0;
    m_size = 0;
  };

  constexpr Vector& operator=(const Vector& other) noexcept {
    if (&other == this) {
      return *this;
    }

    reserve(other.size());

    for (size_t i{}; i < other.size(); ++i) {
      push_back(other[i]);
    }

    return *this;
  };

  constexpr Vector& operator=(Vector&& other) noexcept {
    if (&other == this) {
      return *this;
    }

    clear();

    reserve(other.m_capacity);
    m_size = other.m_size;
    m_data = other.m_data;

    other.clear();
    if (other.m_data) {
      std::allocator_traits<Allocator>::deallocate(other.m_alloc, other.m_data, other.m_capacity);
    }

    other.m_size = 0;
    other.m_capacity = 0;
    other.m_data = nullptr;
    return *this;
  };

  constexpr Vector& operator=(const std::initializer_list<T>& items) {
    clear();

    for (auto&& item : items) {
      push_back(item);
    }

    return *this;
  }

  constexpr void reserve(size_t capacity) noexcept {
    if (capacity <= m_capacity) {
      return;
    }

    pointer newData = std::allocator_traits<Allocator>::allocate(m_alloc, capacity);

    if (m_data) {
      for (size_t i{}; i < m_size; ++i) {
        std::allocator_traits<Allocator>::construct(m_alloc, newData + i, std::move(m_data[i]));
      }
    }

    if (m_data) {
      for (size_t i{}; i < m_size; i++) {
        std::allocator_traits<Allocator>::destroy(m_alloc, m_data + i);
      }

      std::allocator_traits<Allocator>::deallocate(m_alloc, m_data, m_capacity);
    }

    m_capacity = capacity;
    m_data = newData;
  }

  constexpr void resize(size_t size) noexcept {
    if (size == m_size) {
      return;
    }

    if (size > m_capacity) {
      reserve(m_capacity);

      for (size_t i{m_size}; i < size; ++i) {
        push_back(T());
      };
    } else {
      for (size_t i{m_size}; i > size; --i) {
        pop_back();
      }
    }

    m_size = size;
  }

  constexpr void shrink_to_fit() noexcept {
    if (m_size == m_capacity) {
      return;
    }

    pointer newData = std::allocator_traits<Allocator>::allocate(m_alloc, m_size);

    if (m_data) {
      for (size_t i{}; i < m_size; ++i) {
        std::allocator_traits<Allocator>::construct(m_alloc, newData + i, m_data[i]);
      }

      for (size_t i{}; i < m_size; ++i) {
        std::allocator_traits<Allocator>::destroy(m_alloc, m_data + i);
      }

      std::allocator_traits<Allocator>::deallocate(m_alloc, m_data, m_capacity);
    }

    m_capacity = m_size;
    m_data = newData;
  }

  constexpr void clear() noexcept {
    for (size_t i{}; i < m_size; ++i) {
      m_data[i].~T();
    }

    m_size = 0;
  }

  constexpr void insert(size_t pos, size_t count, const T& value) noexcept {
    if (count == 0 || pos > m_size || !m_data) {
      return;
    }

    size_t newCapacity = m_capacity;
    if (m_size + count >= m_capacity) {
      newCapacity = (m_size + count) * 2;
    }

    pointer newData = std::allocator_traits<Allocator>::allocate(m_alloc, newCapacity);

    for (size_t i{}; i < pos; i++) {
      std::allocator_traits<Allocator>::construct(m_alloc, newData + i, m_data[i]);
    }

    for (size_t i{}; i < count; i++) {
      std::allocator_traits<Allocator>::construct(m_alloc, newData + pos + i, value);
    }

    for (size_t i{pos}; i < m_size; i++) {
      std::allocator_traits<Allocator>::construct(m_alloc, newData + count + i, m_data[i]);
    }

    if (m_data) {
      for (size_t i{}; i < m_size; i++) {
        std::allocator_traits<Allocator>::destroy(m_alloc, m_data + i);
      }

      std::allocator_traits<Allocator>::deallocate(m_alloc, m_data, m_capacity);
    }

    m_capacity = newCapacity;
    m_size = m_size + count;
    m_data = newData;
  }

  constexpr void insert(size_t pos, const T& value) noexcept { insert(pos, 1, value); }

  constexpr void push_back(const T& item) noexcept {
    expandCapacity();

    std::allocator_traits<Allocator>::construct(m_alloc, m_data + m_size++, item);
  }

  constexpr void push_back(T&& item) noexcept {
    expandCapacity();

    std::allocator_traits<Allocator>::construct(m_alloc, m_data + m_size++, std::move(item));
  }

  constexpr void pop_back() noexcept {
    if (m_size == 0) {
      return;
    }

    std::allocator_traits<Allocator>::destroy(m_alloc, m_data + m_size--);
  }

  template <typename... Args>
  constexpr void emplace_back(Args&&... args) noexcept {
    expandCapacity();

    std::allocator_traits<Allocator>::construct(m_alloc, m_data + m_size++,
                                                std::forward<Args>(args)...);
  }

  template <typename... Args>
  constexpr void emplace(size_t pos, Args&&... args) noexcept {
    if (pos > m_size || !m_data) {
      return;
    }

    size_t newCapacity = m_capacity;
    size_t newSize = m_size + 1;

    if (newSize >= m_capacity) {
      newCapacity = m_capacity * 2;
    }

    pointer newData = std::allocator_traits<Allocator>::allocate(m_alloc, newCapacity);

    for (size_t i{}; i < pos; i++) {
      std::allocator_traits<Allocator>::construct(m_alloc, newData + i, m_data[i]);
    }

    std::allocator_traits<Allocator>::construct(m_alloc, newData + pos,
                                                std::forward<Args>(args)...);

    for (size_t i{pos}; i < m_size; i++) {
      std::allocator_traits<Allocator>::construct(m_alloc, newData + i + 1, m_data[i]);
    }

    if (m_data) {
      for (size_t i{}; i < m_size; i++) {
        std::allocator_traits<Allocator>::destroy(m_alloc, m_data + i);
      }

      std::allocator_traits<Allocator>::deallocate(m_alloc, m_data, m_capacity);
    }

    m_capacity = newCapacity;
    m_size = newSize;
    m_data = newData;
  }

  [[nodiscard]] constexpr reference front() const noexcept { return m_data[0]; }
  [[nodiscard]] constexpr reference back() const noexcept { return m_data[m_size - 1]; }

  [[nodiscard]] constexpr reference operator[](size_t index) noexcept { return m_data[index]; };
  [[nodiscard]] constexpr const_reference operator[](size_t index) const noexcept {
    return m_data[index];
  };

  [[nodiscard]] constexpr reference at(size_t index) {
    if (index >= m_size) {
      throw std::runtime_error("MySTL::Vector: Index out of bound");
    }

    return m_data[index];
  };

  [[nodiscard]] constexpr const_reference at(size_t index) const {
    if (index >= m_size) {
      throw std::runtime_error("MySTL::Vector: Index out of bound");
    }

    return m_data[index];
  };

  [[nodiscard]] constexpr bool empty() const noexcept { return m_size == 0; }
  [[nodiscard]] constexpr size_t capacity() const noexcept { return m_capacity; }
  [[nodiscard]] constexpr size_t size() const noexcept { return m_size; }

  [[nodiscard]] constexpr allocator_type get_allocator() const noexcept { return m_alloc; }

  Iterator begin() { return Iterator(&m_data[0]); }
  Iterator end() { return Iterator(&m_data[m_size]); }
  ConstIterator begin() const { return ConstIterator(&m_data[0]); }
  ConstIterator end() const { return ConstIterator(&m_data[m_size]); }
  ConstIterator cbegin() const { return ConstIterator(&m_data[0]); }
  ConstIterator cend() const { return ConstIterator(&m_data[m_size]); }

 private:
  void expandCapacity() noexcept {
    if (m_size >= m_capacity) {
      reserve(m_capacity == 0 ? 1 : m_capacity * 2);
    }
  };

  Allocator m_alloc;
  size_t m_capacity{};
  size_t m_size{};
  pointer m_data{nullptr};
};
}  // namespace MySTL
