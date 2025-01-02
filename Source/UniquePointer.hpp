#pragma once

#include <ostream>
#include <utility>

namespace MySTL {

template <typename Deleter>
concept DeleterRequirements =
    std::is_move_assignable_v<Deleter> && std::is_move_constructible_v<Deleter>;

template <typename T>
struct DefaultDeleter {
  constexpr DefaultDeleter() noexcept = default;

  constexpr void operator()(T* ptr) {
    if (ptr != nullptr) {
      delete ptr;
    }
  }
};

template <typename T, typename Deleter = DefaultDeleter<T>>
  requires DeleterRequirements<Deleter>
class UniquePointer {
 public:
  constexpr UniquePointer() noexcept : m_ptr(nullptr), m_deleter(Deleter()) {}
  explicit constexpr UniquePointer(T* ptr, Deleter deleter = Deleter()) noexcept
      : m_ptr(ptr), m_deleter(std::move(deleter)) {}

  ~UniquePointer() noexcept {
    if (m_ptr) {
      m_deleter(m_ptr);
    }
  }

  UniquePointer(const UniquePointer&) = delete;
  UniquePointer& operator=(const UniquePointer&) = delete;

  UniquePointer(UniquePointer&& other) noexcept
      : m_ptr(other.m_ptr), m_deleter(std::move(other.m_deleter)) {
    other.reset(nullptr);
  }

  [[nodiscard]] UniquePointer& operator=(UniquePointer&& other) noexcept {
    if (&other != this) {
      reset(nullptr);
      m_ptr = other.m_ptr;
      other.reset(nullptr);
    }

    return *this;
  }

  [[nodiscard]] constexpr T& operator*() const noexcept { return *m_ptr; }
  [[nodiscard]] constexpr T* operator->() const noexcept { return m_ptr; }
  [[nodiscard]] constexpr T* get() const noexcept { return m_ptr; }

  constexpr void reset(T* ptr) noexcept {
    if (m_ptr != ptr) {
      T* oldPtr = std::exchange(m_ptr, ptr);
      if (oldPtr) {
        m_deleter(oldPtr);
      }
    }
  }

  constexpr void reset(std::nullptr_t = nullptr) noexcept { reset(); }

  [[nodiscard]] constexpr T* release() { return std::exchange(m_ptr, nullptr); }

  constexpr void swap(UniquePointer& other) noexcept {
    std::swap(m_ptr, other.m_ptr);
    std::swap(m_deleter, other.m_deleter);
  }

  [[nodiscard]] constexpr operator bool() const noexcept { return m_ptr != nullptr; }

 private:
  T* m_ptr;
  Deleter m_deleter;
};

template <typename T, typename Deleter = DefaultDeleter<T>>
[[nodiscard]] constexpr bool operator==(const UniquePointer<T, Deleter>& u1,
                                        const UniquePointer<T, Deleter>& u2) noexcept {
  return u1.get() == u2.get();
}

template <typename T, typename Deleter = DefaultDeleter<T>>
[[nodiscard]] constexpr bool operator==(const UniquePointer<T, Deleter>& u1,
                                        const T* rawPtr) noexcept {
  return u1.get() == rawPtr;
}

template <typename T, typename Deleter = DefaultDeleter<T>>
[[nodiscard]] constexpr bool operator==(const UniquePointer<T, Deleter>& u1,
                                        std::nullptr_t) noexcept {
  return u1.get() == nullptr;
}

template <typename T, typename Deleter = DefaultDeleter<T>>
[[nodiscard]] constexpr bool operator!=(const UniquePointer<T, Deleter>& u1,
                                        const T* rawPtr) noexcept {
  return u1.get() != rawPtr;
}

template <typename T, typename Deleter = DefaultDeleter<T>>
[[nodiscard]] constexpr bool operator!=(const UniquePointer<T, Deleter>& u1,
                                        const UniquePointer<T, Deleter>& u2) noexcept {
  return u1.get() != u2.get();
}

template <typename T, typename Deleter = DefaultDeleter<T>>
[[nodiscard]] constexpr bool operator!=(const UniquePointer<T, Deleter>& u1,
                                        std::nullptr_t) noexcept {
  return u1.get() != nullptr;
}

template <typename T, typename Deleter = DefaultDeleter<T>>
[[nodiscard]] constexpr bool operator<(const UniquePointer<T, Deleter>& u1,
                                       const UniquePointer<T, Deleter>& u2) noexcept {
  return u1.get() < u2.get();
}

template <typename T, typename Deleter = DefaultDeleter<T>>
[[nodiscard]] constexpr bool operator>(const UniquePointer<T, Deleter>& u1,
                                       const UniquePointer<T, Deleter>& u2) noexcept {
  return u1.get() > u2.get();
}

template <typename T, typename Deleter = DefaultDeleter<T>>
[[nodiscard]] constexpr bool operator<=(const UniquePointer<T, Deleter>& u1,
                                        const UniquePointer<T, Deleter>& u2) noexcept {
  return (u1 < u2) || (u1 == u2);
}

template <typename T, typename Deleter = DefaultDeleter<T>>
[[nodiscard]] constexpr bool operator>=(const UniquePointer<T, Deleter>& u1,
                                        const UniquePointer<T, Deleter>& u2) noexcept {
  return (u1 > u2) || (u1 == u2);
}

template <typename T, typename Deleter = DefaultDeleter<T>>
inline std::ostream operator<<(std::ostream& os, const UniquePointer<T, Deleter>& p) noexcept {
  os << p.get();
  return os;
}

template <typename T, typename Deleter = DefaultDeleter<T>, typename... Args>
inline MySTL::UniquePointer<T, Deleter> make_unique(Args&&... args) noexcept {
  return MySTL::UniquePointer<T, Deleter>(new T(std::forward<Args>(args)...));
}

template <typename T, typename Deleter = DefaultDeleter<T>>
inline MySTL::UniquePointer<T, Deleter> make_unique_for_overwrite() noexcept {
  return MySTL::UniquePointer<T, Deleter>(new T);
}

template <typename T, typename Deleter = DefaultDeleter<T>>
inline void swap(MySTL::UniquePointer<T, Deleter>& p,
                 MySTL::UniquePointer<T, Deleter>& q) noexcept {
  p.swap(q);
}

}  // namespace MySTL
