[![Actions Status](https://github.com/TheLartians/BitView/workflows/MacOS/badge.svg)](https://github.com/TheLartians/CPM.cmake/actions)
[![Actions Status](https://github.com/TheLartians/BitView/workflows/Windows/badge.svg)](https://github.com/TheLartians/CPM.cmake/actions)
[![Actions Status](https://github.com/TheLartians/BitView/workflows/Ubuntu/badge.svg)](https://github.com/TheLartians/CPM.cmake/actions)
[![Actions Status](https://github.com/TheLartians/BitView/workflows/Style/badge.svg)](https://github.com/TheLartians/CPM.cmake/actions)

# BitView

A simple container view for seamless swithing between integer and bit representations.
Ever wished you could have the convienience and space optimizations of `std::vector<bool>` without sacrificing performance and optimizations for bulk operations?
Then this is a library for you!

## Usage

Perform any bulk operations through bitwise operators on the original container.
Use `bit_view::Container` for everything else.

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
