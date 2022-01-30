#include <type_traits>

namespace sec_02 {
template <typename It>
auto fcn2(It beg, It end) ->
    typename std::remove_reference<decltype(*beg)>::type {
  return *beg;
}
}