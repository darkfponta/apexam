# Report: Python Interface

In this report the `c++` implementation of a python interface for the Binary Search Tree class `Bst` developed for the **c++** part of the exam is described.

The interface has been implemented using the boost::python library and it composes the **_bst** python module.

## The challenge

The peculiarity of a c++/python interface stands in the impossibility to completely expose raw pointers since python is a scripted and managed language.

This requires conscious development of the c++ class as well as integration with the boost::python objects since the beginning. The downside of this is a more complex c++ class and forcing users that just want the c++ class to install a quite a heavy library and have all the dependencies properly set.

For this reason the interface has been has been developed *"the hard way"* that is:

- one can compile the bare **c++** class and use it that way without having to deal with other external libraries. To do this refer to the **c++** part of this exam to build and run.
- who wants the python interop, can refer to the **README.md** for the instructions on how to build the interface. This procedure will deal with the compilation of the **c++** class by its own.

This has been achieved by wrapping the **Bst** class in a proper class implementing the additional parts needed in a fully functional python interface.

## The code implementation

The code implementation is carried out in the file **_bst.cc** and consists of two main parts: 
- the first defines all the needed classes and object converters.

- the second part consists of the python module definition, that is the explicit definition of all the objects and functions exported in the interface.

  

### The `_Bst` class

The `_Bst` class inherits directly from the `Bst` class and is therefore **templated** according to the following pattern:

```c++
template <typename K, typename V, class C>
```

where:

- `K` is the type of the keys i.e. the objects on which the ordering is imposed
- `V` is the type of values stored in each node
- `C` is the type of a comparator class which is used to define the ordering of nodes according to their keys;
  in the `Bst` class implementation it is defaulted to `std::less<K>` which will cause nodes with smaller keys to be inserted on the left side while node with greater keys to be inserted on the right side.

#### The additional methods
  
The class has the following **methods**:
- `bst_getitem` the getter that will replace the `operator[](const K& key)` which couldn't be exposed directly.
- `bst_setitem` the setter that will replace the `operator[](const K& key)` in assignment statements.

The class also includes the definition of the following non inherited methods:
- `begin()` 
- `end()` 

In fact, the **Iterator** and **ConstIterator** iterators defined in the **Bst** class used and returned **Node** objects when dereferenced. To avoid to expose yet another class in the interface, write the object converters and deal with their pointer members, an *ad-hoc* iterator (**KVIterator**) has been implemented.



#### The `KVIterator` class

This **public** class defines the iterator object that allows proper traversing of the tree without having to deal with **Node** objects and related pointers.
Being an inner class it inherits the templated pattern of the tree it is part of.

Moreover it inherits from both the **Iterator** class and the **std::iterator**, thus allowing the interoperability with python:

```c++
class _Bst<K, V, C>::KVIterator 
	:	public Bst<K, V, C>::Iterator,
		public std::iterator<std::forward_iterator_tag, const std::pair<K,V>>
```

The class implements the needed operators:

- the prefix sum operator `KVIterator operator++()`
- the postfix sum operator `KVIterator operator++(int)`
- the dereference operator `const std::pair<K,V>& operator*() const`

As can be seen the iterator deals with **std::pair** objects instead of **Node** objects: the operators here defined take advantage of the corresponding operators of the **Iterator** class and use the *getpair()* function to expose the key-value pair to the python interface.



### The object converters

The following structures have been implemented:

- **std_pair_to_tuple**: converts std::pair to a python tuple of size 2

- **pair_from_python_tuple**: converts a python tuple of size 2 to a std::pair

- **vector_from_python_list**: converts a python list to a std::vector

  

These structures implement the methods required by the boost::python library (e.g. *convert*, *convertible*, *construct*) to properly convert c++ and python objects back and forth.



### The custom exception wrappers

To work in python, c++ exceptions need to be mangled properly. For this reason the following functions have been implemented:

- `template <class T> void explainException(const T& x)` which gets the string message from the c++ exception and define a proper python error string with it.
- `template <class T> void throwException()` which is used to throw the exception from the python code.



### The Interface definition

The python module is declared as

```c++
BOOST_PYTHON_MODULE(_bst)
```

this means that the module is called **_bst** and this name should be used when referring to it from python code. Between the braces following this definition the following take place:

- the initialization of the three object converters  **std_pair_to_tuple**, **pair_from_python_tuple**, **vector_from_python_list**. The constructor defined for these structures will add the converters to a list used by boost::python to call the right converter at the right time.
- the class converter for `std::vector<std::pair<size_t,size_t>>` is defined. This will implicitly refer to the converters initialized in the previous point to convert python lists of tuples to vectors of pairs which are used in the Bst class.

- using `register_exception_translator<T>` the exception mangling functions for the three custom exceptions are defined.
- finally the `_Bst<size_t,size_t,std::less<size_t>>` class is exported. The name used is **Bst_sizet**. The exported functions are the following:
  - `init<>`, default constructor as `Bst::Bst(C c = C{})`
  - `init<std::pair<size_t,size_t>>())`, the constructor taking a `std::pair` to define the root node as `Bst(std::pair<size_t,size_t> pair, C c = C{})`.
  - `init<std::vector<std::pair<size_t,size_t>>>())`, the constructor building a tree from the given vector as `Bst(const vector<KVpair>& container, C c = C{})`.
    In the python code the input container is a list of tuples.
  - `print`, calls `void Bst::print() const`.
  - `detailedPrint `, calls `void Bst::detailedPrint() const`.
  - `addSubTree`, calls
  ```c++
    void Bst::addSubTree(const std::vector<std::pair<size_t,size_t>>& container, size_t first, size_t last)
  ```
  - `addSubTreeBalanced`, calls
  ```c++
    void Bst::addSubTreeBalanced(std::vector<std::pair<size_t,size_t>>& container)
  ```
  - `insert`, calls `void Bst::insert(const std::pair<size_t,size_t>& pair)`.
  - `size`, calls `size_t Bst::size() const`.
  - `depth`, calls
  ```c++
    size_t Bst::depth() const
  ```
  The other overload is discarded because it deals with pointers.
  - `avgdepth`, calls `double Bst::avgdepth() const`.
  - `balance`, calls `void Bst::balance()`.
  - `clear`, calls `void Bst::clear()`.
  - `erase`, calls `void Bst::erase(const size_t& key)`.
  - `checkBalanced`, calls
  ```c++
    bool Bst::checkBalanced() const
  ```
  The other overload is discarded because it deals with pointers.
  - `__iter__`, defines the iterator range using `_Bst::KVIterator`.
  - `__getitem__`, calls `size_t _Bst::bst_getitem(size_t key)`.
  - `__setitem__`, calls `void _Bst::bst_setitem(size_t key, size_t value))`.


## The code example

Just as **testingmain.cc** the code revolves around a tree built with this unordered array of keys (10, 79, 13, 80, 60, 50).

The example file is **bst.py**.

The main functions described above are tested and work as intended.

```
       13                      50
      /  \      balance()     /   \
     10   79       ==>      10     79
          / \                \    /  \
         60  80              13  60   80
         /				   
        50				   
```
> Graphical representation of one of the operations performed during the tests.
