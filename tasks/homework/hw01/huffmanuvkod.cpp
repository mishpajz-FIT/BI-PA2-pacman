// TODO: Implement UTF-8 storage and conversion
// TODE: Implement compressing

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

class CodeTree {
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
					throw "Error in encoding";
				}
				nodeIter = nodeIter->right;
			} else {
				if (nodeIter->left == nullptr) {
					throw "Error in encoding";
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

class Decompresser {
	private:

	ifstream ifilestream;
	ofstream ofilestream;

	CodeTree tree;

	deque<bool> bits;

	void readBits() {
		char c;

		if (ifilestream.eof()) {
			throw "Trying to read when EOF";
		}

		try {
			ifilestream.get(c);
		}
		catch (...) {
			throw "Could not read from file";
		}

		if (ifilestream.fail() && !ifilestream.eof()) {
			throw "Error reading from file";
		}

		for (int i = 7; i >= 0; i--) {
			bits.push_back((c >> i) & 1 ? true : false);
		}
	}

	bool readHeader() {
		int numberOfNodes = 0;
		int numberOfLists = 0;

		queue<bool> positions;
		queue<char> characters;

		bool readingChars = false;

		while (true) {
			if (bits.size() < 8) {
				try {
					readBits();
				}
				catch (...) {
					return false;
				}
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
		return true;
	}

	bool readChars() {
		unsigned long remainingToRead = 0;
		bool lastChunk = false;

		while (true) {

			if (remainingToRead == 0) {
				if (lastChunk) {
					// TODO: Check if file is trurly empty...
					break;
				}

				if (bits.size() < 8) {
					try {
						readBits();
					}
					catch (...) {
						return false;
					}
				}
				if (bits.front() == 1) {
					bits.pop_front();
					remainingToRead = 4096;
				}
				else {
					bits.pop_front();
					while (bits.size() < 12) {
						try {
							readBits();
						}
						catch (...) {
							return false;
						}
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
				try {
					readBits();
				}
				catch (...) {
					return false;
				}
			}

			char c;
			try {
				if (!tree.extractCharFromTree(bits, c)) {
					try {
						readBits();
					}
					catch (...) {
						return false;
					}
					continue;
				}
			}
			catch (...) {
				return false;
			}
			ofilestream << c;
			remainingToRead--;
		}

		return true;
	}

	public:
	Decompresser(const char* inFileName, const char* outFileName) {
		tree = CodeTree();

		ifilestream.open(inFileName, ios::in | ios::binary);
		ofilestream.open(outFileName, ios::out);

		if (ifilestream.fail() || ofilestream.fail()) {
			throw "Error opening file";
		}
	}

	~Decompresser() {
		if (ifilestream.is_open()) {
			ifilestream.close();
		}

		if (ofilestream.is_open()) {
			ofilestream.close();
		}
	}

	bool decode() {

		if (!readHeader()) {
			return false;
		}
		
		if (!readChars()) {
			return false;
		}

		return true;
	}
};

bool decompressFile(const char * inFileName, const char * outFileName) {

	try {
		Decompresser decompresser(inFileName, outFileName);
		return decompresser.decode();
	} catch (...) {
		return false;
	}
	return false;
}

bool compressFile(const char * inFileName, const char * outFileName) {
  	// keep this dummy implementation (no bonus) or implement the compression (bonus)
	return false;
}

#ifndef __PROGTEST__
bool identicalFiles(const char * fileName1, const char * fileName2) {

	ifstream ifs1(fileName1, ios::in | ios::binary);
	ifstream ifs2(fileName2, ios::in | ios::binary);

	while(true) {
		bool eof;
		if ((eof = ifs1.eof()) != ifs2.eof()) {
			ifs1.close();
			ifs2.close();
			return false;
		}

		if(eof) {
			break;
		}

		char c1, c2;
		ifs1.get(c1);
		ifs2.get(c2);
		if (c1 != c2) {
			ifs1.close();
			ifs2.close();
			return false;
		}
	}

	ifs1.close();
	ifs2.close();
	return true;
}

int main(void) {

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

	/*
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
