# Report: Binary Search Tree

In this report the `c++` implementation of a templated Binary Search Tree class `Bst` is described.

A binary search tree is a data structure that allows fast search, insertion and deletion of items
because of its particular binary behavior:

- each node has a key (and eventually a value associated with it) and can have two children nodes,
called left and right which by recursion can be two entire subtrees;
- the key in each node must be greater than to any key stored in the left sub-tree, and less than
  any key stored in the right sub-tree.

The concept can be generalized and adapted for any order relation (total or weak) between the keys.

```
       9
      / \
     5   10
    / \
   2   6

```
> An example of Bst


## The code implementation


### The `Bst` class


#### The `Node` class


#### The `Iterator` and `ConstIterator` classes


#### The `KVstruct` structure


#### The Bst class methods


## Testing


### Run


### Time complexity
