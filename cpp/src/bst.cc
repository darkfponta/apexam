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


// specialized templates
template class Bst<size_t, size_t>;
template class Bst<size_t, size_t, bool(*)(const size_t&, const size_t&)>;
