#  MY STL

### References


## Setup & Run

Requirement: cmake and some c++ compiler (gcc/clang, etc)

1) From root folder run: cmake -S . -B build -DCMAKE_BUILD_TYPE=Release (or Debug)
2) Then ./build/MySTL 

## Benchmarks Results

MySTL Vector **70.007M/s** vs STD Vector **34.825M/s**
MySTL Vector with allocator **45.007M/s** vs STD Vector **37.825M/s**

## Contact

For any questions or suggestions, please reach out [Ariel Berardi](https://www.linkedin.com/in/aberardi95/).
