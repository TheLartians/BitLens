#include <doctest/doctest.h>

/**
 * A test case to see if the code from the readme compiles
 */

// clang-format off
#include <bit_lens.h>
#include <vector>
#include <algorithm>

void rd_main() {
  bit_lens::BitContainer<std::vector<unsigned>> bits; // create a bit container based on vector<unsigned>
  // bit_lens::BitLens bits(container); // or create a lens into an existing container
  bits.resize(10); // resize the container to store at least 10 bits
  bits.size(); // the actual number of bits that the container can store
  bits[8];// gets the 8th bit
  bits[8] = 1; // sets the 8th bit
  for (auto bit: bits) { bit = 1; } // iterate over all bits
  std::count(bits.begin(), bits.end(), true); // <algorithm> compatible iterators
  for (auto &v: bits.data()) { // access the underlying container
    v &= 0b1101; // perform bitwise bulk operations for best performance (see benchmark)
  }
}
// clang-format on

TEST_CASE_TEMPLATE("readme", T, unsigned char, unsigned short, unsigned int, size_t) { rd_main(); }
