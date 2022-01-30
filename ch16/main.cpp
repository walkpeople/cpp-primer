#include <iostream>

#include "blob.h"
#include "debug_delete.h"

int main() {
  double *p = new double;
  DebugDelete d;
  d(p);
  int *ip = new int;
  DebugDelete()(ip);
}

