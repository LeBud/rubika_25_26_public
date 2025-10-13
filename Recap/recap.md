# Table of content

1. [Stack vs heap](#stack-vs-heap)
1. [Pointer & smart pointer](#pointer--smart-pointer)
1. [Pointer, Reference and copy](#Pointer,-Reference-and-copy)
1. [Container](#Container)
1. [Inheritance](#Inheritance)
1. [Compilation process](#compilation-process)
1. [Template](#Template)
2. [Cast](#cast)

## Stack vs heap

- **Stack**
  * Faster access to this memory than accessing to the heap
  * Allocation and deallocation are automatic. There is no need to handle that part. A reserved space is reserved for each local variable. Those variables will be automatically destroyed at end the end of their scope.

- **Heap**
  * Permanent memory that must be handled manually by the user
  * Allocated a new variable, array, buffer... is processed using **new**, **malloc** or **calloc**
    * **malloc**: Simple allocation on the heap
    * **calloc**: Simple allocation on the heap and set all the allocated memory to a given byte (usually 0)
    * **new**: C++ version. Correspond to a **malloc** + a call to the constructor.
  * Deallocated a variable is proccess using **free** or **delete**
    * **free**: Deallocate an allocation made using **malloc** or **calloc**. Crash if we try to free a `nullptr` pointer.
    * **delete**: Call the destructor of the object and deallocate the mem;ory. Can only be used with memory allocated using **nez**. Can be use with **nullptr**. In that case, nothing will happen.
  * If the allocated memory is not deallocated, the memory will stay reserved which leads in less memory available which might lead to a crash. This is a **memory leak**.

## Pointer & smart pointer

- **pointer** : variable that store the adress of an object in memory. It can be nullptr (which means 0) or have a value. If it has a value, it is impossibe for a program to determine if this value is right or not. (it can point on outside the program's memory, point on already free memory...). Using a invalid pointer will crah.
- Because allocated memory on the **heap** is often not deallocated, wrapper around raw pointers have been created to release the memory once no one needs them : **smart_ptr**
  * **shared_ptr** : Pointer that can be shared between every one. If someone needs it, he can increment a ref count and keep a referece to this wrapper. Once he has done his job using this pointer, he can set it to nullptr, the ref count is decreased. If the ref count turns 0, that means no one use it anymore so the memory can be released.
  * **weak_ptr** : Works in pair with **shared_ptr**. Keeping a **weak_ptr** will not increase the ref count of the **shared_ptr**, but to retrieve the data you must 'upgrade' the **weak_ptr** to a **shared_ptr**. Because it doesn't increase the ref count, it is possible that the **shared_ptr** has already been destroyed. In that case, the 'upgrade' process will fail.

  ```cpp
  	// Create on the heap
	MyClass* mc = new MyClass();

	// Also created on the heap
	std::shared_ptr<MyClass> spMc = std::make_shared<MyClass>();

	// weak_ptr on spMc, did not increase ref
	std::weak_ptr<MyClass> wpMc = spMc;

	{
		// New shared ptr, no allocation but the ref count has been added
		std::shared_ptr<MyClass> spMc2 = spMc;

		// At the end of this scope, spMc2 will be destroyed so the ref count will decrease
	}

	{
		// Upgrade the weak_ptr to a shared_ptr
		std::shared_ptr<MyClass> spMc2 = wpMc.lock();

		// There is a new shared_ptr in this scope, so the ref count will increase (and decrease at the end of the scope)
	}

	// spMc is now nullptr, there is no more object that has a ref on the instance of MyClass, the object will be deallocated
	spMc = nullptr;

	// The shared_ptr linked to our weak_ptr is destroyed. WIll return true
	bool bExpired = wpMc.expired();

	// spMc2 is empty because expired is true
	std::shared_ptr<MyClass> spMc2 = wpMc.lock();

	//mc has not de deleted so there is a memory leak
    ```

- **unique_ptr** : Works the same a **shared_ptr** but only one person has the ownership of this object

## Pointer, Reference and copy
- 3 possible ways to pass a data to a function (in fact there are 4, but let's keep it simple)

### Copy
- Data is copy
- Easy in case of simple type
- Horrible in case of complex object (copy an entire object, all its members...)
- Update the data in the function won't affect the base object
```cpp
// This is a copy
// If c contains a vector with 1000000 elements in it, you have to copy it
void func(MyClass copy)
{
  // don't affect c
  copy.vector.push_back(10);
}

{
  MyClass c;
  func(c);
}
```

### Pointer
- Pass the address of the object (the pointer is copy but as it is a simple type, it is OK)
- Have to handle the fact that the pointer might be nullptr
- Affect the base object
```cpp
// This is a pointer
void func(MyClass* pointer)
{
  if (!pointer)
  {
    return;
  }
  // Same object as c
  pointer->vector.push_back(10);
}

{
  MyClass c;
  func(&c);
}
```

### Reference
- Equivalent as the pointer (pass a pointer)
- A reference is valid
- Affect the base object
```cpp
// This is a reference
void func(MyClass& reference)
{
  // the reference is always valid, there is no way to check it
  // Same object as c
  pointer.vector.push_back(10);
}

{
  MyClass c;

  // Same syntax than the copy, only the function determines which way is used
  func(c);
}
```

## Container

### Vector
- **Contiguous memory**
- Reserved capacity even if there are less elements in it
- Reallocate if necessary (malloc + copy + free old) => have to reserve the right capacity before use if possible
- Fast to iterate
- Easy to push/pop at the end
- Insert/remove at another place than the end implies to move everything which has a huge impact

### List
- Allocate a new element each time (huge memory fragmentation)
- Pointer to the next element
- Iterate over the entire list is slow (because we have to dereference each pointer to go to the next element)*
- Very easy to add/remove an element ((de)allocate a new element + change few pointers)

### Map
- Key/value system
- Key is hashed and used to found the element (O(1)) => Very easy/fast to find an element
- Iteration can be slow depending of the implementation
- Various implementation possible (tree, various vector...)

### Set
- Various implementation possible
- Unique element in the container
- Value is hashed and used to guarantee uniqueness

### Queue/FIFO
- Various implementation possible (list, vector, rolling cache...)
- First element to get In is the First element to get Out

### Stack/FILO
- Various implementation possible (list, vector, rolling cache...)
- First element to get In is the Last element to get Out

## Inheritance


## Compilation process

To Create the executable/dll, every cpp files must be transformed into binary and then join together. This process works in 3 phasis

### Preprocessor

- Replace every directives (keywords that start with `#` such as `#include`,  `#ifdef`...)
  * `#include` replaces the directive by the content of the corresponding file
    * A same file can be replaced several times unless you use `#pragma once` or `#ifdef`
  * `#ifdef/#ifndef/#if/...` enable some specific sections of code depending of the context
  * ...
- Constants are replaced... everything that can be replaced is replaced
- Get a unique and standalone file that can be compiled

### Compilation

Now each cpp files are standalone, they can be turned into binaries:
- Each line is turned into instruction
- To be able to compile, the compiler needs to know that the corresponding symbol exists and as been declared above (using includes, forward declaration...)
- `static_assert` are resolved at the moment
- Each file is now an `.obj` file
### Link

Now we have lots of `.obj` file we need to join them together and to be sure that all used symobls (function, variables, class)... have a definition
```cpp

// In file A
void Toto()
{
  // ...
}

// In file B
void Toto();

void main()
{
  Toto();
}

```
In this example, the file `A` declares and defines a function named `Toto`.
The file `B` needs the `Toto` function. So we have a forward declaration of that function which indicated that this function exists. So, the compilation phase works because the compiler "knows" that there is a symbol named `Toto` without knowing the definition.
At the link phase, all `.obj` files will be join together:
- If `A` is not part of the project, the A.obj file is not part of the final executble, so the definition of the `Toto` function is missing. We use a symbol that is not defined so we cannot link. There is a link problem.
- If `A` is part of the project, every thing will work as expected.

### Forward declaration

Each time a cpp file is modified, it has to be recompiled (which creates a new `.obj`) and all objects files must be relinked to upodate the executable. However, if a dependency (a header file) of a cpp file is updated, because of the preprocessor pahse, the cpp file must be recompiled. It can become worse if a header includes another header that includes headers... It is not a big problem for tiny project but it becomes a real nightmare for big project. For example, modifying a simple header might recompile the all project of dependencies are not handled proprelly.

To avoid that, it is a good habit to reduce the number of includes inside a header file. If a header file only needs declarations, it don't need a include but a forward declaration
```cpp
class MyClass
{
  public:
    MyMethod(MyOtherClass* other);
}
```
In this example, the easy (but dirty) way to indicate this header that `MyOtherClass` exists is to include the corresponding header file
```cpp
#include "MyOtherClass.h"
```

By doing that, those who will include the `MyClass` header will be able to compile. But we have added a unecessary dependency between `MyClass` and `MyOtherClass`. Because we use a pointer/reference, we only the declaration of the type. SO we can add a forward declaration to reduce the number of dependencies.
```cpp
class MyOtherClass;
```

## Template

- Way to generalize a class, function,... to all types.
- The code of the template will be generated at the moment it is used with a specific type
```cpp
template<typename T>
struct Foo
{
    T bar;
    void doSomething(T param) {/* ... */}
};

// somewhere in a .cpp
Foo<int> f; 
```

will generate
```cpp
struct FooInt
{
    int bar;
    void doSomething(int param) {/* ... */}
};
```
- It is possible to specify so requirments that the type given as a template must fulfill to be used.
- Have to put the definition in the header file.

## Cast

Casting (the right full name is **typecasting**) is a solution to convert a type to another. There are several types of cast which provide various features.
- **static_cast** :
  * Can only cast types that are compatible such as related types, numeric types, class in the sane inheritance hierarchy 
  * Casting is executed at compile time. That means that the check will be performed during conpilation. If it fails, that will result in a compilation error.
  * Because it is processed at compile time, their is no cost during runtime.
  ```cpp
  // Works
  int n = 10;
  double nd = static_cast<double>(n);

  // Does not work - Compilation error
  MyClass c = static_cast<MyClass>(n);
  ```

- **dynamic_cast** : 
  * Mainly used to performed downcasting, cast a base class to a derived one.
  * Such as **static_cast**, it performs small checks at compile time to ensure the cast is theorically possible
  * The real casting is performed at runtime because we need the actual instance to try to cast it to another type. This cast has a cost during runtime execution.
  * Might return nullptr if the cast is not possible.
```cpp
class B {};
class D1 : public B {};
class D2 : public B {};

B* d1 = new D1();

// casted_d1 has a value
D1* casted_d1 = dynanic_cast<D1*>(d1);

// casted_d2 his nullptr
D2* casted_d2 = dynanic_cast<D2*>(d1);
```
- **reinterpret_cast/C Cast**:
  * Perform any conversion without any check even if it has no sense to do it
  * Most dangeurous among all casts, you must know why you are using them.
```cpp

int i = 19;

// That works but... it will boum :)
MyClass* mc = reinterpret_cast<MyClass*>(&i);

// C style version
MyClass* mc = (MyClass*)&i
```
- **const_cast**:
  * Use to add or remove the const attribute from a variable
  * Breaks the entire architecture of your project, so it is not a right solution but sometimes you will need it.
  * If you have to use it, think about it. Is there any other solution? Yes, it is safe to do it (it won't break your program) but the const was there for a reason.
```cpp
const int i = 19;
i = 123; // will not compile

int* pI = &i; // will not compile

int* pI2 = const_cast<int*>(pI);
*pI2 = 123; // Works
```
