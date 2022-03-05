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
#include <bitset>
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

class EncodeTree {
	Node * root;
	size_t size;

	queue<bool> creationPositions;
	queue<char> creationCharacters;

	void createNode(Node* at) {
		if (creationPositions.front() == false) {
			at->left = new Node;
			size++;
			creationPositions.pop();
			createNode(at->left);
		}
		else {
			at->left = new Node(creationCharacters.front());
			size++;
			creationPositions.pop();
			creationCharacters.pop();
		}

		if (creationPositions.front() == false) {
			at->right = new Node;
			size++;
			creationPositions.pop();
			createNode(at->right);
		}
		else {
			at->right = new Node(creationCharacters.front());
			size++;
			creationPositions.pop();
			creationCharacters.pop();
		}
	}

	void printRec(Node * from) {
		if (from->right != nullptr) {
			printRec(from->right);
		}

		if (from->left != nullptr) {
			printRec(from->left);
		}

		if (from->data != 0) {
			cout << from->data << endl;
		}
	}

	public:
	void createNewTree(queue<bool> & positions, queue<char> & characters) {
		root = new Node;
		if (positions.front() == true) {
			root->data = characters.front();
			return;
		} else {
			positions.pop();

			creationPositions = positions;
			creationCharacters = characters;

			createNode(root);
		}
	}

	void printTree() {
		printRec(root);
	}
};

void readBits(const fstream & from, queue<bool> to) {
	from.get(c);

	for (int i = 7; i >= 0; i--) {
		to.push((c >> i) & 1 ? true : false);
	}
}

void readTree() {
	
}

bool decompressFile(const char * inFileName, const char * outFileName) {

	fstream ifilestream(inFileName, ios::in | ios::binary);

	if (ifilestream.fail()) {
		return false;
	}

	bool readingChars = false;
	int numberOfNodes = 0;
	int numberOfLists = 0;
	queue<bool> positions;
	queue<char> characters;

	queue<bool> bits;

	char c;
	while(numberOfLists != numberOfNodes + 1) {
		if (bits.size() < 8) {
			ifilestream.get(c);

			for (int i = 7; i >= 0; i--) {
				bits.push((c >> i) & 1 ? true : false);
			}
		}
		if (readingChars && bits.size() >= 8) {
			bitset<8> byte;
			for (int i = 7; i >= 0; i--) {
				byte[i] = bits.front();
				bits.pop();
			}
			characters.push(static_cast<char>(byte.to_ulong()));
			readingChars = false;
		} else if (!readingChars) {
			positions.push(bits.front());	
			bits.pop();
			if (positions.back() == false) {
				numberOfNodes++;
			} else {
				numberOfLists++;
				readingChars = true;
			}
		}
	}
	characters.push('h');
	
	EncodeTree tree;

	tree.createNewTree(positions, characters);
	tree.printTree();

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

	decompressFile("tests/test0.huf", "tempfile");

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
