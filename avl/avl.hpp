#ifndef AVL_HPP
#define AVL_HPP

#include <memory>
#include <iostream>
#include <stack>
#include <vector>
namespace AVL {

template<typename T>
struct node {
	T value;
	signed char height;
	std::unique_ptr<node<T>> left;
	std::unique_ptr<node<T>> right;
	
	node(T v): value(v), height(-1), left(nullptr), right(nullptr){}
};

template<typename T>
class BinaryTree {

private:
	std::unique_ptr<node<T>> root;
	bool (*comparator)(T, T);
	void _insert(std::unique_ptr<node<T>> &, std::unique_ptr<node<T>> &);
	void print(const std::unique_ptr<node<T>> &) const;
	signed char height(const std::unique_ptr<node<T>>&) const;
	void rotateRight(std::unique_ptr<node<T>>&);
	void rotateLeft(std::unique_ptr<node<T>>&);

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
};

template<typename T>
BinaryTree<T>::BinaryTree() {
	// sanity, even though on test its nullptr
	comparator = nullptr;
}

// private functions
template<typename T>
void BinaryTree<T>::_insert(std::unique_ptr<node<T>> &tree, std::unique_ptr<node<T>> &node) {

	if (tree == nullptr) {
		tree = std::move(node);
		return;
	}

	if (tree->value < node->value) {
		// go right
		_insert(tree->right, node);
	} else {
		_insert(tree->left, node);
	}

	signed char delta = height(tree->right) - height(tree->left);
	std::cout << "DELTA [" << static_cast<int>(delta) << "]" << std::endl;
	if (delta == 2) {
		rotateLeft(tree);
	} else if (delta == -2) {
		rotateRight(tree);
	}

	tree->height = std::max(height(tree->left), height(tree->right)) + 1;
}

template<typename T>
void BinaryTree<T>::print(const std::unique_ptr<node<T>> &node) const{
	if(node == nullptr)
		return;

	if (node->left)
		print(node->left);

	std::cout << "[" << node->value << " " << &node << " ]";

	if (node->right)
		print(node->right);

}

template <typename T>
signed char BinaryTree<T>::height(const std::unique_ptr<node<T>>& node) const {
	if (! node)
		return -1;

	return node->height;
}

template <typename T>
void BinaryTree<T>::rotateRight(std::unique_ptr<node<T>>& node) {
	std::unique_ptr<AVL::node<T>> temp;
	temp = std::move(node->left);
	node->left = std::move(temp->right);
	temp->right = std::move(node);
	node = std::move(temp);
}

template <typename T>
void BinaryTree<T>::rotateLeft(std::unique_ptr<node<T>>& node) {
	std::unique_ptr<AVL::node<T>> temp;
	temp = std::move(node->right);
	node->right = std::move(temp->left);
	temp->left = std::move(node);
	node = std::move(temp);
}



// PUBLIC
// 

template<typename T>
void BinaryTree<T>::setComparator(bool (*f)(T,T)) {
	comparator = f;
}

template <typename T>
bool BinaryTree<T>::compare(T a, T b) {
	return comparator(a, b);
}

template<typename T>
void BinaryTree<T>::insert(const T value) {
	std::unique_ptr<node<T>> node(new AVL::node<T>(value));
	std::cout << "node address:" << &node << std::endl;
	_insert(root, node);
}

template <typename T>
void BinaryTree<T>::print() const {
	print(root);
	std::cout << std::endl;
}

} // end namespace AVL
#endif