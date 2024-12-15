#include <algorithm>
#include <iostream>

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

  int value;
};

int main() {
  MySTL::Vector<TestObject> v;
  v.reserve(4);
  v.emplace_back(1);
  v.emplace_back(2);
  v.emplace_back(3);
  v.emplace_back(4);

  std::for_each(v.begin(), v.end(), [](auto& t) { std::cout << t.value << std::endl; });

  std::find(v.begin(), v.end(), 2);

  for (auto& t : v) {
    std::cout << t.value << std::endl;
  }
}
