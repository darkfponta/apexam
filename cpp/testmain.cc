#include <iostream>
#include <vector>
#include "bst.h"
#include "misc.h"

#define tpair std::pair<size_t,size_t>
#define ttree Bst<size_t,size_t>

int main() {

	cout << "=================================" << endl;
	cout << "Testing with size_t" << endl;
	cout << "=================================" << endl << endl;
	{
		cout << "Creating empty tree..." << endl;
		ttree tree{ };
		tree.print();
		const unique_ptr<vector<tpair>> v{ new vector<tpair>{
			make_pair(10,10),
			make_pair(79,79),
			make_pair(13,13),
			make_pair(80,80),
			make_pair(60,60),
			make_pair(50,50)
		} };

		cout << "Adding vevctor using addsubtree..." << endl;
		tree.addSubTree(*v, 0, v->size() - 1);
		tree.print();
		tree.detailedPrint();
		cout << "max tree depth before balance: " << tree.depth() << endl;
		cout << "avg tree depth before balance: " << tree.avgdepth() << endl;
		cout << "Balancing the tree..." << endl;
		tree.balance();
		tree.print();
		cout << "tree depth after balance: " << tree.depth() << endl;
		cout << "avg tree depth after balance: " << tree.avgdepth() << endl;

		cout << "non const assign subscript with non existent key creates element 'tree[11]=11'" << endl;
		tree[11] = 11;
		cout << "query newly added node with non const subscript: tree[11]=" << tree[11] << endl;
		cout << "using insert to change the value of existing node" << endl;
		tree.insert(make_pair(11, 100));
		cout << "tree[11]=" << tree[11] << endl;

		cout << "erasing tree[10]..." << endl;
		tree.erase(10);
		tree.print();


		cout << "Copy move semantics" << endl;
		cout << "copy ctor the tree..." << endl;
		ttree copiedtree{ tree };
		cout << "copied tree content" << endl;
		copiedtree.print();
		cout << "depth: " << copiedtree.depth() << endl;

		cout << "copying the tree with copy assignment..." << endl;
		ttree copiedtree2;
		copiedtree2 = copiedtree;

		cout << "making the tree unbalanced erasing the left subtree..." << endl;
		copiedtree.erase(11);
		copiedtree.erase(13);
		cout << "is tree balanced? " << (copiedtree.checkBalanced() ? "true" : "false") << endl;

		const ttree::Node* node = copiedtree.getRoot()->getright();
		cout << "is right subtree balanced? " << (copiedtree.checkBalanced(node) ? "true" : "false") << endl;

		cout << "clearing tree" << endl;
		copiedtree.clear();
		copiedtree.print();

		cout << endl << "Creating const tree with one element..." << endl;
		const ttree ctree{ make_pair(10,10) };
		ctree.print();
		try {
			cout << "Querying non existent node with key=11" << endl;
			std::cout << ctree[11];
		}
		catch (const exception& ex) {
			cout << ex.what() << endl;
		}

		cout << endl << "Moving to other objects..." << endl;
		cout << "by move ctor..." << endl;
		ttree movedtree{ std::move(tree) };
		movedtree.print();

		cout << "by move assignment..." << endl;
		ttree movedtree2;
		movedtree2 = ttree{ make_pair(12,12) };
		movedtree2.print();

		cout << endl << "creating tree with custom comparison function" << endl;

		bool(*mycompPtr)(const size_t&, const size_t&) = mycomparison;
		Bst<size_t, size_t, bool(*)(const size_t&, const size_t&)> comptree{mycompPtr};

		cout << "Adding vevctor using addsubtree..." << endl;
		comptree.addSubTree(*v, 0, v->size() - 1);
		comptree.print();

		cout << "calling destructor..." << endl;
	}
	return 0;
}