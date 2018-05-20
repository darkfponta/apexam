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
		/// copy constructor
		Node(const Node& b) : pair{ b.pair }, parent{ nullptr } {
#ifdef DEBUG
			std::cout << "Node copy ctor" << std::endl;
#endif
			if (b.left) {
				left.reset(new Node{ *(b.left.get()) });
				left->parent = this;
			}
			if (b.right) {
				right.reset(new Node{ *(b.right.get()) });
				right->parent = this;
			}
		}
		
		/// gets the key associated with the node
		const K& getkey() const { return pair.first; };
		/// gets the value associated with the node
		const V& getvalue() const { return pair.second; };
		/// gets the key-value pair associated with the node
		const KVpair& getpair() const { return pair; };

		/// gets the left node associated with the node
		const Node* getleft() const { return this->left.get(); };
		/// gets the right node associated with the node
		const Node* getright() const { return this->right.get(); };
		/// gets the parent node associated with the node
		const Node* getparent() const { return this->parent; };

		friend class Bst;
	};
	
protected:
	/// the root node
	unique_ptr<Node> root;
	/// the local copy of the comparison
	C comp;
	
	/// moves down the tree from the given node always picking the left branches
	static void ffwd(Node*& n) {
		while (n->left != nullptr)
			n = n->left.get();
	}
	
	/// adds a new node, wrapped by insert(const KVpair& pair)
	Node* insertInternal(const KVpair& pair);

	/// adds a new node, used by insertInternal(const KVpair& pair)
	Node* insertSingle(Node* cur, const KVpair& pair);

	/// removes the pointer to the given node from its parent
	void detachParent(const Node* cur);
	
	/// nested structure that defines the operator used for comparing two key-value pairs
	struct KVcomp {
		C comp;
		bool operator()(const KVpair& lhs, const KVpair& rhs) {
			return comp(lhs.first, rhs.first);
		}
		KVcomp(C c = C{}) : comp{ c } {};
		friend class Bst;
	};

	
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
	
	/// constructor with root
	Bst(KVpair pair, C c = C{}) : root{ new Node{pair,nullptr} }, comp{ c } {
#ifdef DEBUG
		cout << "Bst(KVpair pair, C c = C{})" << endl;
#endif
	}

	/// constructor with input vector
	Bst(const vector<KVpair>& container, C c = C{}) : comp{ c } {
#ifdef DEBUG
		cout << "Bst(const vector<KVpair>& container, C c = C{})" << endl;
#endif
		addSubTree(container, 0, container.size() - 1);
	}

	/// copy ctor
	Bst(const Bst& tree) {
#ifdef DEBUG
		std::cout << "copy ctor Bst(const Bst& tree)" << std::endl;
#endif 
		if (tree.root != nullptr)
			root.reset(new Node{ *(tree.root.get()) });
	}

	/// copy assignment
	Bst& operator=(const Bst& tree) {
#ifdef DEBUG
		std::cout << "copy assignment Bst& operator=(const Bst& tree)" << std::endl;
#endif 
		if (tree.root != nullptr)
			root.reset(new Node{ *(tree.root.get()) });
		return *this;
	}

	/// move ctor
	Bst(Bst&& tree) noexcept : root{ move(tree.root) }, comp{ move(tree.comp) } {
#ifdef DEBUG
		std::cout << "move ctor Bst(Bst&& tree)" << std::endl;
#endif 
	}

	/// move assignment
	Bst& operator=(Bst&& tree) noexcept {
#ifdef DEBUG
		std::cout << "move assignment Bst& operator=(Bst&& tree)" << std::endl;
#endif 
		root = move(tree.root);
		comp = move(tree.comp);
		return *this;
	}

	/// destructor
	~Bst() {
#ifdef DEBUG
		std::cout << "destructor ~Bst" << std::endl;
#endif 
		this->clear();
	};

	/// Gets the root node
	const Node* getRoot() const { return root.get(); };

	/// Prints all the nodes in the format key: value
	void print() const;
	/// Prints all the contents of the Bst including the connected nodes for each node
	void detailedPrint() const;

	/// Adds to the tree all the vector elements in the given range
	void addSubTree(const vector<KVpair>& container, size_t first, size_t last);
	/// adds a new node
	void insert(const KVpair& pair) {
		insertInternal(pair); // ignore returned pointer
	}
	
	/// computes the number of nodes
	size_t size() const;
	/// computes the maximum depth of the tree
	size_t depth() const;
	/// computes the average depth of the tree
	double avgdepth() const;

	/// Balances the Bst
	void balance();
	
	/// Clears the Bst
	void clear() { root.reset(); };
	/// deletes the node with the given key
	void erase(const K& key);
	
	/// checks whether the tree is balanced
	bool checkBalanced() const;
	/// checks whether the subtree rooted in the given node is balanced
	bool checkBalanced(const Node * const node) const;

	/// finds a the node with the given key,
	/// if the node is not found, the function returns Iterator{nullptr}
	Iterator find(const K& key) const;
	
	/**
	 * \brief begin function of the iterator
	 * \return the leftmost node of the Bst
	 */
	Iterator begin() {
		if (!root)
			return Iterator{ nullptr };
		Node* temp = root.get();
		ffwd(temp);
		return Iterator{ temp };
	}

	/**
	* \brief end function of the iterator
	* \return Iterator of nullptr
	*/
	Iterator end() { return Iterator{ nullptr }; }
	
	
	/**
	* \brief begin function of the const iterator
	* \return the leftmost node of the Bst
	*/
	ConstIterator cbegin() const {
		if (!root)
			return ConstIterator{ nullptr };
		Node* temp = root.get();
		ffwd(temp);
		return ConstIterator{ temp };
	}

	/**
	* \brief end function of the const iterator
	* \return Iterator of nullptr
	*/
	ConstIterator cend() const { return ConstIterator{ nullptr }; }

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

	/// dereferences the iterator
	N& operator*() const { return *current; };
	
	/// moves the iterator forward
	Iterator& operator++() {
		if (current != nullptr) {
			if (current->right != nullptr) {
				current = current->right.get();
				ffwd(current);
			}
			else {
				N* temp = current->parent;
				while (temp != nullptr && current == temp->right.get()) {
					current = temp;
					temp = temp->parent;
				}
				current = temp;
			}
		}
		return *this;
	}

	/// boolean operator that checks if two nodes are equal
	bool operator==(const Iterator& rhs) { return this->current == rhs.current; };
	/// boolean operator that checks if two nodes are different
	bool operator!=(const Iterator& rhs) { return !this->operator==(rhs); };
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
	/// derefereces the iterator
	const N& operator*() const { return parent::operator*(); };
};

#endif // __BST_H__