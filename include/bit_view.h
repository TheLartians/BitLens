#pragma once

#include <climits>
#include <cstddef>
#include <type_traits>

namespace bit_view {

const size_t BITS_IN_BYTE = CHAR_BIT;

template <class C> class Container {
protected:
  C &container;

public:
  using Word = typename C::value_type;
  static const size_t WORD_SIZE = sizeof(Word) * BITS_IN_BYTE;

  Container(C &c) : container(c) {
    static_assert(std::is_integral<Word>::value &&
                      std::is_unsigned<Word>::value,
                  "only unsigned integer value types are supported");
  }

  /**
   * returns the number of bits in the container
   */
  size_t size() { return container.size() * WORD_SIZE; }

  /**
   * resizes the container to hold at least `N` bits.
   */
  void resizeToHold(size_t N, bool v = false) {
    Word offset = N % WORD_SIZE == 0 ? 0 : 1;
    container.resize(N / WORD_SIZE + offset, v ? ~0 : 0);
  }

  /**
   * returns the value of the `i`th bit.
   */
  bool get(size_t i) const {
    Word offset = i % WORD_SIZE;
    return (container[i / WORD_SIZE] >> offset) & Word(1);
  }

  /**
   * same as `get(i)`.
   */
  bool operator[](size_t i) const { return get(i); }

  /**
   * sets the value of the `i`th bit
   */
  void set(size_t i, bool v) {
    Word offset = i % WORD_SIZE;
    const size_t index = i / WORD_SIZE;
    container[index] =
        (container[index] & ~(Word(1) << offset)) | (Word(v) << offset);
  }

  /**
   * iterate over `[value,index]` pairs
   */
  template <class F> void forEach(F &&f) {
    size_t N = size();
    for (size_t idx = 0; idx != N; ++idx) {
      f(get(idx), idx);
    }
  }
};

} // namespace bit_view
