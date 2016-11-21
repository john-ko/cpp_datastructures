#include <iostream>
#include "avl.hpp"
#include "math.h"

using namespace std;


bool testComparatorFunction(int a, int b) {
	return a < b;
}

void testComparator() {
	cout << endl << "Test Comparator" << endl;
	AVL::BinaryTree<int> tree;

	cout << "  Adding test Comparator function";
	tree.setComparator(testComparatorFunction);

	cout << endl << "  comparing (1, 2) should be True: ";
	if (tree.compare(1, 2)) {
		cout << "passed";
	} else {
		cout << "failed";
	}

	cout << endl << "  comparing (2, 1) should be False: "; 
	if (tree.compare(2, 1) == false) {
		cout << "passed";
	} else {
		cout << "failed";
	}
}

void testNode() {
	cout << endl << "Node Creation:" << endl;
	AVL::node<int> a(1);

	cout << "  height should be -1: ";
	if(a.height == -1) {
		cout << "passed";
	} else {
		cout << "failed";
	}

	cout << endl << "  value should be 1 : ";

	if (a.value == 1) {
		cout << "passed";
	} else {
		cout << "failed";
	}

	cout << endl << "  left should be nullptr  : ";
	if (a.left == nullptr) {
		cout << "passed";
	} else {
		cout << "failed";
	}

	cout << endl << "  right should be nullptr : ";
	if (a.right == nullptr) {
		cout << "passed";
	} else {
		cout << "failed";
	}

	cout << endl;
}

void testInsert() {
	cout << endl << "Tree Insertion:" << endl;

	AVL::BinaryTree<int> tree;

	tree.insert(4);
	tree.insert(3);
	tree.insert(2);
	tree.print();
	tree.insert(1);

	tree.print();
	tree.printBFS();
}

void testLog() {
	for(int i = 0; i < 9; ++i) {
		std::cout << i << " : " << (int)log2(i) << std::endl;
	}
}

int main() {
	cout << "Included avl.hpp" << std::endl;

	testNode();
	testComparator();
	testInsert();
	// testLog();
	return 0;
}