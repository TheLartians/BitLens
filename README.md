[![Actions Status](https://github.com/TheLartians/BitLens/workflows/MacOS/badge.svg)](https://github.com/TheLartians/BitLens/actions)
[![Actions Status](https://github.com/TheLartians/BitLens/workflows/Windows/badge.svg)](https://github.com/TheLartians/BitLens/actions)
[![Actions Status](https://github.com/TheLartians/BitLens/workflows/Ubuntu/badge.svg)](https://github.com/TheLartians/BitLens/actions)
[![Actions Status](https://github.com/TheLartians/BitLens/workflows/Style/badge.svg)](https://github.com/TheLartians/BitLens/actions)

# BitLens

A zero-overhead container wrapper for seamless switching between integer and bit representations.
Ever wished you could have the convenience and compact storage of `std::vector<bool>` without all the [issues](http://www.gotw.ca/publications/N1211.pdf) and performance sacrifices?
Then this is a library for you!

## Usage

The actual data is stored in an unsigned integral container, such as `std::vector<unsigned int>`.
Biswise operations are performed as usual on the original container.
These operate on many bits in parallel and are optimized by the compiler.
`bit_lens::Lens` provides a simple API for bit-specific operations and can be created with zero overhead whenever needed.

## API

```cpp
#include <bit_lens.h>
#include <vector>

int main() {
  std::vector<unsigned char> container; // the storage container with the actual data
  bit_lens::Lens bits(container); // create a bitwise lens into the container
  bits.resizeToHold(10); // resize the container to store at least 10 bits
  bits.size(); // the actual number of bits that the container can store
  bits.get(8); // gets the ith bit
  bits.set(8, 1); // sets the ith bit
  bits.forEach([](auto value, auto index){ ... }); // iterate over all bits
}
```

## Integration

BitLens is a single header library the can be easily added via [CPM.cmake](https://github.com/TheLartians/CPM.cmake).

```cmake
CPMAddPackage(
  NAME BitLens
  GITHUB_REPOSITORY TheLartians/BitLens
  VERSION 1.4
)
```

Alternatively use git submodules, install globally, or simply download and copy the [header](include/bit_lens.h) into your project.

## Benchmark

To illustrate the performance difference between `vector<bool>` and other container types, this repository contains a benchmark suite than  can be run through the following commands.

```bash
cmake -Hbenchmark -Bbuild/bench -DCMAKE_BUILD_TYPE=Release
cmake --build build/bench -j8
./build/bench/BitLensBenchmark
```

As an example, on a 2018 mac notebook, the benchmark produced the following output.

```
------------------------------------------------------------------------
Benchmark                              Time             CPU   Iterations
------------------------------------------------------------------------
bitwiseRandomAccessVectorBool    1110850 ns      1106066 ns          625
bitwiseRandomAccessVectorChar     829747 ns       828204 ns          788
bitwiseRandomAccessVectorInt      815876 ns       814413 ns          791
bitwiseDifferenceVectorBool       517632 ns       515011 ns         1000
bitwiseDifferenceVectorChar       152281 ns       152097 ns         4171
bitwiseDifferenceVectorInt        161656 ns       161367 ns         4278
bytewiseDifferenceVectorChar        8435 ns         8405 ns        81414
bytewiseDifferenceVectorInt          438 ns          437 ns      1563446
bytewiseDifferenceVectorSizeT        423 ns          423 ns      1598141
```

We can see that for integer containers even random access bitwise operations are slightly faster than `vector<bool>`, bitwise iteration is at least twice the speed, while bytewise operations outperform `vector<bool>` by orders of magnitude.
