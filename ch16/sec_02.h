#include <iostream>
#include <sstream>
#include <type_traits>

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