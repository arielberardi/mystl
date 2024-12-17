#include <algorithm>
#include <iostream>
#include <memory>

#include "UniquePointer.hpp"
#include "Vector.hpp"

struct TestObject {
  TestObject(int value) : value(value) { std::cout << "Constructor: " << value << std::endl; }
  ~TestObject() { std::cout << "Destructor: " << value << std::endl; }

  TestObject(const TestObject& other) {
    value = other.value;
    std::cout << "Copy Constructor: " << value << std::endl;
  }

  TestObject(TestObject&& other) {
    value = other.value;
    other.value = -1;
    std::cout << "Move Constructor: " << value << std::endl;
  }

  TestObject& operator=(const TestObject& other) {
    if (&other == this) {
      return *this;
    }

    value = other.value;
    std::cout << "Copy Assignment: " << value << std::endl;
    return *this;
  }

  TestObject& operator=(TestObject&& other) {
    if (&other == this) {
      return *this;
    }

    value = other.value;
    other.value = -1;
    std::cout << "Move Assignment: " << value << std::endl;
    return *this;
  }

  bool operator==(const TestObject& other) const { return value == other.value; }
  bool operator<(const TestObject& other) const { return value < other.value; }
  bool operator>(const TestObject& other) const { return value > other.value; }

  int value;
};

template <typename T>
struct CustomDeleter {
  constexpr void operator()(TestObject* ptr) noexcept {
    std::cout << "Custom Deleter" << std::endl;
    delete ptr;
  }
};

int main() {
  // MySTL::Vector<TestObject> v;
  // v.reserve(4);
  // v.emplace_back(3);
  // v.emplace_back(1);
  // v.emplace_back(4);
  // v.emplace_back(2);

  // std::for_each(v.begin(), v.end(), [](auto& t) { std::cout << t.value << std::endl; });

  // std::find(v.begin(), v.end(), 2);

  // std::sort(v.begin(), v.end());

  // for (auto& t : v) {
  //   std::cout << t.value << std::endl;
  // }

  auto v = MySTL::make_unique<TestObject, CustomDeleter<TestObject>>(1);
  auto b = MySTL::make_unique<TestObject, CustomDeleter<TestObject>>(2);

  std::cout << v->value << std::endl;
  std::cout << b->value << std::endl;

  v.swap(b);

  std::cout << v->value << std::endl;
  std::cout << b->value << std::endl;
}
