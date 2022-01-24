# 模板

## 1. 函数模板

### 定义一个函数模板

* 使用`typename`来声明模板参数. 早期也用`class`

```c++
template <typename T> int compare(const T &v1, const T &v2) {
  if (v1 < v2) return -1;
  if (v1 > v2) return 1;
  return 0;
}
```

### 实例化函数模板

​	当调用一个函数模板时，编译器会根据函数实参来推断模板实参。然后实例化`(instantiate)`一个特定版本的函数。如:  `compare(2, 3)`,编译器将实例化出

```c++
int compare(const int &v1, const int &v2){....}
```



### 模板类型参数/非模板类型参数

   模板类型参数一般使用`typename`和`class`来修饰。模板类型参数也可以作为函数的返回类型和参数类型，也可以在函数内用于变量声明和类型转换。

```c++
template <typename T> T foo(T *t) {
    T tmp = *t;
    return tmp;
} 
```

​	一个非模板类型参数表示一个值而非一个类型。模板实例化，非模板类型参数将由编译器用一个值来替换。

```c++
template <unsigned N, unsigned M>int compare(const char (&a)[N], const char (&b)[M]) {
  return strcmp(a, b);
}
```

 `compare("hello", "world")`将被编译器实例化为: *注意:  编译器将在字符串字面常量末尾加上一个空字符作为终结符*

```c++
int compare(const char a[6], char b[7])
```



### `inline`/`constexpr`的函数模板。

`inline`和`constexpr`说明符置于模板参数列表之后， 返回值之前 。

```c++
template <typename T> inline T foo() {...}
```



## 2. 类模板

1. 编译器不能为类模板推断模板参数类型。使用 **显式模板实参** 来指定模板参数类型。定义

   ```c++
   template <typename T>
   class Blob {
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
     void push_back(const T& t) { data->push_back(t); }
     void push_back(const T&& t) { data->push_back(std::move(t)); }
     void pop_back();
   
     // visit element
     T& back();
     T& operator[](size_type i);
   
    private:
     std::shared_ptr<std::vector<T>> data;
     void check(size_type i, const std::string& msg) const;
   };
   ```

   

2. 实例化模板，使用 **显式模板实参** 来指定模板参数类型

   ```c++
   Blob<int> ia;
   Blob<int> ia2 = {0, 1, 2, 3, 4};
   ```

   

3. 类模板的成员函数：既可以类模板内部定义，也可以在外部。在外部声明如下: 

   ```c++
   template <typename T>
   void Blob<T>::check(size_type i, const std::string& msg) const {
     if (i >= data->size()) throw std::out_of_range(msg);
   }
   ```

   

4. 类模板成员函数的实例化:

   默认情况下，一个类模板的成员函数只有当程序用到它时才进行实例化。如果一个函数没有被使用，那么编译器不会将它实例化。

   

5. 在类代码内简化模板类名的使用

   

