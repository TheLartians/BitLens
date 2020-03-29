[![Actions Status](https://github.com/TheLartians/BitView/workflows/MacOS/badge.svg)](https://github.com/TheLartians/CPM.cmake/actions)
[![Actions Status](https://github.com/TheLartians/BitView/workflows/Windows/badge.svg)](https://github.com/TheLartians/CPM.cmake/actions)
[![Actions Status](https://github.com/TheLartians/BitView/workflows/Ubuntu/badge.svg)](https://github.com/TheLartians/CPM.cmake/actions)
[![Actions Status](https://github.com/TheLartians/BitView/workflows/Style/badge.svg)](https://github.com/TheLartians/CPM.cmake/actions)

# bit_view

A C++17 bit view for vector types.

## Usage

```cpp
std::vector<char> container; // any integer type is supported
bit_view::Container bits(container);
bits.resizeToHold(10); // resize container to hold at least 10 bits
bits.size() // the number of bits that the container can hold
bits.get(4); // get the ith bit
bits.set(4, 1); // set the ith bit
```
