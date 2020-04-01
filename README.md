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
Bitwise operations are performed as usual on the original container.
These operate on many bits in parallel and are optimized by the compiler.
`bit_lens::BitLens` provides a simple API for bit-specific operations and can be created with zero overhead whenever needed.

## API

```cpp
#include <bit_lens.h>
#include <vector>
#include <algorithm>

int main() {
  // create a bit container based on any another random-access container
  bit_lens::BitContainer<std::vector<unsigned>> bits;

  // or create a lens into an existing container
  // bit_lens::BitLens bits(container);

  // resize the container to store at least 10 bits
  bits.resize(10);

  // the actual number of bits that the container can store
  bits.size();

  // access individual bits using standard operators
  bits[8] = 1;
  
  // iterate over all bits using bit references
  for (auto bit: bits) { bit = 1; }

  // <algorithm> compatible iterators
  std::count(bits.begin(), bits.end(), true);

  // access the underlying data container
  bits.data();

  // if possible, use bitwise operators on values as it's much more performant (see benchmark)
  for (auto &v: bits.data()) { v &= 0b1101; }
}
```

## Integration

BitLens is a single header library that can be easily added via [CPM.cmake](https://github.com/TheLartians/CPM.cmake).

```cmake
CPMAddPackage(
  NAME BitLens
  GITHUB_REPOSITORY TheLartians/BitLens
  VERSION 2.1
)
```

Alternatively, use git submodules, install globally, or simply download and copy the [header](include/bit_lens.h) into your project.

## Benchmark

To illustrate the performance difference between `vector<bool>` and other container types, this repository contains a benchmark suite than  can be run through the following commands.

```bash
cmake -Hbenchmark -Bbuild/bench -DCMAKE_BUILD_TYPE=Release
cmake --build build/bench -j8
./build/bench/BitLensBenchmark
```

As an example, on a 2018 mac notebook, the benchmark produced the following output.

```
-------------------------------------------------------------------------
Benchmark                               Time             CPU   Iterations
-------------------------------------------------------------------------
bitwiseRandomAccessVectorBool     1080454 ns      1079235 ns          629
bitwiseRandomAccessVectorChar      852203 ns       851398 ns          797
bitwiseRandomAccessVectorInt       820476 ns       819897 ns          806
bitwiseDifferenceVectorBool        426308 ns       425972 ns         1331
bitwiseDifferenceVectorChar        159575 ns       159465 ns         4143
bitwiseDifferenceVectorInt         171368 ns       171338 ns         3874
valuewiseDifferenceVectorChar        8185 ns         8180 ns        80039
valuewiseDifferenceVectorInt          433 ns          433 ns      1560264
valuewiseDifferenceVectorSizeT        422 ns          422 ns      1636872
```

We can see that for integer containers, even bitwise random access operations are noticeably faster than `vector<bool>`.
Bitwise iteration using a `BitLens` has more than twice the speed, while doing bitwise operations on values (e.g. working on `BitLens::data()`) outperforms `vector<bool>` by many orders of magnitude.
This is because bitwise operators allow manipulating up to 64 bits at once (depending on the value type) and get an extra performance boost through vectorization.
Using BitLens we can choose the appropriate representation (value / bits) anytime, allowing us to achieve optimal storage and performance requirements.
