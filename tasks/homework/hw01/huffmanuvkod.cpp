#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

struct Node {
	char data;
	Node * left;
	Node * right;

	Node(char newData = 0) {
		data = newData;
		left = nullptr;
		right = nullptr;
	}
};

void insertNode(Node * at, queue<char> & positions, queue<char> & characters) {
	if (positions.front() == 0) {
		at->left = new Node;
		positions.pop();
		insertNode(at->left, positions, characters);
	} else {
		at->left = new Node(characters.front());
		positions.pop();
		characters.pop();
	}

	if (positions.front() == 0) {
		at->right = new Node;
		positions.pop();
		insertNode(at->right, positions, characters);
	} else {
		at->right = new Node(characters.front());
		positions.pop();
		characters.pop();
	}
}

void printTree(Node * from) {

	if (from->right != nullptr) {
		printTree(from->right);
	}

	if (from->left != nullptr) {
		printTree(from->left);
	}

	if (from->data != 0) {
		cout << from->data;
	}
}

void test() {
	Node * root = new Node;
	queue<char> positions;
	queue<char> characters;

	positions.push(1);
	positions.push(0);
	positions.push(0);
	positions.push(1);
	positions.push(1);
	positions.push(0);
	positions.push(1);
	positions.push(1);

	characters.push('o');
	characters.push('t');
	characters.push('c');
	characters.push('K');
	characters.push('l');

	insertNode(root, positions, characters);

	printTree(root);
	cout << endl;
}

bool decompressFile(const char * inFileName, const char * outFileName) {
	// todo
	return false;
}

bool compressFile(const char * inFileName, const char * outFileName) {
  	// keep this dummy implementation (no bonus) or implement the compression (bonus)
	return false;
}

#ifndef __PROGTEST__
bool identicalFiles(const char * fileName1, const char * fileName2) {
	// todo
	return false;
}

int main(void) {

	test();

	/*
	assert(decompressFile("tests/test0.huf", "tempfile"));
	assert(identicalFiles("tests/test0.orig", "tempfile"));

	assert(decompressFile("tests/test1.huf", "tempfile"));
	assert(identicalFiles("tests/test1.orig", "tempfile"));

	assert(decompressFile("tests/test2.huf", "tempfile"));
	assert(identicalFiles("tests/test2.orig", "tempfile"));

	assert(decompressFile("tests/test3.huf", "tempfile"));
	assert(identicalFiles("tests/test3.orig", "tempfile"));

	assert(decompressFile("tests/test4.huf", "tempfile"));
	assert(identicalFiles("tests/test4.orig", "tempfile"));

	assert(!decompressFile("tests/test5.huf", "tempfile"));


	assert(decompressFile("tests/extra0.huf", "tempfile"));
	assert(identicalFiles("tests/extra0.orig", "tempfile"));

	assert(decompressFile("tests/extra1.huf", "tempfile"));
	assert(identicalFiles("tests/extra1.orig", "tempfile"));

	assert(decompressFile("tests/extra2.huf", "tempfile"));
	assert(identicalFiles("tests/extra2.orig", "tempfile"));

	assert(decompressFile("tests/extra3.huf", "tempfile"));
	assert(identicalFiles("tests/extra3.orig", "tempfile"));

	assert(decompressFile ( "tests/extra4.huf", "tempfile" ) );
	assert(identicalFiles ( "tests/extra4.orig", "tempfile" ) );

	assert(decompressFile("tests/extra5.huf", "tempfile"));
	assert(identicalFiles("tests/extra5.orig", "tempfile"));

	assert(decompressFile("tests/extra6.huf", "tempfile"));
	assert(identicalFiles("tests/extra6.orig", "tempfile"));

	assert(decompressFile("tests/extra7.huf", "tempfile"));
	assert(identicalFiles("tests/extra7.orig", "tempfile"));

	assert(decompressFile("tests/extra8.huf", "tempfile"));
	assert(identicalFiles("tests/extra8.orig", "tempfile"));

	assert(decompressFile("tests/extra9.huf", "tempfile"));
	assert(identicalFiles("tests/extra9.orig", "tempfile"));
	return 0;
	*/
}
#endif /* __PROGTEST__ */
