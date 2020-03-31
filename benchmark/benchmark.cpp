#include <benchmark/benchmark.h>
#include <bit_lens.h>
#include <bitset>
#include <random>
#include <type_traits>

template <class T> auto createRandomData(size_t bytes) {
  std::vector<T> array;
  if (std::is_same<T, bool>::value) {
    // vector<bool> stores only a single bit per entry
    array.resize(bytes * CHAR_BIT);
  } else {
    // avector<T> can store sizeof(T) bytes per entry
    auto offset = bytes % sizeof(T) == 0 ? 0 : 1;
    array.resize(bytes / sizeof(T) + offset);
  }
  auto gen = std::bind(std::uniform_int_distribution<>(0, 1),
                       std::default_random_engine());
  for (size_t i = 0; i < array.size(); ++i) {
    std::bitset<sizeof(T)> bits;
    for (size_t j = 0; j < bits.size(); ++j) {
      bits[j] = gen();
    }
    array[i] = static_cast<T>(bits.to_ullong());
  }
  return array;
}

template <class T, size_t N>
void bitwiseRandomAccessBenchmark(benchmark::State &state) {
  auto a = createRandomData<T>(N);
  auto b = createRandomData<T>(N);
  auto c = createRandomData<T>(N);
  auto va = bit_lens::BitLens(a);
  auto vb = bit_lens::BitLens(b);
  auto vc = bit_lens::BitLens(c);
  auto gen = std::bind(std::uniform_int_distribution<size_t>(0, N - 1),
                       std::default_random_engine());
  for (auto _ : state) {
    for (size_t i = 0; i < N; ++i) {
      if constexpr (std::is_same<T, bool>::value) {
        c[gen()] = a[gen()] && !b[gen()];
      } else {
        vc.set(gen(), va.get(gen()) && !vb.get(gen()));
      }
      benchmark::DoNotOptimize(a);
    }
  }
}

auto bitwiseRandomAccessVectorBool = bitwiseRandomAccessBenchmark<bool, 10000>;
auto bitwiseRandomAccessVectorChar =
    bitwiseRandomAccessBenchmark<unsigned char, 10000>;
auto bitwiseRandomAccessVectorInt =
    bitwiseRandomAccessBenchmark<unsigned int, 10000>;

BENCHMARK(bitwiseRandomAccessVectorBool);
BENCHMARK(bitwiseRandomAccessVectorChar);
BENCHMARK(bitwiseRandomAccessVectorInt);

template <class T> auto bytewiseDifference(const T &a, const T &b) {
  auto N = a.size();
  T c;
  c.resize(N);
  for (size_t i = 0; i < N; ++i) {
    c[i] = a[i] & ~b[i];
  }
  return c;
}

template <class T> auto bitwiseDifference(const T &a, const T &b) {
  T c;
  auto va = bit_lens::BitLens(a);
  auto vb = bit_lens::BitLens(b);
  auto vc = bit_lens::BitLens(c);
  auto N = va.size();
  vc.resizeToHold(N);
  for (size_t i = 0; i < N; ++i) {
    vc.set(i, va.get(i) & ~vb.get(i));
  }
  return c;
}

template <class T, size_t N>
void bytewiseDifferenceBenchmark(benchmark::State &state) {
  auto a = createRandomData<T>(N);
  auto b = createRandomData<T>(N);
  for (auto _ : state) {
    auto c = bytewiseDifference(a, b);
    benchmark::DoNotOptimize(a);
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(c);
  }
}

template <class T, size_t N>
void bitwiseDifferenceBenchmark(benchmark::State &state) {
  auto a = createRandomData<T>(N);
  auto b = createRandomData<T>(N);
  for (auto _ : state) {
    auto c = bitwiseDifference(a, b);
    benchmark::DoNotOptimize(a);
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(c);
  }
}

auto bitwiseDifferenceVectorBool =
    bytewiseDifferenceBenchmark<bool, 10000>; // vector bool elements are
                                              // actually bits
auto bitwiseDifferenceVectorChar =
    bitwiseDifferenceBenchmark<unsigned char, 10000>;
auto bitwiseDifferenceVectorInt =
    bitwiseDifferenceBenchmark<unsigned int, 10000>;
auto bytewiseDifferenceVectorChar =
    bytewiseDifferenceBenchmark<unsigned char, 10000>;
auto bytewiseDifferenceVectorInt =
    bytewiseDifferenceBenchmark<unsigned int, 10000>;
auto bytewiseDifferenceVectorSizeT = bytewiseDifferenceBenchmark<size_t, 10000>;

BENCHMARK(bitwiseDifferenceVectorBool);
BENCHMARK(bitwiseDifferenceVectorChar);
BENCHMARK(bitwiseDifferenceVectorInt);
BENCHMARK(bytewiseDifferenceVectorChar);
BENCHMARK(bytewiseDifferenceVectorInt);
BENCHMARK(bytewiseDifferenceVectorSizeT);

BENCHMARK_MAIN();
