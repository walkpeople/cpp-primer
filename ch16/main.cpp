#include <iostream>

#include "blob.h"
#include "sec_02.h"

using std::cout;
using std::endl;

void f(int v1, int &v2) {
  cout << v1 << " " << ++v2 << endl;
}

void g(int &&v1, int &v2) {
  cout << v1 << " " << v2 << endl;
}

void test_sec_02_type_trans() {
  std::string str = "hello";
  char s = sec_02::fcn2(str.begin(), str.end());
  cout << s << endl;
}

void test_sec_02_override() {
  std::string s("hi");
  cout << sec_02::debug_rep(s) << endl;
}

void test_forward() {
  int i = 1;
  f(42, i);
  cout << "Actual i = " << i << endl;
  sec_02::flip1(f, 42, i);
  cout << "Actual i = " << i << endl;
  sec_02::flip2(f, 42, i);
  cout << "Actual i = " << i << endl;
  g(42, i);
  // Error: 不能从一个左值实例化出一个int &&(i -> int &&)
  sec_02::flip2(g, 42, i);
  sec_02::flip(g, 42, i);
}

void test_blob() {
  Blob<int> blob;
  for (size_t i = 0; i < 100; i++) {
    blob.push_back(i);
  }
  cout << "size: " << blob.size() << endl;
  cout << "blob[0]: " << blob[0] << endl;
  blob.pop_back();
  cout << "size: " << blob.size() << endl;
  cout << "empty: " << blob.empty() << endl;
}
int main() {
  test_sec_02_type_trans();
  test_sec_02_override();
}
