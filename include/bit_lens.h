#pragma once

#include <climits>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>

namespace bit_lens {

  template <class Word> struct WordSize {
    static constexpr size_t value = sizeof(Word) * CHAR_BIT;
  };

  /*
   * A reference for a single bit
   */
  template <class Word> class BitReference {
  private:
    size_t offset;
    Word &value;

  public:
    constexpr BitReference(Word &v, size_t o) noexcept : offset(o), value(v) {
      static_assert(std::is_integral<Word>::value && std::is_unsigned<Word>::value,
                    "only unsigned integer value types are supported");
    }

    constexpr operator bool() const noexcept { return (value >> offset) & Word(1); }

    BitReference &operator=(bool v) noexcept {
      value = (value & ~(Word(1) << offset)) | (Word(v) << offset);
      return *this;
    }

    BitReference &operator=(const BitReference &v) noexcept { return *this = bool(v); }
  };

  /**
   * Swaps only the referenced bit
   */
  template <class Word> void swap(BitReference<Word> &a, BitReference<Word> &b) {
    bool tmp = a;
    a = b;
    b = tmp;
  }

  template <class ContainerIterator> class BitIterator {
  private:
    using Word = typename std::decay<decltype(*std::declval<ContainerIterator>())>::type;
    static constexpr auto WORD_SIZE = WordSize<Word>::value;
    ContainerIterator iterator;
    size_t index;

  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = BitReference<Word>;
    using difference_type = size_t;
    using reference = value_type;

    constexpr BitIterator(ContainerIterator i, size_t o) noexcept : iterator(i), index(o) {}

    BitIterator &operator++(int) noexcept {
      BitIterator copy(*this);
      index++;
      return copy;
    }

    BitIterator operator++() noexcept {
      ++index;
      return *this;
    }

    BitIterator &operator--(int) noexcept {
      BitIterator copy(*this);
      index--;
      return copy;
    }

    BitIterator operator--() noexcept {
      --index;
      return *this;
    }

    BitIterator operator+(int n) const noexcept { return BitIterator(iterator, index + n); }
    BitIterator operator-(int n) const noexcept { return BitIterator(iterator, index - n); }

    size_t operator-(const BitIterator &other) const noexcept { return index - other.index; }

    constexpr bool operator!=(const BitIterator &other) const {
      return iterator != other.iterator || index != other.index;
    }

    constexpr bool operator==(const BitIterator &other) const {
      return iterator == other.iterator && index == other.index;
    }

    constexpr BitReference<Word> operator*() const noexcept {
      return BitReference<Word>(*(iterator + index / WORD_SIZE), index % WORD_SIZE);
    }
  };

  template <class BitContainer> class BitLens {
  private:
    BitContainer &container;

  public:
    using Word = typename std::remove_reference<decltype(std::declval<BitContainer>()[0])>::type;
    static constexpr auto WORD_SIZE = WordSize<Word>::value;

    constexpr BitLens(BitContainer &c) noexcept : container(c) {}

    /**
     * returns the number of bits that fit in the container
     */
    size_t size() noexcept(noexcept(container.size())) { return container.size() * WORD_SIZE; }

    /**
     * resizes the container to hold at least `N` bits.
     * Note that the actual size will be a multiple of the bits in a word.
     */
    void resize(size_t N, bool v = false) noexcept(noexcept(container.resize(0, 0))) {
      Word offset = N % WORD_SIZE == 0 ? 0 : 1;
      container.resize(N / WORD_SIZE + offset, v ? ~0 : 0);
    }

    [[deprecated("legacy API: use `resize(i,v)` insted")]] void resizeToHold(size_t N,
                                                                             bool v = false) {
      Word offset = N % WORD_SIZE == 0 ? 0 : 1;
      container.resize(N / WORD_SIZE + offset, v ? ~0 : 0);
    }

    /**
     * returns a BitReference to the bit at position `i`.
     */
    constexpr BitReference<Word> operator[](size_t i) noexcept(noexcept(container[0])) {
      Word offset = i % WORD_SIZE;
      return BitReference<Word>(container[i / WORD_SIZE], offset);
    }

    /**
     * returns a BitReference to the bit at position `i`.
     */
    constexpr BitReference<const Word> operator[](size_t i) const noexcept(noexcept(container[0])) {
      Word offset = i % WORD_SIZE;
      return BitReference<const Word>(container[i / WORD_SIZE], offset);
    }

    [[deprecated("legacy API: use `[i]` insted")]] bool get(size_t i) const
        noexcept(noexcept(std::declval<BitLens<BitContainer>>()[i])) {
      return (*this)[i];
    }

    [[deprecated("legacy API: use `[i] = v` insted")]] void set(size_t i, bool v) noexcept(
        noexcept(std::declval<BitLens<BitContainer>>()[i])) {
      (*this)[i] = v;
    }

    constexpr auto begin() noexcept(noexcept(container.begin())) {
      return BitIterator(container.begin(), 0);
    }

    constexpr auto end() noexcept(noexcept(container.begin())) {
      return BitIterator(container.begin(), size());
    }

    constexpr auto begin() const noexcept(noexcept(container.begin())) {
      return BitIterator(container.begin(), 0);
    }

    constexpr auto end() const noexcept(noexcept(container.begin())) {
      return BitIterator(container.begin(), size());
    }

    //  [[deprecated("legacy API: use iterators instead")]]
    template <class F> void forEach(F &&f) {
      size_t N = size();
      for (size_t idx = 0; idx != N; ++idx) {
        f((*this)[idx], idx);
      }
    }
  };

  /**
   * Legacy API
   * @deprecated Use BitLens instead
   */
  template <class BitContainer> using Lens = BitLens<BitContainer>;

  /**
   * creates a bitlens that owns its data
   */
  template <class C> class BitContainer : public BitLens<C> {
  private:
    C dataContainer;

  public:
    constexpr BitContainer() noexcept(noexcept(C())) : BitLens<C>(dataContainer) {}
    constexpr BitContainer(const BitContainer &other) noexcept(
        noexcept(C(std::declval<const C &>())))
        : dataContainer(other.data()), BitLens<C>(dataContainer) {}
    constexpr BitContainer(BitContainer &&other) noexcept(noexcept(C(std::declval<C &&>())))
        : dataContainer(std::move(other.data())), BitLens<C>(dataContainer) {}

    template <typename... Args> BitContainer(Args &&... args)
        : dataContainer(std::forward<Args...>(args...)), BitLens<C>(dataContainer) {}

    /**
     *  get the underlying data container
     */
    C &data() noexcept { return dataContainer; }

    /**
     *  get the underlying data container
     */
    const C &data() const noexcept { return dataContainer; }
  };

}  // namespace bit_lens

namespace std {
  template <class T> struct iterator_traits<bit_lens::BitIterator<T>>
      : public bit_lens::BitIterator<T> {};
}  // namespace std
