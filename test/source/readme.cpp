#include <bit_lens.h>
#include <doctest/doctest.h>

#include <vector>

/**
 * A test case to see if the code from the readme compiles
 */

TEST_CASE_TEMPLATE("readme", T, unsigned char, unsigned short, unsigned int, size_t) {
  // clang-format off

  std::vector<unsigned char> container; // the storage container with the actual data
  bit_lens::BitLens bits(container); // create a bitwise lens into the container
  // bit_lens::BitContainer<std::vector<unsigned char>> // alternative: create an owning container
  bits.resize(10); // resize the container to store at least 10 bits
  bits.size(); // the actual number of bits that the container can store
  bits[8];// gets the ith bit
  bits[8] = 1; // sets the ith bit
  for (auto bit: bits) { bit = 1; } // iterate over all bits
  std::count(container.begin(), container.end(), true); // <algorithm> compatible iterators
  for (auto &v: bits.data()) { v &= 0b1101; } // access the underlying container to efficiently perform bitwise bulk operations

  // clang-format on
}
