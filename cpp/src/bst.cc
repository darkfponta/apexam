#include "bst.h"
#include <iostream>
#include <vector>


using namespace std;

template <typename K, typename V, class C>
void Bst<K, V, C>::addSubTree(
	const vector<KVpair>& container, size_t first, size_t last) {

	if (first > last)
		throw InvalidRangeException();
	const size_t mid=(first+last)/2;

	this->insert(container[mid]);

	if (first<mid)
		this->addSubTree(container, first, mid - 1);
	if (mid<last)
		this->addSubTree(container, mid + 1, last);
}


template <typename K, typename V, class C>
typename Bst<K,V,C>::Node* Bst<K, V, C>::insertSingle(Node* cur, const KVpair& pair) {
	const K& key = pair.first;
	const V& val = pair.second;

	Node* temp = cur;

	if (comp(key, temp->pair.first)) { // go left
		if (temp->left == nullptr)
		{
			temp->left.reset(new Node(pair, temp)); return temp->left.get();
		}

		temp = temp->left.get();
	} else if (comp(temp->pair.first, key)) { // go right
		if (temp->right == nullptr)
		{
			temp->right.reset(new Node{ pair, temp }); return temp->right.get();
		}

		temp = temp->right.get();
	} else { temp->pair.second = val; return temp;}

	return insertSingle(temp, pair);
}


template <typename K, typename V, class C>
typename Bst<K,V,C>::Node* Bst<K, V, C>::insertInternal(const KVpair& pair) {
	if (!root)
	{
		root.reset(new Node{ pair });
		return root.get();
	}

	Node* temp = root.get();
	return insertSingle(temp, pair);
}



template <typename K, typename V, class C>
void Bst<K, V, C>::insertSorted(const vector<KVpair>& container, size_t first, size_t last, Node* prev, unique_ptr<Node>* branch) {
	if ((!!prev ^ !!branch) != 0)
		throw InvalidLeafException();

	if (first > last)
		throw InvalidRangeException();

	const size_t mid = (first + last) / 2;

	Node* node = new Node(container[mid]);
	if (prev == nullptr) {
		root.reset(node);
	} else {
		branch->reset(node);
	}
	node->parent = prev;

	if (first<mid)
		this->insertSorted(container, first, mid - 1, node, &node->left);
	if (mid<last)
		this->insertSorted(container, mid + 1, last, node, &node->right);
}


template <typename K, typename V, typename C>
void Bst<K, V, C>::addSubTreeFromSorted(vector<KVpair>& container) {
	// if we come from a vector obtained by an in-order traversal this is just O(n)
	this->insertSorted(container, 0, container.size() - 1, nullptr, nullptr);
}


template <typename K, typename V, typename C>
void Bst<K, V, C>::addSubTreeBalanced(vector<KVpair>& container) {
	sort(container.begin(), container.end(), KVcomp());
	this->addSubTreeFromSorted(container);
}


template <typename K, typename V, class C>
void Bst<K, V, C>::detachParent(const Node* cur) {
	Node* parent = cur->parent;
	if (parent != nullptr)
	{
		if (cur == parent->left.get())
			parent->left.release();
		else if (cur == parent->right.get())
			parent->right.release();
	}
}


template <typename K, typename V, class C>
void Bst<K,V,C>::detailedPrint() const {
	if (this->cbegin() == this->cend())
	{
		cout << "This Bst is empty." << endl; return;
	}

	cout << "Bst contents:" << endl;
	for (auto it = cbegin(); it != cend(); ++it) {
		cout << "================================" << endl;
		cout << "key: " << (*it).getkey() << " value: " << (*it).getvalue() << endl;
		cout << "left: " << ((*it).getleft() == nullptr ? ("---") : std::to_string((*it).getleft()->getkey())) <<
			" right: " << ((*it).getright() == nullptr ? ("---") : std::to_string((*it).getright()->getkey())) <<
			" parent: " << ((*it).getparent() == nullptr ? ("---") : std::to_string((*it).getparent()->getkey())) << endl;
	}
	cout << endl;
}


template <typename K, typename V, class C>
void Bst<K, V, C>::print() const {
	if (this->cbegin() == this->cend())
	{
		cout << "This Bst is empty." << endl; return;
	}

	cout << "Bst contents:" << endl;
	for (auto it = cbegin(); it != cend(); ++it) {
		cout << "key: " << (*it).getkey() << " value: " << (*it).getvalue() << endl;
	}
	cout << endl;
}


template <typename K, typename V, class C>
void Bst<K, V, C>::balance() {
	if (!root) return;

	vector<KVpair> v;
	v.reserve(this->size());
	for (auto it = cbegin(); it != cend(); ++it)
		v.push_back((*it).pair);

	clear();
	//addSubTree(v, 0, v.size()-1);
	addSubTreeFromSorted(v); // faster than ordinary addSubTree since v is sorted.
}


template <typename K, typename V, class C>
void Bst<K, V, C>::erase(const K& key) {
	auto found = (find(key));
	if (found!= end()) {
		Node& n = *found;

		vector<KVpair> container{};

		Node* left = n.left.release();
		if (left) {
			left->parent = nullptr;
			Bst leftTree{ };
			leftTree.root.reset(left);
			for (auto it = leftTree.cbegin(); it != leftTree.cend(); ++it)
				container.push_back((*it).pair);
		}

		Node* right = n.right.release();
		if (right) {
			right->parent = nullptr;
			Bst rightTree{ };
			rightTree.root.reset(right);
			for (auto it = rightTree.cbegin(); it != rightTree.cend(); ++it)
				container.push_back((*it).pair);
		}

		if (root.get() == &n)
			root.release();
		else
			this->detachParent(&n);

		delete &n;

		for (auto it : container)
			insert(it);
	}
	else { throw NotFoundException(); };
}


template <typename K, typename V, class C>
void Bst<K, V, C>::sizeInternal(Node* node, size_t&c) const {
	if (node == nullptr) return;
	++c;
	sizeInternal(node->left.get(), c);
	sizeInternal(node->right.get(), c);
}

template <typename K, typename V, class C>
size_t Bst<K, V, C>::size() const {  // this performs ~2.5x faster than the one below
	size_t c = 0;
	sizeInternal(root.get(), c);
	return c;
}

// alternative implementation
//template<typename K, typename V, class C>
//size_t Bst<K, V, C>::size() const {
//	size_t c = 0;
//	for (ConstIterator it = cbegin(); it != cend(); ++it)
//		c++;
//	return c;
//}


template <typename K, typename V, class C>
size_t Bst<K, V, C>::depth(const Node * const node) const {
	if (!node)
		return 0;

	return max(depth(node->left.get()), depth(node->right.get())) + 1;
}

template <typename K, typename V, class C>
double Bst<K, V, C>::avgdepth() const {
	if (!root)
		return 0;

	size_t curDepth = 1;
	uint64_t acc = curDepth;
	avgdepthInternal(root->left.get(), curDepth, acc);
	avgdepthInternal(root->right.get(), curDepth, acc);
	return (static_cast<double>(acc) / this->size());
}


template <typename K, typename V, class C>
void Bst<K, V, C>::avgdepthInternal(const Node* node, size_t curDepth, uint64_t& acc) const {
	if (!node)
		return;
	curDepth++;
	acc += curDepth;
	avgdepthInternal(node->left.get(), curDepth, acc);
	avgdepthInternal(node->right.get(), curDepth, acc);
}


template <typename K, typename V, class C>
bool Bst<K, V, C>::checkBalanced(const Node * const node) const {
	if (node == nullptr)
		return true;

	size_t depth;
	return checkBalancedInternal(node, depth);
}

template <typename K, typename V, class C>
bool Bst<K, V, C>::checkBalancedInternal(const Node * const node, size_t& depth) const {
	if (node == nullptr) {
		depth = 0;
		return true;
	}

	size_t ldepth, rdepth;
	bool lbalanced = checkBalancedInternal(node->left.get(), ldepth);
	bool rbalanced = checkBalancedInternal(node->right.get(), rdepth);
	depth = max(ldepth, rdepth) + 1;
	return (absdiff(ldepth,rdepth) <= 1) && lbalanced && rbalanced;
}


template <typename K, typename V, class C>
typename Bst<K, V, C>::Iterator Bst<K, V, C>::find(const K& key) const {
	if (!root)
		return Iterator{ nullptr };

	Node *temp = root.get();

	while (comp(temp->pair.first, key) || comp(key, temp->pair.first)) {
		if (comp(key, temp->pair.first)) { // go left
			if (temp->left != nullptr) {
				temp = temp->left.get();
			}
			else { return Iterator{ nullptr }; }
		}
		else { 
			if (temp->right != nullptr) { // go right
				temp = temp->right.get();
			}
			else { return Iterator{ nullptr }; }
		}
	}
	return Iterator{ temp };
}

template <typename K, typename V, class C>
typename Bst<K, V, C>::ConstIterator Bst<K, V, C>::cfind(const K& key) const {
	if (!root)
		return ConstIterator{ nullptr };

	Node *temp = root.get();

	while (comp(temp->pair.first, key) || comp(key, temp->pair.first)) {
		if (comp(key, temp->pair.first)) { // go left
			if (temp->left != nullptr) {
				temp = temp->left.get();
			}
			else { return ConstIterator{ nullptr }; }
		}
		else {
			if (temp->right != nullptr) { // go right
				temp = temp->right.get();
			}
			else { return ConstIterator{ nullptr }; }
		}
	}
	return ConstIterator{ temp };
}


// specialized templates
template class Bst<size_t, size_t>;
template class Bst<size_t, size_t, bool(*)(const size_t&, const size_t&)>;