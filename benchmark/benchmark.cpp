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
  auto gen = std::bind(std::uniform_int_distribution<>(0, 1), std::default_random_engine());
  for (size_t i = 0; i < array.size(); ++i) {
    std::bitset<sizeof(T)> bits;
    for (size_t j = 0; j < bits.size(); ++j) {
      bits[j] = gen();
    }
    array[i] = static_cast<T>(bits.to_ullong());
  }
  return array;
}

template <class T, size_t N> void bitwiseRandomAccessBenchmark(benchmark::State &state) {
  auto a = createRandomData<T>(N);
  auto b = createRandomData<T>(N);
  auto c = createRandomData<T>(N);
  auto va = bit_lens::BitLens(a);
  auto vb = bit_lens::BitLens(b);
  auto vc = bit_lens::BitLens(c);
  auto gen
      = std::bind(std::uniform_int_distribution<size_t>(0, N - 1), std::default_random_engine());
  for (auto _ : state) {
    for (size_t i = 0; i < N; ++i) {
      if constexpr (std::is_same<T, bool>::value) {
        c[gen()] = a[gen()] && !b[gen()];
      } else {
        vc[gen()] = va[gen()] && !vb[gen()];
      }
      benchmark::DoNotOptimize(a);
      benchmark::DoNotOptimize(b);
      benchmark::DoNotOptimize(c);
    }
  }
}

auto bitwiseRandomAccessVectorBool = bitwiseRandomAccessBenchmark<bool, 10000>;
auto bitwiseRandomAccessVectorChar = bitwiseRandomAccessBenchmark<unsigned char, 10000>;
auto bitwiseRandomAccessVectorInt = bitwiseRandomAccessBenchmark<unsigned int, 10000>;

BENCHMARK(bitwiseRandomAccessVectorBool);
BENCHMARK(bitwiseRandomAccessVectorChar);
BENCHMARK(bitwiseRandomAccessVectorInt);

template <class T> auto valuewiseDifference(const T &a, const T &b) {
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
  vc.resize(N);
  for (size_t i = 0; i < N; ++i) {
    vc[i] = va[i] & ~vb[i];
  }
  return c;
}

template <class T, size_t N> void valuewiseDifferenceBenchmark(benchmark::State &state) {
  auto a = createRandomData<T>(N);
  auto b = createRandomData<T>(N);
  for (auto _ : state) {
    auto c = valuewiseDifference(a, b);
    benchmark::DoNotOptimize(a);
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(c);
  }
}

template <class T, size_t N> void bitwiseDifferenceBenchmark(benchmark::State &state) {
  auto a = createRandomData<T>(N);
  auto b = createRandomData<T>(N);
  for (auto _ : state) {
    auto c = bitwiseDifference(a, b);
    benchmark::DoNotOptimize(a);
    benchmark::DoNotOptimize(b);
    benchmark::DoNotOptimize(c);
  }
}

auto bitwiseDifferenceVectorBool
    = valuewiseDifferenceBenchmark<bool, 10000>;  // vector bool elements are
                                                  // actually bits
auto bitwiseDifferenceVectorChar = bitwiseDifferenceBenchmark<unsigned char, 10000>;
auto bitwiseDifferenceVectorInt = bitwiseDifferenceBenchmark<unsigned int, 10000>;
auto valuewiseDifferenceVectorChar = valuewiseDifferenceBenchmark<unsigned char, 10000>;
auto valuewiseDifferenceVectorInt = valuewiseDifferenceBenchmark<unsigned int, 10000>;
auto valuewiseDifferenceVectorSizeT = valuewiseDifferenceBenchmark<size_t, 10000>;

BENCHMARK(bitwiseDifferenceVectorBool);
BENCHMARK(bitwiseDifferenceVectorChar);
BENCHMARK(bitwiseDifferenceVectorInt);
BENCHMARK(valuewiseDifferenceVectorChar);
BENCHMARK(valuewiseDifferenceVectorInt);
BENCHMARK(valuewiseDifferenceVectorSizeT);

BENCHMARK_MAIN();
