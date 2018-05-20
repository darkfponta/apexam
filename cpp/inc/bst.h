#ifndef __BST_H__
#define __BST_H__

#include <utility>
#include <iostream>
#include <memory>
#include <algorithm>
#include <vector>
#include "except.h"


using namespace std;

#define KVpair std::pair<K, V>
#define alloc(T) allocator<T>
#define absdiff(a,b) (a > b ? a - b : b - a)

template <typename K, typename V, class C = less<K>>
class Bst {
public:
	class Node {
	protected:
		KVpair pair;
		unique_ptr<Node> right;
		unique_ptr<Node> left;
		Node* parent;
		//C comp;

	public:
		/// constructor. Builds the node given a pair and optionally the parent node.
		Node(const KVpair& v, Node* par = nullptr) : pair{ v },
			right{ nullptr }, left{ nullptr }, parent{ par } {};
			
		friend class Bst;
	};
	
protected:
	/// the root node
	unique_ptr<Node> root;
	/// the local copy of the comparison
	C comp;
	
public:

	// Iterator fwd decl
	class Iterator;
	// ConstIterator fwd decl
	class ConstIterator;
	
	/// default constructor
	Bst(C c = C{}) : comp{ c } {
#ifdef DEBUG
		cout << "Bst(C c = C{})" << endl;
#endif
	}
	
};


/**
 * \brief Iterator class for Bst
 */
template <typename K, typename V, class C>
class Bst<K, V, C>::Iterator {
protected:
	using N = typename Bst<K, V, C>::Node;
	/// current node of the Iterator
	N* current;

public:
	/// Iterator constructor
	Iterator(N* node) : current{ node } {};
};


/**
 * \brief ConstIterator class for Bst
 */
template <typename K, typename V, class C>
class Bst<K,V,C>::ConstIterator :
	public Bst<K,V,C>::Iterator {
	
	using parent = typename Bst<K, V, C>::Iterator;
	using N = typename parent::N;
	
public:
	/// use parent constructor
	using parent::Iterator;
};

#endif // __BST_H__