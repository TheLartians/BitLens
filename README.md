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

  // iterating over single bits should only be done as a last resort
  // as bitwise operations on the aligned data much more performant
  for (auto &v: bits.data()) {
    v &= 0b1101;
  }
}
```

## Integration

BitLens is a single header library the can be easily added via [CPM.cmake](https://github.com/TheLartians/CPM.cmake).

```cmake
CPMAddPackage(
  NAME BitLens
  GITHUB_REPOSITORY TheLartians/BitLens
  VERSION 2.0
)
```

Alternatively use git submodules, install globally, or simply download and copy the [header](include/bit_lens.h) into your project.

## Performance comparison to `vector<bool>`

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
bitwiseRandomAccessVectorBool    1031834 ns      1030598 ns          602
bitwiseRandomAccessVectorChar     801293 ns       800819 ns          786
bitwiseRandomAccessVectorInt      846211 ns       844902 ns          814
bitwiseDifferenceVectorBool       544263 ns       543967 ns         1384
bitwiseDifferenceVectorChar       153796 ns       153706 ns         4068
bitwiseDifferenceVectorInt        164300 ns       164038 ns         3995
bytewiseDifferenceVectorChar        8337 ns         8324 ns        76981
bytewiseDifferenceVectorInt          431 ns          430 ns      1602241
bytewiseDifferenceVectorSizeT        431 ns          430 ns      1604555
```

We can see that for integer containers even random access bitwise operations are slightly faster than `vector<bool>`, bitwise iteration is at least twice the speed, while bytewise operations outperform `vector<bool>` by orders of magnitude.
