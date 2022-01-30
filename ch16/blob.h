#include <initializer_list>
#include <iostream>
#include <memory>
#include <vector>
// 前置声明; 在Blob中声明友元所需要的
// 声明模板；模板声明包含模板参数列表
template <typename>
class BlobPtr;
template <typename>
class Blob;

template <typename T>
bool operator==(const Blob<T> &, const Blob<T> &);

template <typename T>
class Blob {
  friend class BlobPtr<T>;
  // 友元的声明使用Blob的模板形参作为自己模板实参
  // 因此友好关系被限定在用相同类型实例化的Blob与BlobPtr相等运算符之间。
  friend bool operator==<T>(const Blob<T> &, const Blob<T> &);

 public:
  typedef T value_type;
  typedef typename std::vector<T>::size_type size_type;

  // Constructor
  Blob() : data(std::make_shared<std::vector<T>>()) {}
  Blob(std::initializer_list<T> il)
      : data(std::make_shared<std::vector<T>>(il)) {}

  // Number of elements in the Blob;
  size_type size() const { return data->size(); };
  bool empty() { return data->empty(); };
  // Add or Delete element
  void push_back(const T &t) { data->push_back(t); }
  void push_back(const T &&t) { data->push_back(std::move(t)); }
  void pop_back();

  // visit element
  T &back();
  T &operator[](size_type i);

 private:
  std::shared_ptr<std::vector<T>> data;
  void check(size_type i, const std::string &msg) const;
};

template <typename T>
void Blob<T>::check(size_type i, const std::string &msg) const {
  if (i >= data->size()) throw std::out_of_range(msg);
}

template <typename T>
T &Blob<T>::back() {
  check(0, "back on empty blob");
  return data->back();
}

template <typename T>
void Blob<T>::pop_back() {
  check(0, "pop_back on empty blob");
  data->pop_back();
}

template <typename T>
T &Blob<T>::operator[](size_type i) {
  check(i, "subscript out of range");
  return (*data)[i];
}

// 一个类可以将另一个模板的每个实例都声明为自己的友元。或者限定特定的实例为友元。
// 前置声明，在将模板的一个特定实例声明为友元要用到。
template <typename T>
class Pal;
class C {
  friend class Pal<C>;  // 用类C实例化的Pal是C的一个友元
  // Pal2的所有实例都是C的友元; 此情况无须前置声明
  template <typename T>
  friend class Pal2;
};

template <typename T>
class C2 {
  // C2的每个实例将相同实例化的Pal声明为友元
  friend class Pal<T>;  // Pal的模板声明必须在作用域之内;
  template <typename X>
  friend class Pal2;  // Pal2的所有实例都是C2的每个实例的友元,不需要前置声明
  // Pal3 是一个非模板类，它是C2所有实例的友元。
<<<<<<< HEAD
  friend class Pal3; //不需要Pal3的前置声明。
=======
  friend class Pal3;  //不需要Pal3的前置声明。
>>>>>>> 9374c1ba57846016dd563896ee315c587969379c
};
