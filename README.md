[![Actions Status](https://github.com/TheLartians/BitView/workflows/MacOS/badge.svg)](https://github.com/TheLartians/CPM.cmake/actions)
[![Actions Status](https://github.com/TheLartians/BitView/workflows/Windows/badge.svg)](https://github.com/TheLartians/CPM.cmake/actions)
[![Actions Status](https://github.com/TheLartians/BitView/workflows/Ubuntu/badge.svg)](https://github.com/TheLartians/CPM.cmake/actions)
[![Actions Status](https://github.com/TheLartians/BitView/workflows/Style/badge.svg)](https://github.com/TheLartians/CPM.cmake/actions)

# BitView

A zero-overhead container view for seamless switching between integer and bit representations.
Ever wished you could have the convenience and compact storage of `std::vector<bool>` without all the [issues](http://www.gotw.ca/publications/N1211.pdf) and performance sacrifices?
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
  VERSION 1.1.1
)
```

Alternatively use git submodules, install globally, or simply download and copy the [header](include/bit_view.h) into your project. 
