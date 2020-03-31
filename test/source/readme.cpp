#include <doctest/doctest.h>

/**
 * A test case to see if the code from the readme compiles
 */

// clang-format off
#include <bit_lens.h>
#include <vector>
#include <algorithm>

void rd_main() {
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
// clang-format on

TEST_CASE_TEMPLATE("readme", T, unsigned char, unsigned short, unsigned int, size_t) { rd_main(); }
