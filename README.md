[![Actions Status](https://github.com/TheLartians/BitView/workflows/MacOS/badge.svg)](https://github.com/TheLartians/CPM.cmake/actions)
[![Actions Status](https://github.com/TheLartians/BitView/workflows/Windows/badge.svg)](https://github.com/TheLartians/CPM.cmake/actions)
[![Actions Status](https://github.com/TheLartians/BitView/workflows/Ubuntu/badge.svg)](https://github.com/TheLartians/CPM.cmake/actions)
[![Actions Status](https://github.com/TheLartians/BitView/workflows/Style/badge.svg)](https://github.com/TheLartians/CPM.cmake/actions)

# BitView

A zero-overhead container view for seamless switching between integer and bit representations.
Ever wished you could have the convenience and compact storage of `std::vector<bool>` without all the [issues](http://www.gotw.ca/publications/N1211.pdf) and performance sacrifices?
Then this is a library for you!

## Usage

The actual data is stored in a standard integer container, such as `std::vector<int>`.
Biswise operations are performed as usual on the original container.
These operate on many bits in parallel and are optimized by the compiler.
`bit_view::Container` provides a simple API for bit-specific operations and can be created with zero overhead whenever needed.

## API

```cpp
#include <bit_view.h>
#include <vector>

int main() {
  std::vector<char> container; // the storage container with the actual data
  bit_view::Container bits(container); // create a bitwise view into the container
  bits.resizeToHold(10); // resize the container to store at least 10 bits
  bits.size(); // the actual number of bits that the container can store
  bits.get(8); // gets the ith bit
  bits.set(8, 1); // sets the ith bit
  bits.forEach([](auto value, auto index){ ... }); // iterate over all bits
}
```

## Integration

BitView is a single header library the can be easily added via [CPM.cmake](https://github.com/TheLartians/CPM.cmake).

```cmake
CPMAddPackage(
  NAME BitView
  GITHUB_REPOSITORY TheLartians/BitView
  VERSION 1.2
)
```

Alternatively use git submodules, install globally, or simply download and copy the [header](include/bit_view.h) into your project.

## Benchmark

To illustrate the performance difference between `vector<bool>` and other container types, this repository contains a benchmark suite than  can be run through the following commands.

```bash
cmake -Hbenchmark -Bbuild/bench -DCMAKE_BUILD_TYPE=Release
cmake --build build/bench -j8
./build/bench/BitViewBenchmark
```

As an example, a 2018 mac notebook, the benchmark produced the following output.

```
------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations
------------------------------------------------------------------------
bitwiseRandomAccessVectorBool    1042500 ns      1041887 ns          627
bitwiseRandomAccessVectorChar     861703 ns       857346 ns          815
bitwiseRandomAccessVectorInt      840315 ns       836922 ns          816
bitwiseDifferenceVectorBool       543340 ns       542516 ns         1194
bitwiseDifferenceVectorChar       138846 ns       138745 ns         4795
bitwiseDifferenceVectorInt        164660 ns       164364 ns         4790
bytewiseDifferenceVectorChar        8654 ns         8628 ns        81478
bytewiseDifferenceVectorInt          451 ns          450 ns      1543959
bytewiseDifferenceVectorSizeT        463 ns          460 ns      1563275
```

We can see that for integer containers even random access bitwise operations are slightly faster than `vector<bool>`, bitwise iteration is at least twice the speed, while bytewise operations outperform `vector<bool>` by an order of magnitude.
