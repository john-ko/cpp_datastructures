#ifndef AVL_HPP
#define AVL_HPP

#include <memory>
#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <math.h>
namespace AVL {

template<typename T>
struct node {
	std::shared_ptr<node<T>> left;
	std::shared_ptr<node<T>> right;
	T value;
	signed char height;
	node(T v): value(v), height(-1), left(nullptr), right(nullptr){}
};

template<typename T>
class BinaryTree {

private:
	std::shared_ptr<node<T>> root;
	bool (*comparator)(T, T);
	void _insert(std::shared_ptr<node<T>> &, std::shared_ptr<node<T>> &);
	void print(const std::shared_ptr<node<T>> &) const;
	signed char height(const std::shared_ptr<node<T>>&) const;
	void rotateRight(std::shared_ptr<node<T>>&);
	void rotateLeft(std::shared_ptr<node<T>>&);

	void printBFS(std::shared_ptr<node<T>> &);

public:

	BinaryTree();
	

	// compare
	// 
	// compares typename T a, and T b
	// based on the function pointer bool(*comparator)(T, T)
	// 
	// idea behind this was to use custom function for custom objects
	// but thinking about it, it would be better just to overload the
	// greater than, less than operators
	bool compare(T, T);

	// setComparator
	// sets the comparator function pointer
	// my question is, what if the level where the function is declared
	// goes out of scope?
	// 
	// def b() {
	// 	setComparator(a);
	// }
	// 
	// def a (T a, T b) {}
	// 
	// def main() {
	// 	b();
	// 	try to do more comparator stuff here?
	// }
	// 
	void setComparator(bool (*f)(T,T));
	void insert(T);
	void print() const;
	void printBFS();
};

template<typename T>
BinaryTree<T>::BinaryTree() {
	// sanity, even though on test its nullptr
	comparator = nullptr;
}

///////////////////////
// PRIVATE FUNCTIONS //
///////////////////////

template<typename T>
void BinaryTree<T>::_insert(std::shared_ptr<node<T>> &tree, std::shared_ptr<node<T>> &node) {

	if (tree == nullptr) {
		tree = node;
		return;
	}

	if (tree->value < node->value) {
		// go right
		_insert(tree->right, node);
	} else {
		_insert(tree->left, node);
	}

	signed char delta = height(tree->right) - height(tree->left);
	if (delta == 2) {
		rotateLeft(tree);
	} else if (delta == -2) {
		rotateRight(tree);
	}

	tree->height = std::max(height(tree->left), height(tree->right)) + 1;
}

template<typename T>
void BinaryTree<T>::print(const std::shared_ptr<node<T>> &node) const{
	if(node == nullptr)
		return;

	if (node->left)
		print(node->left);

	std::cout << "[" << node->value << " ]";

	if (node->right)
		print(node->right);

}

template <typename T>
signed char BinaryTree<T>::height(const std::shared_ptr<node<T>>& node) const {
	if (! node)
		return -1;

	return node->height;
}

template <typename T>
void BinaryTree<T>::rotateRight(std::shared_ptr<node<T>>& node) {
	std::shared_ptr<AVL::node<T>> temp;
	temp = node->left;
	node->left = temp->right;
	temp->right = node;
	node = temp;
}

template <typename T>
void BinaryTree<T>::rotateLeft(std::shared_ptr<node<T>>& node) {
	std::shared_ptr<AVL::node<T>> temp;
	temp = node->right;
	node->right = temp->left;
	temp->left = node;
	node = temp;
}

template <typename T>
void BinaryTree<T>::printBFS(std::shared_ptr<node<T>> &current) {
	std::queue< std::shared_ptr<node<T>> > queue;
	std::shared_ptr<node<T>> currentNode;
	std::shared_ptr<node<T>> nullNode(new AVL::node<T>(0));

	queue.push(current);
	int i = 1;
	int c = 0;
	while(!queue.empty()) {

		if (c != (int)log2(i)) {
			std::cout << std::endl;
		}

		if (queue.front()->value != 0) {
			if (queue.front()->left) {
				queue.push(queue.front()->left);
			} else {
				queue.push(nullNode);
			}
			if (queue.front()->right) {
				queue.push(queue.front()->right);
			} else {
				queue.push(nullNode);
			}
		}

		std::cout << " " << queue.front()->value;

		if (c != (int)log2(i)) {
			c = (int)log2(i);
		}

		i++;

		queue.pop();
	}

}





////////////////////
// PUBLIC METHODS // 
////////////////////

template <typename T>
void BinaryTree<T>::setComparator(bool (*f)(T,T)) {
	comparator = f;
}

template <typename T>
bool BinaryTree<T>::compare(T a, T b) {
	return comparator(a, b);
}

template <typename T>
void BinaryTree<T>::insert(const T value) {
	std::shared_ptr<node<T>> node(new AVL::node<T>(value));
	_insert(root, node);
}

template <typename T>
void BinaryTree<T>::print() const {
	print(root);
	std::cout << std::endl;
}

template <typename T>
void BinaryTree<T>::printBFS() {
	printBFS(root);
}

} // end namespace AVL
#endif