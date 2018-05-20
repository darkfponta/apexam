# Python interface

This folder contains the implementation of a Pyhton interface for the Binary Search Tree class developed in the **c++** part of this exam. The exposed class is the **Bst** template specialization `Bst<size_t,size_t,std::less<size_t>>`.
The interface has been developed with the **boost::python** library.

For a brief report and a description of the exposed methods refer to **REPORT.md**.

The source code is contained in the file **_bst.cc**.

## Prerequisites

**Python3+** and python3+ compatible **boost::python** headers and libraries are needed.

The interface refers to the **c++** source code in the *cpp* folder of this repository. 
