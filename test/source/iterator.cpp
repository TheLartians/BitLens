#include <algorithm>
#include <bit_lens.h>
#include <doctest/doctest.h>
#include <vector>

TEST_CASE_TEMPLATE("bit iterator", T, unsigned char, unsigned short,
                   unsigned int, size_t) {
  bit_lens::Container<std::vector<T>> container;
  CHECK(container.begin() == container.end());
  container.resize(100, 0);
  CHECK(container.begin() != container.end());
  CHECK(container.begin() + container.size() == container.end());
  CHECK(container.end() - container.begin() == container.size());
  CHECK(std::count(container.begin(), container.end(), true) == 0);
  std::fill(container.begin(), container.begin() + 50, 1);
  CHECK(std::count(container.begin(), container.end(), true) == 50);
  container.resize(200, 0);
  std::copy(container.begin() + 25, container.begin() + 75,
            container.begin() + 75);
  CHECK(std::count(container.begin(), container.end(), true) == 75);
}
