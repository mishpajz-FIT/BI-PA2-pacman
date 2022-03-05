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

	bool hasData() {
		if (left == nullptr && right == nullptr) {
			return true;
		}
		return false;
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

	bool extractCharFromTree(deque<bool>& bits, char & to) const {
		size_t bitIter = 0;
		Node * nodeIter = root;
		while (true) {
			if (bits.size() < bitIter + 1) {
				return false;
			}

			if (nodeIter->hasData()) {
				to = nodeIter->data;
				for (size_t i = 0; i < bitIter; i++) {
					bits.pop_front();
				}
				return true;
			}

			if (bits.at(bitIter) == true) {
				if (nodeIter->right == nullptr) {
					return false;
				}
				nodeIter = nodeIter->right;
			} else {
				if (nodeIter->left == nullptr) {
					return false;
				}
				nodeIter = nodeIter->left;
			}
			bitIter++;
		}
	}

	void printTree() {
		printRec(root);
	}
};

void readBits(ifstream& from, deque<bool>& to) {
	char c;

	from.get(c);

	for (int i = 7; i >= 0; i--) {
		to.push_back((c >> i) & 1 ? true : false);
	}
}

void readTreeFromFile(EncodeTree& tree, ifstream& from, deque<bool>& bits) {
	int numberOfNodes = 0;
	int numberOfLists = 0;

	queue<bool> positions;
	queue<char> characters;

	bool readingChars = false;

	while (true) {
		if (bits.size() < 8) {
			readBits(from, bits);
		}
		if (readingChars && bits.size() >= 8) {
			bitset<8> byte;
			for (int i = 7; i >= 0; i--) {
				byte[i] = bits.front();
				bits.pop_front();
			}
			characters.push(static_cast<char>(byte.to_ulong()));
			readingChars = false;

			if (numberOfLists == numberOfNodes + 1) {
				break;
			}
		}
		else if (!readingChars) {
			positions.push(bits.front());
			bits.pop_front();
			if (positions.back() == false) {
				numberOfNodes++;
			}
			else {
				numberOfLists++;
				readingChars = true;
			}
		}
	}
	tree.createNewTree(positions, characters);
}

bool readCharsFromFile(const EncodeTree& tree, ifstream& from, ofstream& to, deque<bool>& bits) {
	unsigned long remainingToRead = 0;
	bool lastChunk = false;

	while (true) {

		if (remainingToRead == 0) {
			if (lastChunk) {
				break;
			}

			if (bits.size() < 8) {
				readBits(from, bits);
			}
			if (bits.front() == 1) {
				bits.pop_front();
				remainingToRead = 4096;
			}
			else {
				bits.pop_front();
				while (bits.size() < 12) {
					readBits(from, bits);
				}

				bitset<12> number;
				for (int i = 11; i >= 0; i--) {
					number[i] = bits.front();
					bits.pop_front();
				}
				remainingToRead = number.to_ulong();
				lastChunk = true;
			}
		}

		if (bits.size() < 8) {
			readBits(from, bits);
		}

		char c;
		if (!tree.extractCharFromTree(bits, c)) {
			readBits(from, bits);
			continue;
		}
		cout << c;
		to << c;
		remainingToRead--;
	}

	return true;
}

bool decompressFile(const char * inFileName, const char * outFileName) {

	ifstream ifilestream(inFileName, ios::in | ios::binary);
	ofstream ofilestream(outFileName, ios::out);

	if (ifilestream.fail() || ofilestream.fail()) {
		return false;
	}

	deque<bool> bits;

	EncodeTree tree;
	readTreeFromFile(tree, ifilestream, bits);

	readCharsFromFile(tree, ifilestream, ofilestream, bits);

	ifilestream.close();
	ofilestream.close();

	return true;
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

	assert(decompressFile("tests/test0.huf", "tempfile"));
	/*assert(identicalFiles("tests/test0.orig", "tempfile"));*/

	/*
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
