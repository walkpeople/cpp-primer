# 模板

## 1. 函数模板

1. **定义一个函数模板**
   * 使用`typename`来声明模板参数. 早期也用`class`


```c++
template <typename T> int compare(const T &v1, const T &v2) {
  if (v1 < v2) return -1;
  if (v1 > v2) return 1;
  return 0;
}
```



2. **实例化函数模板**

​		当调用一个函数模板时，编译器会根据函数实参来推断模板实参。然后实例化`(instantiate)`一个特定版本的函数。如:  `compare(2, 3)`,编译器将实例化出	

```c++
int compare(const int &v1, const int &v2){....}
```



3. **模板类型参数/非模板类型参数**

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



4.  **`inline`/`constexpr`的函数模板。**

`inline`和`constexpr`说明符置于模板参数列表之后， 返回值之前 。

```c++
template <typename T> inline T foo() {...}
```



## 2. 类模板

1. **编译器不能为类模板推断模板参数类型。使用 *显式模板实参* 来指定模板参数类型。定义**

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

   

2. **实例化模板**，使用 **显式模板实参** 来指定模板参数类型

   ```c++
   Blob<int> ia;
   Blob<int> ia2 = {0, 1, 2, 3, 4};
   ```

   

3. **类模板的成员函数**：既可以类模板内部定义，也可以在外部定义.在外部声明如下: 

   ```c++
   template <typename T>
   void Blob<T>::check(size_type i, const std::string& msg) const {
     if (i >= data->size()) throw std::out_of_range(msg);
   }
   ```

   

4. **类模板成员函数的实例化:**

   默认情况下，一个类模板的成员函数只有当程序用到它时才进行实例化。如果一个函数没有被使用，那么编译器不会将它实例化。

   

5. **在类代码内简化模板类名的使用**

   ​	当使用一个类模板类型时必须指定模板实参。 但有一个例外： 便是在类模板自己的作用域内，可以直接使用模板名而不提供实参。
   
   ```c++
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
   
       // ++i; 此处直接使用模板名而不提供实参
       BlobPtr operator++();
       BlobPtr operator--();
   
     private:
       std::shared_ptr<std::vector<T>> check(std::size_t, const std::string &msg) const;
       std::weak_ptr<std::vector<T>> wptr;
       std::size_t curr;
   }
   ```
   
   
   
6. **在类代码外使用模板名:**

   进入类的作用域后也可省略实参。

   ```c++
   template<typename T> 
   BlobPtr<T> BlobPtr<T>::operator++(int) {
     // 与 BlobPtr<T> ret = *this;等价
     BlobPtr ret = *this;
     ++*this;
     return ret;
   }
   ```



7. **类模板与友元**

   如果一个类内包含一个非模板友元，则友元被授权访问所有该类的模板实例。

   如果友元自身为模板，类可以授权给所有友元模板实例，也可以只授权给特定实例。

   1. 一对一

      ```c++
      // 前置声明; 在Blob中声明友元所需要的
      // 声明模板；模板声明包含模板参数列表
      template <typename> class BlobPtr;
      template <typename> class Blob;
      
      template <typename T>
      bool operator==(const Blob<T>&, const Blob<T>&);
      
      template <typename T>
      class Blob {
        friend class BlobPtr<T>;
        // 友元的声明使用Blob的模板形参作为自己模板实参
        // 因此友好关系被限定在用相同类型实例化的Blob与BlobPtr相等运算符之间。
        friend bool operator==<T>(const Blob<T>&, const Blob<T>&);
      };
      ```

   2. 通用和特定的模板

      一个类可以将另一个模板的每个实例都声明为自己的友元。或者限定特定的实例为友元。

      **Note:** 为了使所有实例成为友元，友元声明中必须使用与类模板本身不同的模板参数。

      ```c++
      // 一个类可以将另一个模板的每个实例都声明为自己的友元。或者限定特定的实例为友元。
      // 前置声明，在将模板的一个特定实例声明为友元要用到。
      template <typename T> class Pal;
      class C {
        friend class Pal<C>; // 用类C实例化的Pal是C的一个友元
        // Pal2的所有实例都是C的友元; 此情况无须前置声明
        template <typename T> friend class Pal2;
      };
      
      template <typename T> class C2 {
        // C2的每个实例将相同实例化的Pal声明为友元
        friend class Pal<T>;  // Pal的模板声明必须在作用域之内;
        template <typename X> friend class Pal2; // Pal2的所有实例都是C2的每个实例的友元,不需要前置声明
        // Pal3 是一个非模板类，它是C2所有实例的友元。
        friend class Pal3; //不需要Pal3的前置声明。
      }
      ```

   

8. **模板自己的类型成为友元**

   ​	新标准中允许将模板类型声明为友元。

   ```c++
   template <typename T> clas Bar {
       friend T;
   }
   ```

   

 9. **模板类型别名**

    1. 使用`typedef`来引用一个实例化的类，但无法定义一个`typedef` 引用一个模板。

       ```c++
       typedef Blob<string> StrBlob;
       ```

    2. 虽然无法使`typedef`引用一个模板，但新标准允许我们为类模板定义一个类型别名。

       ```c++
       template <typename T> using twin = pair<T, T>;
       twin<string> authors;
       ```

    3. 为类模板定义类型别名时，也可以固定一个或多个模板参数。

       ```c++
       template <typename T> using partNo = pair<T, unsigned>;
       partNo<string> books; // books => pair<string, unsigned>
       ```

 10. **类模板的`static`成员**

     ​	一个static成员函数只有在使用时才会实例化。

     1. 类模板可以声明`static`成员, 如下面的例子，对于指定类型T, 所有`Foo<T>`类型共享相同的`curr`对象和`count()`函数。  所有`Foo<string>`共享`Foo<string>::curr`, `Foo<int`共享`Foo<int>::curr`
     
         ```c++
         template <typename T> class Foo {
         public: 
            static std::size_t count()  { return curr; }
         private:
             static std::size_t curr;
         }
         ```
     
     2. 在类模板外部定义静态成员，与定义类模板的其他成员类似。 当使用一个特定的模板实参类型实例化Foo时，将会为该类型实例化一个独立的`curr`并初始化为0.
     
         ```c++
         template <typename T> Foo<T>::curr = 0;
         ```
     
         
     
     3. 访问类模板的静态成员时，必须同时指定模板类型实参。
     
         ```c++
         Foo<int> fi;
         auto ct = Foo<int>::count();
         ct = fi.count();
         ct = Foo<int>::curr;
         ```
     
         

## 3. 模板参数



