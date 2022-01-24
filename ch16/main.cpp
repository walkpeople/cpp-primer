#include <iostream>

#include "blob.h"

using std::cout;
using std::endl;
int main() {
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