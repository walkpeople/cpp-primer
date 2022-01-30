#include <iostream>

class DebugDelete {
public:
  DebugDelete(std::ostream &s = std::cerr) : os(s) {}
  template <typename T> void operator()(T *p) const {
    os << "deleteing unique_ptr" << std::endl;
    delete p;
  }

private:
  std::ostream &os;
};
