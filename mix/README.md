# Python interface

This folder contains the implementation of a Pyhton interface for the Binary Search Tree class developed in the **c++** part of this exam. The exposed class is the **Bst** template specialization `Bst<size_t,size_t,std::less<size_t>>`.
The interface has been developed with the **boost::python** library.

For a brief report and a description of the exposed methods refer to **REPORT.md**.

The source code is contained in the file **_bst.cc**.

## Prerequisites

**Python3+** and python3+ compatible **boost::python** headers and libraries are needed.

The interface refers to the **c++** source code in the *cpp* folder of this repository. 

## Build and run

The **Makefile** provided allows to compile the code.

In order to build the interface the user must provide a **Makefile.ini** file containing the definition of the following variables:

- **PYTHON**: the path to the python include files
- **BOOST_PYTHON**: the path to the boost::python library
- **BOOST_PYTHON_LIB**: the linker directive holding the boost::python library name

An example could be:

```makefile
PYTHON = /usr/include/python3.6m/
BOOST_PYTHON = /usr/lib/x86_64-linux-gnu
BOOST_PYTHON_LIB = -lboost_python-py36
```
The file must reside in the same folder as the **Makefile**.
