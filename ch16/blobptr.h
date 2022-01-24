#include <iostream>
#include <memory>
#include <vector>

template<typename T> class Blob;
template<typename T> class BlobPtr{
  public:
    BlobPtr(): curr(0) {}
    BlobPtr(Blob<T> &a, size_t sz = 0) : wptr(a.data), curr(sz) {}
    T& operator*() const {
      auto p = check(curr, "deference past end");
      return (*p)[curr];
    }

    // ++i;
    BlobPtr operator++();
    BlobPtr operator--();

    // i++;
    BlobPtr operator++(int);
    BlobPtr operator--(int);

  private:
    std::shared_ptr<std::vector<T>> check(std::size_t, const std::string &msg) const;
    std::weak_ptr<std::vector<T>> wptr;
    std::size_t curr;
};

template<typename T> 
BlobPtr<T> BlobPtr<T>::operator++(int) {
  // 与 BlobPtr<T> ret = *this;等价
  BlobPtr ret = *this;
  ++*this;
  return ret;
}
