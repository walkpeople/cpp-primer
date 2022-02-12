#include <iostream>
#include <sstream>
#include <type_traits>
#include <type_traits>
#include <utility>

namespace sec_02 {
template <typename It>
auto fcn2(It beg, It end) ->
    typename std::remove_reference<decltype(*beg)>::type {
  return *beg;
}

template <typename T>
std::string debug_rep(const T &t) {
  std::ostringstream ret;
  ret << t;
  return ret.str();
}

template <typename T>
std::string debug_rep(T *p) {
  ostringstream ret;
  ret << "pointer: " << p;
  if (p)
    ret << " " << debug_rep(*p);
  else
    ret << " null pointer ";
  return ret.str();
}
}  // namespace sec_02
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
