#include <iostream>

#include "blob.h"
<<<<<<< HEAD
#include "debug_delete.h"

int main() {
  double *p = new double;
  DebugDelete d;
  d(p);
  int *ip = new int;
  DebugDelete()(ip);
}

=======
#include "sec_02_type_trans.h"

using std::cout;
using std::endl;

void test_sec_02_type_trans() {
  std::string str = "hello";
  char s = sec_02::fcn2(str.begin(), str.end());
  cout << s << endl;
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
int main() { test_sec_02_type_trans(); }
>>>>>>> 9374c1ba57846016dd563896ee315c587969379c
