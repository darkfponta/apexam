# Report: Python Interface

In this report the `c++` implementation of a python interface for the Binary Search Tree class `Bst` developed for the **c++** part of the exam is described.

The interface has been implemented using the boost::python library and it composes the **_bst** python module.

## The challenge

The peculiarity of a c++/python interface stands in the impossibility to completely expose raw pointers since python is a scripted and managed language.

This requires conscious development of the c++ class as well as integration with the boost::python objects since the beginning. The downside of this is forcing the users who just want the c++ class to install a quite a heavy library and have all the dependencies properly set.


## The code implementation


### The object converters


### The custom exception wrappers


## The code example
