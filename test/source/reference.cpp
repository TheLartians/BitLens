#include <bit_lens.h>
#include <doctest/doctest.h>

#include <bitset>

// an irregular test sequence
auto pattern = [](auto i) { return i % ((i / 3) % 7 + 1) == 0; };

TEST_CASE_TEMPLATE("bit reference", T, unsigned char, unsigned short, unsigned int, size_t) {
  SUBCASE("setting and reading bits") {
    bool setBits;

    SUBCASE("read bits") { setBits = false; }

    SUBCASE("set bits") { setBits = true; }

    CAPTURE(setBits);

    T value = 0;
    std::bitset<sizeof(T)> bits;
    for (size_t i = 0; i < bits.size(); ++i) {
      if (setBits) {
        bit_lens::BitReference reference(value, i);
        reference = pattern(i);
      } else {
        bits[i] = pattern(i);
      }
    }
    if (!setBits) {
      value = static_cast<T>(bits.to_ullong());
    }
    for (size_t i = 0; i < bits.size(); ++i) {
      bit_lens::BitReference reference(value, i);
      CHECK(reference == pattern(i));
    }
  }

  SUBCASE("constexpr") {
    constexpr T value = 0b110;
    static_assert(!bool(bit_lens::BitReference(value, 0)));
    static_assert(bool(bit_lens::BitReference(value, 1)));
    static_assert(bool(bit_lens::BitReference(value, 2)));
  }
}
