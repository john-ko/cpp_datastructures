#ifndef WAVL_HPP
#define WAVL_HPP

struct WAVLNode {
	int rank;
	int value;
	WAVLNode* left;
	WAVLNode* right;
};

#endif