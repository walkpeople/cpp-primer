#include <initializer_list>
#include <iostream>
#include <memory>
#include <vector>

template <typename T> class Blob {
public:
  typedef T value_type;
  typedef typename std::vector<T>::size_type size_type;

  // Constructor
  Blob();
  Blob(std::initializer_list<T> il);
  // Number of elements in the Blob;
  size_type size() const { return data->size(); };
  bool empty() { data->empty(); };

private:
  std::shared_ptr<std::vector<T>> data;
}
