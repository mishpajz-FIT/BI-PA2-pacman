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

class LongChar {
private:
    char * data;
    unsigned int length;

public:
    LongChar() : length(0) {
        data = new char[4];
    }

    LongChar(const LongChar & obj) : length(obj.size()) {
        data = new char[4];
        for (unsigned int i = 0; i < obj.size(); i++) {
            data[i] = obj[i];
        }
    }

    ~LongChar() {
        delete [] data;
    }

    void push(char newChar) {
        if (length < 4) {
            data[length++] = newChar;
        } else {
            throw "String full";
        }
    }

    char & at(unsigned int i) const {
        if (i < length) {
            return data[i];
        } else {
            throw "Wrong input";
        }
    }

    char & operator [ ] (unsigned int i) const {
        if (i < length) {
            return data[i];
        } else {
            throw "Wrong input";
        }
    }

    bool operator == (const LongChar & rhs) const {
        if (length == rhs.size()) {
            for (unsigned int i = 0; i < length; i++) {
                if (data[i] != rhs[i]) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    unsigned int size() const {
        return length;
    }

    void operator = (const LongChar & obj) {
        length = obj.size();
        for (unsigned int i = 0; i < obj.size(); i++) {
            data[i] = obj[i];
        }
    }

    bool operator < (const LongChar & rhs) const {
        if (length == rhs.size()) {
            for (unsigned int i = 0; i < length; i++) {
                if (data[i] < rhs[i]) {
                    return true;
                }
            }
        } else {
            return (length < rhs.size());
        }
        return false;
    }

    friend ostream & operator << (ostream & os, LongChar & longChar) {
        for (unsigned int i = 0; i < longChar.length; i++) {
            os << longChar.data[i];
        }
        return os;
    }
};

struct Node {
    LongChar data;
    unsigned int frequency;
    Node * left;
    Node * right;
    Node * parent;

    Node() : data(), frequency(0) {
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }

    Node(LongChar newChar, unsigned int newFrequency = 0) : data(newChar), frequency(newFrequency) {
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }

    bool shouldHaveData() {
        if (left == nullptr && right == nullptr) {
            return true;
        }
        return false;
    }

    bool operator < (const Node & rhs) const {
        return data < rhs.data;
    }
};

bool compareNodePointersByFreq(Node * rhs, Node * lhs) {
    return (*rhs).frequency < (*lhs).frequency;
}

class CodeTree {
protected:
    Node * root;
    size_t size;

    void printRec(Node * from, string path, ostream & os) {
        string newPath = path;
        if (from->right != nullptr) {
            newPath.push_back('1');
            printRec(from->right, newPath, os);
        }

        newPath = path;
        if (from->left != nullptr) {
            newPath.push_back('0');
            printRec(from->left, newPath, os);
        }

        if (from->shouldHaveData()) {
            os << "\n" << from->data << ": " << path;
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

    void printTree(ostream & os) {
        string path = "";
        printRec(root, path, os);
    }

    friend ostream & operator << (ostream & os, CodeTree & rhs) {
        rhs.printTree(os);
        return os;
    }
};

class DecodeTree : public CodeTree {
private:
    queue<bool> creationPositions;
    queue<LongChar> creationCharacters;

    void createNode(Node ** at) {
        if (creationPositions.front() == false) {
            *at = new Node;
            size++;
            creationPositions.pop();
            createNodePosition(*at);
        } else {
            *at = new Node(creationCharacters.front());
            size++;
            creationPositions.pop();
            creationCharacters.pop();
        }
    }

    void createNodePosition(Node * at) {
        createNode(&(at->left));

        createNode(&(at->right));
    }

public:
    void createNewTree(queue<bool> & positions, queue<LongChar> & characters) {
        root = new Node;
        if (positions.front() == true) {
            root->data = characters.front();
            return;
        } else {
            positions.pop();

            creationPositions = positions;
            creationCharacters = characters;

            createNodePosition(root);
        }
    }

    bool extractCharFromTree(deque<bool> & bits, LongChar & to) const {
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
};

class EncodeTree : public CodeTree {
private:

    map<LongChar, Node * > translation;

    bool organizeNodes(map<LongChar, unsigned int> & frequecies) {
        deque<Node * > nodes;
        for (auto const & [key, value] : frequecies) {
            Node * newNode = new Node(key, value);
            nodes.push_back(newNode);
            translation[key] = newNode;
            size++;
        }

        if (nodes.size() == 0) {
            return false;
        }

        sort(nodes.begin(), nodes.end(), compareNodePointersByFreq);

        while (nodes.size() > 1) {
            Node * parentNode = new Node();
            nodes.front()->parent = parentNode;
            parentNode->left = nodes.front();
            nodes.pop_front();
            parentNode->right = nodes.front();
            nodes.front()->parent = parentNode;
            nodes.pop_front();
            parentNode->frequency = parentNode->left->frequency + parentNode->right->frequency;

            nodes.insert(lower_bound(nodes.begin(), nodes.end(), parentNode, compareNodePointersByFreq), parentNode);

            size++;
        }

        root = nodes.front();

        sort(translation.begin(), translation.end());

        return true;
    }
public:

    bool retrieveCoding(const LongChar & forChar, vector<bool> & to) {
        if (translation.find(forChar) != translation.end()) {
            Node * codedNode = translation[forChar];
            Node * codedParent = codedNode->parent;
            if (codedParent == nullptr) {
                return false;
            }
            while (codedParent != nullptr) {
                if (codedNode == codedParent->left) {
                    to.push_back(false);
                } else if (codedNode == codedParent->right) {
                    to.push_back(true);
                } else {
                    return false;
                }

                return true;
            }
        }
        return false;
    }

    bool generateTree(vector<LongChar> & characters) {
        map<LongChar, unsigned int> frequecies;

        for (auto & longCharElement : characters) {
            frequecies[longCharElement]++;
        }

        return organizeNodes(frequecies);
    }
};

class Compression {
protected:
    ifstream ifilestream;
    ofstream ofilestream;

    deque<bool> bits;

public:
    int utf8CharLength(const char & newChar) {
        if ((newChar & 0xF8) == 0xF0) {
            return 4;
        } else if ((newChar & 0xF0) == 0xE0) {
            return 3;
        } else if ((newChar & 0xE0) == 0xC0) {
            return 2;
        } else if ((newChar & 0x80) == 0) {
            return 1;
        } else {
            return 0;
        }
    }

    bool isValidUTF8Char(int totalCharLength, int currentChar, bool & firstCharMaxPossible, char & newChar) {
        if (totalCharLength == 4) {
            if (currentChar == 0) {
                if (reinterpret_cast<unsigned char &>(newChar) == (unsigned)(0xF4)) {
                    firstCharMaxPossible = true;
                } else if (reinterpret_cast<unsigned char &>(newChar) > (unsigned)(0xF4)) {
                    return false;
                }
            } else if (currentChar == 1) {
                if ((reinterpret_cast<unsigned char &>(newChar) > (unsigned)(0x8F)) && firstCharMaxPossible) {
                    return false;
                }
            }
        }

        if (currentChar != 0) {
            if ((newChar & 0xC0) != 0x80) {
                return false;
            }
        }
        return true;
    }
};

class Decompresser : public Compression {
private:

    DecodeTree tree;

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
        queue<LongChar> characters;

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
                LongChar longChar;
                char newChar = readChar();
                longChar.push(newChar);

                int charLength = utf8CharLength(newChar);
                if (charLength == 0) {
                    return false;
                }

                while (bits.size() < (unsigned long)((charLength - 1) * 8)) {
                    try {
                        readBits();
                    }
                    catch (...) {
                        return false;
                    }
                }

                bool maxPossibleForUTF8 = false;
                if (!isValidUTF8Char(charLength, 0, maxPossibleForUTF8, newChar)) {
                    return false;
                }
                for (int i = 1; i < charLength; i++) {
                    newChar = readChar();

                    if (!isValidUTF8Char(charLength, i, maxPossibleForUTF8, newChar)) {
                        return false;
                    }

                    longChar.push(newChar);
                }


                characters.push(longChar);
                readingChars = false;

                if (numberOfLists == numberOfNodes + 1) {
                    break;
                }
            } else if (!readingChars) {
                positions.push(bits.front());
                bits.pop_front();
                if (positions.back() == false) {
                    numberOfNodes++;
                } else {
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
                } else {
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

            LongChar c;
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

            if (ofilestream.fail()) {
                return false;
            }

            for (size_t i = 0; i < c.size(); i++) {
                ofilestream.put(c.at(i));
            }
            remainingToRead--;
        }

        return true;
    }

public:
    Decompresser(const char * inFileName, const char * outFileName) {
        tree = DecodeTree();

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

class Compresser : public Compression {
private:
    EncodeTree tree;
    vector<LongChar> characters;

    bool writeFile(bool last = false) {

    }

    bool convertChars() {
        while (characters.size() > 0) {
            vector<bool> charBits;
        }
    }

    bool readChar(char & c) {
        try {
            ifilestream.get(c);
        }
        catch (...) {
            throw "Could not read from file";
        }

        if (ifilestream.eof()) {
            return false;
        }

        if (ifilestream.fail()) {
            throw "Error reading from file";
        }

        return true;
    }

    bool readFile() {
        while (true) {
            LongChar newLongChar;
            char newChar;
            try {
                if (!readChar(newChar)) {
                    break;
                }
            }
            catch (...) {
                return false;
            }

            newLongChar.push(newChar);

            int charLength = utf8CharLength(newChar);
            if (charLength == 0) {
                return false;
            }

            bool maxPossibleForUTF8 = false;
            if (!isValidUTF8Char(charLength, 0, maxPossibleForUTF8, newChar)) {
                return false;
            }
            for (int i = 1; i < charLength; i++) {
                try {
                    if (!readChar(newChar)) {
                        return false;
                    }
                }
                catch (...) {
                    return false;
                }

                if (!isValidUTF8Char(charLength, i, maxPossibleForUTF8, newChar)) {
                    return false;
                }

                newLongChar.push(newChar);
            }

            characters.push_back(newLongChar);
        }
        return true;
    }

public:
    Compresser(const char * inFileName, const char * outFileName) {
        tree = EncodeTree();

        ifilestream.open(inFileName, ios::in);
        ofilestream.open(outFileName, ios::out | ios::binary);

        if (ifilestream.fail() || ofilestream.fail()) {
            throw "Error opening file";
        }
    }

    bool encode() {

        if (!readFile()) {
            return false;
        }

        if (!tree.generateTree(characters)) {
            return false;
        }



        return true;
    }
};

bool decompressFile(const char * inFileName, const char * outFileName) {

    try {
        Decompresser decompresser(inFileName, outFileName);
        return decompresser.decode();
    }
    catch (...) {
        return false;
    }
    return false;
}

bool compressFile(const char * inFileName, const char * outFileName) {
    try {
        Compresser compresser(inFileName, outFileName);
        return compresser.encode();
    }
    catch (...) {
        return false;
    }
    return false;
}

#ifndef __PROGTEST__
bool identicalFiles(const char * fileName1, const char * fileName2) {

    ifstream ifs1(fileName1, ios::in | ios::binary);
    ifstream ifs2(fileName2, ios::in | ios::binary);

    while (true) {
        bool eof;

        char c1 = 0, c2 = 0;
        ifs1.get(c1);
        ifs2.get(c2);

        if ((eof = ifs1.fail()) != ifs2.fail()) {
            ifs1.close();
            ifs2.close();
            return false;
        }

        if (eof) {
            break;
        }

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

    assert(!decompressFile("tests/test10.huf", "tempfile"));

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

    compressFile("tests/test0.orig", "tempfile");
    return 0;
}
#endif /* __PROGTEST__ */
