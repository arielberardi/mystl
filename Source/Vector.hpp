#pragma once

#include <initializer_list>
#include <memory>
#include <ostream>

// Implement custom Allocators
// Implement iterators for ranges

namespace MySTL {

template <typename T>
class Vector {
 public:
  Vector() = default;

  constexpr Vector(size_t count, const T& item) noexcept {
    reserve(count);

    for (size_t i{}; i < count; ++i) {
      push_back(item);
    }
  }

  constexpr Vector(const std::initializer_list<T>& items) noexcept {
    reserve(items.size());

    for (auto&& item : items) {
      push_back(item);
    }
  };

  constexpr ~Vector() noexcept {
    clear();

    if (m_data) {
      ::operator delete(m_data);
    }

    m_data = nullptr;
    m_capacity = 0;
    m_size = 0;
  };

  constexpr Vector(const Vector& other) noexcept {
    reserve(other.size());

    for (size_t i{}; i < other.size(); ++i) {
      push_back(other[i]);
    }
  };

  constexpr Vector(Vector&& other) noexcept {
    reserve(other.m_capacity);
    m_size = other.m_size;
    m_data = other.m_data;

    other.clear();
    other.m_capacity = 0;
    other.m_size = 0;

    if (other.m_data) {
      ::operator delete(other.m_data);
    }

    other.m_data = nullptr;
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
    other.m_size = 0;
    other.m_capacity = 0;

    if (other.m_data) {
      ::operator delete(other.m_data);
    }

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

    T* newData = static_cast<T*>(::operator new(capacity * sizeof(T)));

    if (m_data) {
      for (size_t i{}; i < m_size; ++i) {
        new (newData + i) T{std::move(m_data[i])};
      }
    }

    if (m_data) {
      for (size_t i{}; i < m_size; i++) {
        m_data[i].~T();
      }

      ::operator delete(m_data);
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

    T* newData = static_cast<T*>(::operator new(m_size * sizeof(T)));

    if (m_data) {
      for (size_t i{}; i < m_size; ++i) {
        new (newData + i) T{m_data[i]};
      }

      for (size_t i{}; i < m_size; ++i) {
        m_data[i].~T();
      }
    }

    if (m_data) {
      for (size_t i{}; i < m_size; i++) {
        m_data[i].~T();
      }

      ::operator delete(m_data);
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

    T* newData = static_cast<T*>(::operator new(newCapacity * sizeof(T)));

    for (size_t i{}; i < pos; i++) {
      new (newData + i) T{m_data[i]};
    }

    for (size_t i{}; i < count; i++) {
      new (newData + pos + i) T{value};
    }

    for (size_t i{pos}; i < m_size; i++) {
      new (newData + count + i) T{m_data[i]};
    }

    if (m_data) {
      for (size_t i{}; i < m_size; i++) {
        m_data[i].~T();
      }

      ::operator delete(m_data);
    }

    m_capacity = newCapacity;
    m_size = m_size + count;
    m_data = newData;
  }

  constexpr void insert(size_t pos, const T& value) noexcept { insert(pos, 1, value); }

  constexpr void push_back(const T& item) noexcept {
    expandCapacity();

    new (m_data + m_size++) T{item};
  }

  constexpr void push_back(T&& item) noexcept {
    expandCapacity();

    new (m_data + m_size++) T{std::move(item)};
  }

  constexpr void pop_back() noexcept {
    if (m_size == 0) {
      return;
    }

    m_data[m_size--].~T();
  }

  template <typename... Args>
  constexpr void emplace_back(Args&&... args) noexcept {
    expandCapacity();

    new (m_data + m_size++) T(std::forward<Args>(args)...);
  }

  template <typename... Args>
  constexpr void emplace(size_t pos, Args&&... args) noexcept {
    if (pos > m_size || !m_data) {
      return;
    }

    size_t newCapacity = m_capacity;
    if (m_size + 1 >= m_capacity) {
      newCapacity = m_capacity * 2;
    }

    T* newData = static_cast<T*>(::operator new(newCapacity * sizeof(T)));

    for (size_t i{}; i < pos; i++) {
      new (newData + i) T{m_data[i]};
    }

    new (newData + pos) T(std::forward<Args>(args)...);

    for (size_t i{pos}; i < m_size; i++) {
      new (newData + i + i) T{m_data[i]};
    }

    if (m_data) {
      for (size_t i{}; i < m_size; i++) {
        m_data[i].~T();
      }

      ::operator delete(m_data);
    }

    m_capacity = newCapacity;
    m_size = m_size + 1;
    m_data = newData;
  }

  [[nodiscard]] constexpr T& front() const noexcept { return m_data[0]; }
  [[nodiscard]] constexpr T& back() const noexcept { return m_data[m_size - 1]; }

  [[nodiscard]] constexpr T& operator[](size_t index) const noexcept { return m_data[index]; };
  [[nodiscard]] constexpr T& at(size_t index) const {
    if (index >= m_size) {
      throw std::runtime_error("MySTL::Vector: Index out of bound");
    }

    return m_data[index];
  };

  [[nodiscard]] constexpr bool empty() const noexcept { return m_size == 0; }
  [[nodiscard]] constexpr size_t capacity() const noexcept { return m_capacity; }
  [[nodiscard]] constexpr size_t size() const noexcept { return m_size; }

 private:
  void expandCapacity() {
    if (m_size >= m_capacity) {
      reserve(m_capacity == 0 ? 1 : m_capacity * 2);
    }
  };

  size_t m_capacity{};
  size_t m_size{};
  T* m_data{nullptr};
};

}  // namespace MySTL
