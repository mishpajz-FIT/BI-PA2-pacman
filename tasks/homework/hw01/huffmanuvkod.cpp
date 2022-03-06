// TODO: Implement compressing

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
    string data;
    Node * left;
    Node * right;

    Node(string newData = "") {
        data = newData;
        left = nullptr;
        right = nullptr;
    }

    bool shouldHaveData() {
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
    queue<string> creationCharacters;

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

    void printRec(Node * from, string path) {
        string newPath = path;
        if (from->right != nullptr) {
            newPath.push_back('1');
            printRec(from->right, newPath);
        }

        newPath = path;
        if (from->left != nullptr) {
            newPath.push_back('0');
            printRec(from->left, newPath);
        }

        if ((from->shouldHaveData())) {
            cout << from->data << " : " << path << endl;
        }
    }

    void deallocRec(Node * from) {
        if (from != nullptr) {
            deallocRec(from->right);
            deallocRec(from->left);

            delete from;
        }
    }

    public:
    ~CodeTree() {
        deallocRec(root);
    }

    void createNewTree(queue<bool> & positions, queue<string> & characters) {
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

    bool extractCharFromTree(deque<bool>& bits, string & to) const {
        size_t bitIter = 0;
        Node * nodeIter = root;
        while (true) {

            if (nodeIter->shouldHaveData()) {
                to = nodeIter->data;
                for (size_t i = 0; i < bitIter; i++) {
                    bits.pop_front();
                }
                return true;
            }

            if (bits.size() <= bitIter) {
                return false;
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
        string path = "";
        printRec(root, path);
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

        try {
            ifilestream.get(c);
        }
        catch (...) {
            throw "Could not read from file";
        }

        if (ifilestream.fail()) {
            throw "Error reading from file";
        }

        for (int i = 7; i >= 0; i--) {
            bits.push_back((c >> i) & 1 ? true : false);
        }
    }

    char readChar() {
        char newChar = 0;
        for (int i = 7; i >= 0; i--) {
            newChar = newChar << 1;
            if (bits.front() == true) {
                newChar = (newChar | 1);
            }
            bits.pop_front();
        }
        return newChar;
    }

    bool readHeader() {
        int numberOfNodes = 0;
        int numberOfLists = 0;

        queue<bool> positions;
        queue<string> characters;

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
                string charString;
                char newChar = readChar();
                charString.push_back(newChar);
                if ((newChar & 0x80) != 0) {
                    unsigned long remainingChars = 0;
                    if ((newChar & 0xF8) == 0xF0) {
                        remainingChars = 3;
                    } else if ((newChar & 0xF0) == 0xE0) {
                        remainingChars = 2;
                    } else if ((newChar & 0xE0) == 0xC0) {
                        remainingChars = 1;
                    }

                    while (bits.size() < remainingChars * 8) {
                        try {
                            readBits();
                        }
                        catch (...) {
                            return false;
                        }
                    }

                    for (unsigned long i = 0; i < remainingChars; i++) {
                        newChar = readChar();
                        if ((newChar & 0xC0) != 0x80) {
                            return false;
                        }
                        charString.push_back(newChar);
                    }
                }

                characters.push(charString);
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

    bool readBody() {
        unsigned long remainingToRead = 0;
        bool lastChunk = false;

        while (true) {

            if (remainingToRead == 0) {
                if (lastChunk) {
                    ifilestream.peek();
                    if (!ifilestream.eof() || bits.size() >= 8) {
                        return false;
                    }
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

                    unsigned long number = 0;
                    for (int i = 11; i >= 0; i--) {
                        number = number << 1;
                        if (bits.front() == true) {
                            number = (number | 1);
                        }
                        bits.pop_front();
                    }
                    remainingToRead = number;
                    lastChunk = true;
                    continue;
                }
            }

            string c;
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

        if (!readBody()) {
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

    assert(!decompressFile("tests/test5.huf", "tempfile"));

    assert(!decompressFile("tests/test6.huf", "tempfile"));

    assert(!decompressFile("tests/test7.huf", "tempfile"));

    assert(!decompressFile("tests/test8.huf", "tempfile"));

    assert(!decompressFile("tests/test9.huf", "tempfile"));

    assert(decompressFile("tests/extra0.huf", "tempfile"));
    assert(identicalFiles("tests/extra0.orig", "tempfile"));

    assert(decompressFile("tests/extra1.huf", "tempfile"));
    assert(identicalFiles("tests/extra1.orig", "tempfile"));

    assert(decompressFile("tests/extra2.huf", "tempfile"));
    assert(identicalFiles("tests/extra2.orig", "tempfile"));

    assert(decompressFile("tests/extra3.huf", "tempfile"));
    assert(identicalFiles("tests/extra3.orig", "tempfile"));

    assert(decompressFile("tests/extra4.huf", "tempfile"));
    assert(identicalFiles("tests/extra4.orig", "tempfile"));

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

    assert(decompressFile("tests/extra10.huf", "tempfile"));
    assert(identicalFiles("tests/extra10.orig", "tempfile"));
    return 0;
}
#endif /* __PROGTEST__ */
