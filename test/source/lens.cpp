#include <bit_lens.h>
#include <doctest/doctest.h>
#include <easy_iterator.h>

#include <bitset>
#include <type_traits>
#include <vector>

// an irregular test sequence
auto pattern = [](auto i) { return i % ((i / 3) % 7 + 1) == 0; };

TEST_CASE_TEMPLATE("bit lens", T, unsigned char, unsigned short, unsigned int, size_t) {
  std::vector<T> container;
  bit_lens::BitLens bits(container);
  static_assert(std::is_same<typename decltype(bits)::Word, T>::value);
  constexpr auto wordSize = bit_lens::WordSize<T>::value;
  static_assert(wordSize == sizeof(T) * CHAR_BIT);
  static_assert(CHAR_BIT >= 8);  // assumed in tests

  SUBCASE("read bits") {
    container.resize(2);
    CHECK(bits.size() == container.size() * wordSize);

    std::bitset<wordSize> bitset;
    bitset[1] = 1;
    bitset[3] = 1;
    bitset[7] = 1;

    container[0] = static_cast<T>(bitset.to_ullong());
    for (size_t i = 0; i < wordSize; ++i) {
      CAPTURE(i);
      CHECK(bits[i] == bitset[i]);
    }

    bitset[2] = 1;
    bitset[3] = 0;
    bitset[6] = 1;
    // bitset[wordSize - 1] = 1;

    // we cant set high bits in the bitset, due to a strange limitation in
    // microsoft's STL that that throws an exception. high bits will be tested
    // in the "set bits" test later.

    container[1] = static_cast<T>(bitset.to_ullong());
    for (size_t i = 0; i < wordSize; ++i) {
      CAPTURE(i);
      CHECK(bits[wordSize + i] == bitset[i]);
    }
  }

  SUBCASE("resize") {
    bool fill;
    SUBCASE("fill with 0") { fill = false; }
    SUBCASE("fill with 1") { fill = true; }
    CAPTURE(fill);
    bits.resize(5 * wordSize - 3, fill);
    CHECK(bits.size() == 5 * wordSize);
    CHECK(container.size() == 5);
    for (size_t i = 0; i < bits.size(); ++i) {
      CAPTURE(i);
      CHECK(bits[i] == fill);
    }
  }

  SUBCASE("set bits") {
    bits.resize(5 * wordSize);
    for (size_t i = 0; i < bits.size(); ++i) {
      CAPTURE(i);
      CHECK(bits[i] == 0);
    }

    for (size_t i = 0; i < bits.size(); ++i) {
      CAPTURE(i);
      bits[i] = pattern(i);
    }
    for (size_t i = 0; i < bits.size(); ++i) {
      CAPTURE(i);
      CHECK(bits[i] == pattern(i));
    }
  }

  SUBCASE("iteration") {
    bits.resize(5 * wordSize);
    for (auto bit : bits) {
      CHECK(!bit);
    };
    for (auto [i, v] : easy_iterator::enumerate(bits)) {
      v = pattern(i);
    };
    for (auto [i, v] : easy_iterator::enumerate(bits)) {
      CAPTURE(i);
      CHECK(v == pattern(i));
    };
  }
}
