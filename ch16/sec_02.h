#include <iostream>
#include <type_traits>
#include <utility>

namespace sec_02 {
template <typename It>
auto fcn2(It beg, It end) ->
    typename std::remove_reference<decltype(*beg)>::type {
  return *beg;
}

// forward
template <typename F, typename T1, typename T2> void flip1(F f, T1 t1, T2 t2) {
  f(t1, t2);
}

template <typename F, typename T1, typename T2>
void flip2(F f, T1 &&t1, T2 &&t2) {
  f(t1, t2);
}

template <typename F, typename T1, typename T2>
void flip(F f, T1 &&t1, T2 &&t2) {
  f(std::forward<T1>(t1), std::forward<T2>(t2));
}


} // namespace sec_02
