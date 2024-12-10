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

  int value;
};

int main() { MySTL::Vector<TestObject> v{}; }
