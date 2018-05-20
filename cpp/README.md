# C++ Exam

This folder contains the implementation of a Binary Search Tree class. For a brief report refer to **REPORT.md**.

The source code is divided into folders

The *"inc"* folder contains the file **bst.h** whihc in turn contains the declaration of the classes:
   - `Tree`
   - `Node`
   - `Iterator`
   - `ConstIterator`
and the implementation of their shortest members.

The *"src"* folder contains the file **bst.cc** in which the implementation of the functions of the Bst class is listed.

The *"Doxygen"* folder contains the file **doxy.in** which is used to generate the code documentation.

The *"misc"* folder contains the file **misc.h** in which are defined a few ancillary functions and structure used for testing the code.

The main folder contains two files:
- **testmain.cc** which tests all the functions of the code and tests the functionality with a custom comparator.
- **timingmain.cc** which measures the performance of the `find(key)` function before and after balancing a tree.

The **Makefile** provided allows to compile the code. Refer to the following options:

- `make` or `make all` will produce the main executables **testmain** and **timingmain**
- `make debug` will produce the debug version of the main executables **testmain** and **timingmain**
- `make doc` will generate the Doxygen documentation from the source files
- `make clean` will clean all the files that the previous commands will produce
