#include <bit_lens.h>
#include <doctest/doctest.h>

#include <cmath>
#include <deque>
#include <list>
#include <vector>

TEST_CASE_TEMPLATE("bit container", T, std::vector<unsigned char>, std::vector<size_t>,
                   std::deque<size_t>) {
  bit_lens::BitContainer<T> container;
  container.resize(1000, 1);
  CHECK(container.size() >= 100);
  CHECK(container.data().size()
        == std::ceil(container.size() / float(bit_lens::WordSize<typename T::value_type>::value)));
  CHECK(container[0]);
  CHECK(container[container.size() - 1]);
}
