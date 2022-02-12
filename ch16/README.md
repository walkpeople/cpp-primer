# 定义模板

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

1. **使用类的类型成员**

   ​	可以使用`::(作用域运算符)`来访问类的static成员和类型成员，但对于类模板，可能会产生歧义，如`T::mem`我们并不知道`mem`是一个static成员还是类型成员。普通类如`string::size_type`编译器根据string的定义，从而知道`size_type`是一个类型。但`T::mem`直到模板实例化时候才能知道。因为我们需要显式告诉编译器`mem`是类型还是static成员。通过`typename`来指定：

   ```c++
   template <typename T> typename T::value_type top(const T& c) {
      
   }
   ```

2. **默认模板实参**

   在新标准中，我们可以为函数和类模板提供默认实参，但早期版本只允许为类模板提供默认实参。在下

   ```c++
   template <typename T, typename F = less<T>> 
   int compare (const T &v1, const T&v2, F f = F()) {
       
   }
   ```

3. **模板默认实参与类模板**

   ​	使用类模板模板时，我们必须使用尖括号指定模板类型，但如果一个类模板为其所有模板参数都提供了默认实参，且我们希望使用这些默认实参，那可以模板名后跟一个空尖括号对。

   ```c++
   template <typename T = int>
   class Numbers { ... }
   
   // 使用默认参数版本。
   Numbers<> n;
   ```




# 模板实参推断

​	对于函数模板，从函数实参来确定模板实参的过程为 **模板实参推断**

## 1. 类型转换与模板类型参数

类型转换能应用于函数模板的情况有如下两种:

* `const`转换: 可以将一个非`const`对象的引用(或指针)传递给一个`const`的引用(或指针形参)
* 数组或函数指针转换： 如果函数形参不是引用类型，则可以对数组 或函数类型的实参应用正常的指针转换。一个数组实参可以转换为一个指向其首元素的指针。同样，一个函数实参可以转为一个函数类型的指针。

​	**Note~~~** : 其他类型转换如算术转换， 派生类向基类的转换，以及用户自定义的转换(转换构造函数`converting constructor`和重载类型转换运算符`conversion operator`)， **都不能应用于函数模板**



**使用相同的模板参数类型的函数形参 **

```c++
template <typename T>> 
int compare (const T &v1, const T&v2) {
    ...
}

// 以下调用是错误的， 因为两个实参类型不一致
long lng;
compare(lng, 24); 
```



**正常类型转换应用于普通函数实参**

​	如下: `os`参数遵循正常类型转换。即如果函数参数类型不是模板参数，则对实参进行正常的类型转换。

```c++
template <typename T> ostream &print(osstream &os, const T &obj) {
    return os << obj;
}
```



## 2. 函数模板显式实参

**指定显式模板实参**

​	提供显式模板实参与定义类模板实例的方式相同，显示模板实参在尖括号中给出，位于函数名之后，实参列表之前 。显式模板实参按由左到右的顺序 瓦屋 对应的模板参数匹配。只有最右参数的显式模板参数可以忽略且前提是可以从函数参数推断出来。

```c++
template <typename T1, typename T2, typename T3>
T1 sum(T2, T3){...}

// T1需要指定显式模板实参, T2, T3从函数实参类型推断而来。
auto val3 = sum<long long>(i, lng);
```



**正常类型转换应用于显式指定的实参**

对于模板类型参数已经显式指定了的函数实参，进行正常的类型转换。

```c++
long lng;
compare<long>(lng, 1024);
compare<int>(lng, 1024);
```



## 3. 尾置返回类型与类型转换

​	当不确定返回结果的准确类型，但可以确定返回结果与传入实参类型一致时，使用尾置返回来指定返回类型

```c++
template <typename It>
auto fcn(It beg, It end) -> decltype(*beg) {
    return *beg;
}
// 如果对一个string序列调用该函数，返回类型将是string &,如果是int序列，则返回类型是int&
```



**进行类型转换的标准库模板类**

​	我们编写上述fcn函数，如果希望返回一个值而不是一个引用。我们可以使用标准库的 **类型转换**来获取元素类型. 这些模板定义在`type_traits`头文件中。

- 使用`remove_reference`来获得元素类型。

  ```c++
  // remove_reference::type脱去引用，剩下元素类型本身。
  remove_reference<decltype(*beg)>::type;
  ```

- 组合使用`remove_reference`,尾置返回及`decltype`，就可以在函数中返回元素值的拷贝。

  - Note: type是一个类的成员，而该类依赖于一个模板参数，因此，必须在返回类型的声明中使用`typename`来告知编译器，`type`表示一个类型。

  ```c++
  #include <type_traits>
  
  template <typename It>
  auto fcn2(It beg, It end) ->
      typename std::remove_reference<decltype(*beg)>::type {
    return *beg;
  }
  ```

- 标准类型转换模板

  | 对Mod<T> 其中Mod为   | 若T为                     | 则Mod<T>::type为 |
  | -------------------- | ------------------------- | ---------------- |
  | remove_reference     | X& 或 X&&<br>否则         | X<br>T           |
  | add_const            | X&、const X或函数<br>否则 | T<br>const T     |
  | add_lvalue_reference | X&<br>X&&<br>否则         | T<br>X&<br>T&    |
  | add_rvalue_reference | X&或X&&<br/>否则          | T<br/>T&&        |
  | remove_pointer       | X*<br>否则                | X<br>T           |
  | add_pointer          | X&或X&&<br>否则           | X*<br>T*         |
  | make_signed          | unsigned X<br>否则        | X<br>T           |
  | make_signed          | 带符号类型<br>否则        | unsingned X<br>T |
  | remove_extent        | X[n]<br>否则              | X<br>T           |
  | remove_all_extents   | X [n1]  [n2]...<br>否则   | X<br>T           |

  

## 4. 函数指针和实参判断 

​	使用一个函数模板初始化一个函数指针或为一个函数指针赋值时，编译器使用指针的类型来推断模板实参

* 为一个函数指针赋值

  ```c++
  template <typename T> int compare(const T&, const T&);
  int (*pf1)(const int &, const int&) = compare;
  ```

* 如果不能从函数指针类型确定模板实参，则会产生错误

  ```c++
  void func(int(*)(const string&, const string&));
  void func(int(*)(const int&, const int&));
  
  // 错误!!!,无法确定使用comapre的哪个实例
  func(compare);
  // fix: 使用 显式模板实参 来消除func调用的歧义 
  func(compare<int>);
  ```

  

## 5. 模板实参推断和引用

**从左值引用函数参数推断类型**

* 当一个函数参数是模板类型参数的一个普通 (左值)引用时(即，形如T&),绑定规则告诉我们只能传递给它一个左值(如，一个变量或一个返回引用类型的表达式)，实参可以是`const`类型，也可以不是，如果实参是`const`的，则T将被推断为`const`类型

  ```c++
  template <typename T> void f1(T&); // 实参必须为左值
  f1(i);  // i是一个int;模型参数类型为int 
  f1(ci); // ci是一个const int ; 模板参数类型为 const int 
  f1(5); // 错误！！！， 传递给一个&参数的实参必须是一个左值
  ```

* 函数参数的类型为`const T&`, 正常绑定规则告诉我们： 可以传递给它任何类型的实参- 一个对象(`const`或`非const`)，一个临时对象或一个字面常量值。当函数参数本身是`const`时，T的类型推断的结果 不会是一个`const`类型。`const`已经是函数参数类型的一部分： 因此，它不会也是模板参数类型的一部分。

  ```c++
  template <typename T> void f2(const T&);
  
  f2(i); // i是一个int; 模板参数为int
  f2(ci); // ci 是 const int ; 模板参数为 int 
  f2(5); // 一个const &参数可以绑定到一个右值; 模板参数为int
  ```

  

**从右值引用函数参数推断类型**

​	当函数参数是一个右值时，类型推断过程类似普通左值引用函数参数的推断过程。推断T的类型是该右值实参的类型。

```c++
template <typename T> void f3(T&&);
f3(42); // 模板类型为 int
```



**引用折叠和右值引用参数**

> 如果一个函数参数是指向模板参数类型的右值引用(如 T&&) ,则可以传递给它任意类型的实参。

正常绑定规则之外的两个例外

* 将一个左值传递给函数的右值引用参数，且此右值引用指向模板类型参数(如 T&&),编译器推断模板类型参数为实参左值引用类型。 因此 `f3(i)` 编译器推断T的类型为`int &`而非int。 T被推断为`int &`看起来意味着f3的函数参数应该是一个类型`int &`的右值引用。

* 引用折叠，通常情况下无法创建一个引用的引用，但是通过类型别名或通过模板类型参数间接定义是可以的。在这种情况下。引用会折叠成一个普通的左值引用类型。新标准中，折叠规则扩展到右值引用，只在一种特殊情况下引用会折叠成右值引用：右值引用的右值引用。即给定一个类型X

  * `X& &`,  `X& &&`,  ` X&& &`都折叠成类型`X&`
  * 类型`X && &&`折叠成`X&&`

  > 引用折叠只能应用于间接创建的引用的引用，如类型别名或模板参数。

这两个规则导致了两个重要结果 

* 如果一个函数参数是一个指向模板类型参数的右值引用(如 T&&), 则它可以被绑定到一个左值。
* 如果一个函数参数是一个左值，则推断出的模板实参类型将是一个左值引用，且函数参数将被 实例化一个(普通 ) 左值引用参数(T&)

**编写接受右值引用参数的模板函数**

```c++
template <typename T> void f3(T&& val) {
    T t = val;
    t = fcn(t);
    if (val == t) { ... }
}


// 
// f3(42):  T 为 int,  局部变量t的类型为int, 对t的改变不改变val
// f3(i);   T 为 int &, 局部变量t的类型为int &, 对t的改变也在改变val
```



右值引用通常用于两种情况: *模板转发其实参*和*模板被重载*

> Note: 使用右值引用的函数模板的通常使用方式。

```c++
template <typename T> void f(T&&); // 绑定到非const 右值
template <typename T> void f(const T&); // 左值和const右值
```



## 6. 理解std::move

> 将一个左值`static_cast`为右值引用允许的。

**std::move定义**

```c++
template<typename _Tp>
constexpr typename std::remove_reference<_Tp>::type&& move(_Tp&& __t) noexcept { 
    return static_cast<typename std::remove_reference<_Tp>::type&&>(__t); 
}
```



**传递一个右值**

```c++
string s2;
s2 = std::move(string("bye"));
```

* 推断`T`类型为`string`
* `remove_reference`用`string`进行实例化
* `remove_reference<string>`的type成员为 `string`
* `move`的返回类型为`string &&`
* `move`的函数参数`t`的类型为`string&&`



**传递一个左值**

```c++
string s1 = string("hello"):
s2 = std::move(s1);
```

* 推断`T`类型为`string &`
* `remove_reference`用`string &`进行实例化
* `remove_reference<string &>`的type成员为 `string`
* `move`的返回类型为`string &&`
* `move`的函数参数`t`的类型为`string& &&`会折叠为`string &`



## 7. 转发





# 重载与模板

* 选择更特例化的版本
* 选择非模板函数 
* 如果非模板函数提供更精确的匹配，优先选择。



# 可变参数模板

## 1. 可变参数模板

> 就是一个接受可变数目参数的模板函数 或模板类。可变数目的参数被 称为参数包。参数包分为两类
>
> * 模板参数包： 表示零个或多个模板参数
> * 函数参数包:    表示零个或多个函数参数

**示例**



**`sizeof...`运算符** 

返回一个常量表达式，表示 参数包中有多少个参数。

## 2. 编写可变参数函数模板



## 3. 包扩展

