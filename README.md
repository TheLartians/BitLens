[![Actions Status](https://github.com/TheLartians/BitView/workflows/MacOS/badge.svg)](https://github.com/TheLartians/CPM.cmake/actions)
[![Actions Status](https://github.com/TheLartians/BitView/workflows/Windows/badge.svg)](https://github.com/TheLartians/CPM.cmake/actions)
[![Actions Status](https://github.com/TheLartians/BitView/workflows/Ubuntu/badge.svg)](https://github.com/TheLartians/CPM.cmake/actions)
[![Actions Status](https://github.com/TheLartians/BitView/workflows/Style/badge.svg)](https://github.com/TheLartians/CPM.cmake/actions)

# BitView

A C++17 bit view for vector types.

## Usage

```cpp
#include <bit_view.h>
#include <vector>

int main() {
  std::vector<char> container; // the container that stores the actual data
  bit_view::Container bits(container); // any integer type is supported
  bits.resizeToHold(10); // resize container to hold at least 10 bits
  bits.size(); // the number of bits that the container can hold
  bits.get(4); // get the ith bit
  bits.set(4, 1); // set the ith bit
  bits.forEach([](auto v, auto i){ ... }); // iterate over [value,index] pairs
}
```

## Integration

BitView is a single header library the can be easily added via [CPM.cmake](https://github.com/TheLartians/CPM.cmake).

```cmake
CPMAddPackage(
  NAME BitView
  GITHUB_REPOSITORY TheLartians/BitView
  VERSION 1.1.1
)
```

Alternatively use git submodules, install the library, or simply copy the header into your project. 

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
bitwiseRandomAccessVectorBool     810308 ns       809719 ns          796
bitwiseRandomAccessVectorChar     771897 ns       771729 ns          812
bitwiseRandomAccessVectorInt      808383 ns       806926 ns          849
bitwiseDifferenceVectorBool       504596 ns       503681 ns         1000
bitwiseDifferenceVectorChar       258595 ns       258122 ns         2538
bitwiseDifferenceVectorInt        163740 ns       163499 ns         5670
bytewiseDifferenceVectorChar        8504 ns         8490 ns        81615
bytewiseDifferenceVectorInt          459 ns          457 ns      1554581
bytewiseDifferenceVectorSizeT        442 ns          442 ns      1553743
```

We can see that while for bitwise random access there is no difference between `vector<bool>` and integer vector types, bitwise iteration is at least twice the speed, while bytewise operations outperform `vector<bool>` by an order of magnitude.
