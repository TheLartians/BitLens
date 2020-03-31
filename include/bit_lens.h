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
template <class Word> class Reference {
private:
  size_t offset;
  Word &value;

public:
  constexpr Reference(Word &v, size_t o) noexcept : offset(o), value(v) {
    static_assert(std::is_integral<Word>::value &&
                      std::is_unsigned<Word>::value,
                  "only unsigned integer value types are supported");
  }

  constexpr operator bool() const noexcept {
    return (value >> offset) & Word(1);
  }

  Reference &operator=(bool v) noexcept {
    value = (value & ~(Word(1) << offset)) | (Word(v) << offset);
    return *this;
  }

  Reference &operator=(const Reference &v) noexcept { return *this = bool(v); }
};

/**
 * Swaps only the referenced bit
 */
template <class Word> void swap(Reference<Word> &a, Reference<Word> &b) {
  bool tmp = a;
  a = b;
  b = tmp;
}

template <class Iterator> struct IteratorWord {
  using type = typename std::decay<decltype(*std::declval<Iterator>())>::type;
};

template <class ContainerIterator>
class Iterator
    : public std::iterator<
          std::random_access_iterator_tag,
          Reference<typename IteratorWord<ContainerIterator>::type>, size_t> {
private:
  using Word = typename IteratorWord<ContainerIterator>::type;
  static constexpr auto WORD_SIZE = WordSize<Word>::value;
  ContainerIterator iterator;
  size_t index;

public:
  constexpr Iterator(ContainerIterator i, size_t o) noexcept
      : iterator(i), index(o) {}

  Iterator operator++() noexcept {
    ++index;
    return Iterator(iterator, index + 1);
  }

  Iterator &operator++(int n) noexcept {
    index++(n);
    return *this;
  }

  Iterator operator+(int n) noexcept { return Iterator(iterator, index + n); }

  Iterator operator-(int n) noexcept { return Iterator(iterator, index - n); }

  size_t operator-(const Iterator &other) noexcept {
    return index - other.index;
  }

  constexpr bool operator!=(const Iterator &other) const {
    return iterator != other.iterator || index != other.index;
  }

  constexpr bool operator==(const Iterator &other) const {
    return iterator == other.iterator && index == other.index;
  }

  constexpr Reference<Word> operator*() const noexcept {
    return Reference<Word>(*(iterator + index / WORD_SIZE), index % WORD_SIZE);
  }
};

template <class Container> class Lens {
private:
  Container &container;

public:
  using Word =
      typename std::decay<decltype(std::declval<Container>()[0])>::type;
  static constexpr auto WORD_SIZE = WordSize<Word>::value;

  constexpr Lens(Container &c) noexcept : container(c) {}

  /**
   * returns the number of bits that fit in the container
   */
  size_t size() noexcept(noexcept(container.size())) {
    return container.size() * WORD_SIZE;
  }

  /**
   * resizes the container to hold at least `N` bits.
   * Note that the actual size will be a multiple of the bits in a word.
   */
  void resize(size_t N,
              bool v = false) noexcept(noexcept(container.resize(0, 0))) {
    Word offset = N % WORD_SIZE == 0 ? 0 : 1;
    container.resize(N / WORD_SIZE + offset, v ? ~0 : 0);
  }

  [[deprecated("legacy API: use `resize(i,v)` insted")]] void
  resizeToHold(size_t N, bool v = false) {
    Word offset = N % WORD_SIZE == 0 ? 0 : 1;
    container.resize(N / WORD_SIZE + offset, v ? ~0 : 0);
  }

  /**
   * returns a Reference to the bit at position `i`.
   */
  constexpr Reference<Word> operator[](size_t i) const
      noexcept(noexcept(container[0])) {
    Word offset = i % WORD_SIZE;
    return Reference<Word>(container[i / WORD_SIZE], offset);
  }

  [[deprecated("legacy API: use `[i]` insted")]] bool get(size_t i) const
      noexcept(noexcept((*this)[i])) {
    return (*this)[i];
  }

  [[deprecated("legacy API: use `[i] = v` insted")]] void
  set(size_t i, bool v) noexcept(noexcept((*this)[i])) {
    (*this)[i] = v;
  }

  constexpr auto begin() noexcept(noexcept(container.begin())) {
    return Iterator(container.begin(), 0);
  }

  constexpr auto end() noexcept(noexcept(container.begin())) {
    return Iterator(container.begin(), size());
  }

  constexpr auto begin() const noexcept(noexcept(container.begin())) {
    return Iterator(container.begin(), 0);
  }

  constexpr auto end() const noexcept(noexcept(container.begin())) {
    return Iterator(container.begin(), size());
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
 * creates a bitlens that owns its data
 */
template <class C> class Container : public Lens<C> {
private:
  C dataContainer;

public:
  constexpr Container() noexcept(noexcept(C())) : Lens<C>(dataContainer) {}
  constexpr Container(const Container &other) noexcept(
      noexcept(C(std::declval<const C &>())))
      : dataContainer(other.data()), Lens<C>(dataContainer) {}
  constexpr Container(Container &&other) noexcept(
      noexcept(C(std::declval<C &&>())))
      : dataContainer(std::move(other.data())), Lens<C>(dataContainer) {}

  template <typename... Args>
  Container(Args &&... args)
      : dataContainer(std::forward<Args...>(args...)), Lens<C>(dataContainer) {}

  /**
   *  get the underlying data container
   */
  C &data() noexcept { return dataContainer; }

  /**
   *  get the underlying data container
   */
  const C &data() const noexcept { return dataContainer; }
};

} // namespace bit_lens
