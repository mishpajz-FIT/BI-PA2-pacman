/**
 * @file huffmanuvkod.cpp
 * @author Michal Dobeš
 * @date 2022-03-11
 *
 * @brief Implementation of decompression and compression of UTF-8 text files using Huffman encoding.
 *
 * Huffman encoding: https://en.wikipedia.org/wiki/Huffman_coding
 *
 *
 * Note: Everything is stored in one file because of the submission software.
 *
 * @copyright Copyright (c) 2022
 *
 */
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

/**
 * @brief Storage for UTF-8 characters
 *
 * Stores up to 4 chars.
 *
 */
class LongChar {
private:
    char * data; //Dynamically allocated array of 4 characters
    unsigned int length; //Count of occupied character in array

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

    /**
     * @brief Add char behind last character (or at the front)
     *
     * Throws if already filled with chars.
     *
     * @param newChar Char to append.
     */
    void push(char newChar) {
        if (length < 4) {
            data[length++] = newChar;
        } else {
            throw "String full";
        }
    }

    /**
     * @brief Access character at position
     *
     * Throws if
     *
     * @param i Index of position
     * @return char& Character at position
     */
    char & at(unsigned int i) const {
        if (i < length) {
            return data[i];
        } else {
            throw "Wrong input";
        }
    }

    /**
     *  @copydoc LongChar::at(unsigned int i) const
     */
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

    /**
     * @brief Return amount of chars that are filled
     *
     * @return unsigned int Amount
     */
    unsigned int size() const {
        return length;
    }

    LongChar & operator = (const LongChar & obj) {
        if (&obj == this) return *this;
        length = obj.size();
        for (unsigned int i = 0; i < obj.size(); i++) {
            data[i] = obj[i];
        }
        return *this;
    }

    bool operator < (const LongChar & rhs) const {
        if (length == rhs.size()) {
            for (unsigned int i = 0; i < length; i++) {
                if (reinterpret_cast<unsigned char &>(data[i]) < reinterpret_cast<unsigned char &>(rhs[i])) {
                    return true;
                } else if (reinterpret_cast<unsigned char &>(data[i]) > reinterpret_cast<unsigned char &>(rhs[i])) {
                    return false;
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

/**
 * @brief Node in bindary tree
 *
 */
struct Node {
    LongChar data; //UTF-8 Char
    unsigned int frequency; //Frequency in file

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

    /**
     * @brief Should this node contain data
     *
     * @return true Node is a leaf
     * @return false Node is not a leaf
     */
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

/**
 * @brief Comparison function for comparing Node by frequency elements
 *
 * @param rhs Node
 * @param lhs Node
 * @return true frequency of rhs < frequecy of lhs
 * @return false !(frequency of rhs < frequecy of lhs)
 */
bool compareNodePointersByFreq(Node * rhs, Node * lhs) {
    return (*rhs).frequency < (*lhs).frequency;
}

/**
 * @brief Binary tree used for Huffman coding
 *
 */
class CodeTree {
protected:

    Node * root; //Root Node of the tree
    size_t size; //Nodes in the tree

    /**
     * @brief Recursively print path to and Node value to stream
     *
     * @param from Node which (and its descendants) to print to stream
     * @param path Path to this Node
     * @param os Stream to print to
     */
    void printRec(Node * from, string path, ostream & os) {
        string newPath = path;
        if (from->right != nullptr) { //Writes 1 for right turn in tree path, 0 for left turn
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

    /**
     * @brief Node deallocation recursively
     *
     * @param from Node which (and its descendants) to dealloc
     */
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

    /**
     * @brief Print tree (Node values and paths to them) to stream
     *
     * @param os Stream to print to
     */
    void printTree(ostream & os) {
        string path = "";
        printRec(root, path, os);
    }

    friend ostream & operator << (ostream & os, CodeTree & rhs) {
        rhs.printTree(os);
        return os;
    }
};

/**
 * @brief Binary tree used to decode from the encoded file
 *
 */
class DecodeTree : public CodeTree {
private:
    queue<bool> creationPositions; //Positions in binary tree for characters read from the file header
    queue<LongChar> creationCharacters; //Characters encoded in the file header

    /**
     * @brief Creates Nodes in the tree
     *
     * Nodes are created at the positions specified by DecodeTree#creationPositions for the characters in DecodeTree#creationCharacters.
     *
     * @param at Pointer to position where new Node should be allocated.
     */
    void createNode(Node ** at) {
        //Positions of nodes should be coded using pre-order travelsal
        if (creationPositions.front() == false) { //If positions (from file header) contain 0, node should not contain characters
            *at = new Node;
            size++;
            creationPositions.pop();
            createNodePosition(*at);
        } else { //If positions (from file header) contain 1, node be a leaf and contain characters
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
    /**
     * @brief Construct a binary encoding tree from the encoding information
     *
     * @param positions Positions in binary tree for characters read from the file header
     * @param characters Characters encoded in the file header
     */
    void createNewTree(queue<bool> & positions, queue<LongChar> & characters) {
        //Create root and then generate other nodes recursively 
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

    /**
     * @brief Based on the provided encoded bits, finds the decoded character in the tree
     *
     * When a character is found, the bits that encode it are removed.
     *
     * Throws when character can not be decoded from encoded bits.
     *
     * @param[inout] bits Encoded bits
     * @param[out] to Decoded character
     * @return true The character was decoded
     * @return false The character could not be decoded (probably not enough bits read from the file, if the encoding is wrong it throws)
     */
    bool extractCharFromTree(deque<bool> & bits, LongChar & to) const {
        size_t bitIter = 0;
        Node * nodeIter = root;
        while (true) {
            if (nodeIter->shouldHaveData()) { //If found leaf, return character
                to = nodeIter->data;
                for (size_t i = 0; i < bitIter; i++) {
                    bits.pop_front();
                }
                return true;
            }

            if (bits.size() <= bitIter) { //If not enough bits loaded to continue reading
                return false;
            }

            if (bits.at(bitIter) == true) { //If bit (encoding the char) is 1, the character is in right branch of node
                if (nodeIter->right == nullptr) {
                    throw "Error in encoding";
                }
                nodeIter = nodeIter->right;
            } else { //If bit (encoding the char) is 0, the character is in left branch of node
                if (nodeIter->left == nullptr) {
                    throw "Error in encoding";
                }
                nodeIter = nodeIter->left;
            }
            bitIter++;
        }
    }
};

/**
 * @brief Binary tree used to encode file
 *
 */
class EncodeTree : public CodeTree {
private:

    map<LongChar, Node * > translation; //The map storing the character and the Node that encodes it

    /**
     * @brief Creates Nodes in the tree
     *
     * Nodes are created based on their frequency of occurrence in the file.
     *
     * @param frequecies Map containing characters and their frequency of occurrence in the set
     * @return true The binary tree with node was created
     * @return false There was an error creating the tree
     */
    bool organizeNodes(map<LongChar, unsigned int> & frequecies) {
        deque<Node * > nodes; //Nodes without parent

        for (auto const & [key, value] : frequecies) { //For each unique character in file create node and pass frequency of characters to node
            Node * newNode = new Node(key, value);
            nodes.push_back(newNode);
            translation[key] = newNode;
            size++;
        }

        if (nodes.size() == 0) {
            return false;
        }

        sort(nodes.begin(), nodes.end(), compareNodePointersByFreq); //Sort pointers to nodes by frequency

        while (nodes.size() > 1) { //Until there is only one node without parent (root)
            Node * parentNode = new Node(); //Create new node (without character)

            //Add the two nodes with smallest frequecy that do not have a parent yet as the new nodes left and right child
            nodes.front()->parent = parentNode;
            parentNode->left = nodes.front();
            nodes.pop_front();
            parentNode->right = nodes.front();
            nodes.front()->parent = parentNode;
            nodes.pop_front();

            parentNode->frequency = parentNode->left->frequency + parentNode->right->frequency; //Set frequency to the new node as sum of freqencies of its childer nodes 

            nodes.insert(lower_bound(nodes.begin(), nodes.end(), parentNode, compareNodePointersByFreq), parentNode); //Insert new node to deque with nodes without parent at correct position (so the deque stays sorted by frequency)

            size++;
        }

        root = nodes.front(); //Last node without parent is root

        return true;
    }


    /**
     * @brief Recursive retrieval of paths to characters
     *
     * Stores the retrieved paths together with the characters.
     *
     * @param node Node which is next in path
     * @param to Bits to which to append the encoding
     */
    void recTreePath(Node * node, deque<bool> & to) {
        //Positions of nodes should be coded using pre-order travelsal
        //If node isnt leaf, add 0, if is leaf add 1 and the whole character
        if (node->shouldHaveData()) {
            to.push_back(true);
            for (unsigned int i = 0; i < node->data.size(); i++) {
                for (int j = 7; j >= 0; j--) {
                    to.push_back((node->data[i] >> j) & 1 ? true : false);
                }
            }
        } else {
            to.push_back(false);
            recTreePath(node->left, to);
            recTreePath(node->right, to);
        }
    }
public:

    /**
     * @brief Gets the bits representing encoding for given character
     *
     * Finds a character in a binary tree and creates an encoding from the path from the root of the tree to the character.
     *
     * The encoding bits are added to the end of the deque
     *
     * @param[in] forChar Character to be encoded
     * @param[inout] to Bits to which to append the encoding
     * @return true The character has been successfully encoded
     * @return false The character hasn't been successfully encoded
     */
    bool retrieveCoding(const LongChar & forChar, deque<bool> & to) {
        if (translation.find(forChar) != translation.end()) { //Find node for given character
            vector<bool> inBits;
            Node * codedNode = translation[forChar];
            Node * codedParent = codedNode->parent;
            if (codedParent == nullptr) {
                return false;
            }
            while (codedParent != nullptr) { //Get path from the node containing the character to the root 
                if (codedNode == codedParent->left) {
                    inBits.push_back(false);
                } else if (codedNode == codedParent->right) {
                    inBits.push_back(true);
                } else {
                    return false;
                }
                codedNode = codedParent;
                codedParent = codedNode->parent;
            }

            while (!inBits.empty()) { //Reverse recieved path (so it is root to node), this path is code encoding the character
                to.push_back(inBits.back());
                inBits.pop_back();
            }
            return true;
        }
        return false;
    }

    /**
     * @brief From the characters in the file creates a binary tree representing the encoding of those characters.
     *
     * @param characters
     * @return true The tree was successfully created
     * @return false The tree creation failed
     */
    bool generateTree(deque<LongChar> & characters) {
        map<LongChar, unsigned int> frequecies;

        for (auto & longCharElement : characters) { //Store the number of occurrences for each character in file using map
            frequecies[longCharElement]++;
        }

        return organizeNodes(frequecies);
    }

    /**
     * @brief Gets the encoding of all stored characters
     *
     * The bits representing the path in the binary tree along with the character are stored in the deque.
     *
     * @param[inout] to Bits to which to append the encoding
     */
    void renderTreePaths(deque<bool> & to) {
        recTreePath(root, to);
    }
};

/**
 * @brief Compression & Decompression masterclass
 *
 */
class Compression {
protected:
    ifstream ifilestream; //Input file
    ofstream ofilestream; //Output file

    deque<bool> bits; //Bits in memory

public:

    /**
     * @brief Based on the first char distinguishes the length of the UTF-8 character
     *
     * @param newChar First char
     * @return int UTF-8 character length (in bytes)
     */
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

    /**
     * @brief Determine if char falls within the UTF-8 character range that is part of
     *
     * @param[in] totalCharLength UTF-8 character length in bytes
     * @param[in] currentChar Index of char in UTF-8 character
     * @param[in] firstCharAtLimit A flag indicating the limit position of the first char in the UTF-8 range
     * @param[in] newChar Char in UTF-8 character
     * @return true newChar is valid in UTF-8 encoding
     * @return false newChar is not valid
     */
    bool isValidUTF8Char(int totalCharLength, int currentChar, int & firstCharAtLimit, char & newChar) {
        if (currentChar == 0) {
            firstCharAtLimit = 0; //Set flag to default
        } else {
            if ((newChar & 0xC0) != 0x80) { //Check if char is valid for not first byte of character 
                return false;
            }
        }

        switch (totalCharLength) { //Check byte is in range specified by last coding point and first coding point (or if first byte is the smallest possible set flag)
            case 2:
                if (currentChar == 0) {
                    if ((reinterpret_cast<unsigned char &>(newChar)) < (unsigned)(0xC2)) {
                        return false;
                    }
                }
            case 3:
                if (currentChar == 0) {
                    if (reinterpret_cast<unsigned char &>(newChar) == (unsigned)(0xE0)) {
                        firstCharAtLimit = -1;
                    }
                } else if (currentChar == 1 && firstCharAtLimit == -1) {
                    if (reinterpret_cast<unsigned char &>(newChar) < (unsigned)(0xA0)) {
                        return false;
                    }
                }
            case 4:
                if (currentChar == 0) {
                    if (reinterpret_cast<unsigned char &>(newChar) == (unsigned)(0xF4)) {
                        firstCharAtLimit = 1;
                    } else if (reinterpret_cast<unsigned char &>(newChar) > (unsigned)(0xF4)) {
                        return false;
                    } else if (reinterpret_cast<unsigned char &>(newChar) == (unsigned)(0xF0)) {
                        firstCharAtLimit = -1;
                    }
                } else if (currentChar == 1) {
                    if (firstCharAtLimit == 1) {
                        if (reinterpret_cast<unsigned char &>(newChar) > (unsigned)(0x8F)) {
                            return false;
                        }
                    } else if (firstCharAtLimit == -1) {
                        if (reinterpret_cast<unsigned char &>(newChar) < (unsigned)(0x90)) {
                            return false;
                        }
                    }
                }
                break;
        }

        return true;
    }
};

/**
 * @brief File decompression using Huffman encoding
 *
 */
class Decompresser : public Compression {
private:

    DecodeTree tree; //Binary tree storing encoding

    /**
     * @brief Reads bytes from input file and stores them as bits
     *
     * If error is encoutered, throws.
     *
     */
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
            bits.push_back((c >> i) & 1 ? true : false); //Using bitshift and mask get each bit out of char, add 0 as false and 1 as true
        }
    }

    /**
     * @brief Re-forms bits into chars (bytes)
     *
     * @return char Created char
     */
    char readChar() {
        char newChar = 0;
        for (int i = 7; i >= 0; i--) { //Using bitshift and mask recreate char out of 8 bits
            newChar = newChar << 1;
            if (bits.front() == true) {
                newChar = (newChar | 1);
            }
            bits.pop_front();
        }
        return newChar;
    }

    /**
     * @brief Reads the header of the encoded input file
     *
     * The header must contain encoding information.
     * Creates an encoding binary tree based on the header.
     *
     * @return true Header read successfully and the tree was created
     * @return false There was a problem during reading or three creation
     */
    bool readHeader() {
        int numberOfNodes = 0; //Number of total nodes for coding tree recieved
        int numberOfLists = 0; //Number of leafs for coding tree recieved

        queue<bool> positions; //Positions in coding tree (0 means node, 1 means leaf with character from characters)
        queue<LongChar> characters; //Characters that is encoded in file

        bool readingChars = false;

        while (true) {
            if (bits.size() < 8) { //Read bits from file if not enough bits
                try {
                    readBits();
                }
                catch (...) {
                    return false;
                }
            }

            if (readingChars && bits.size() >= 8) { //If enough bits and there should be a char (instead of positions of encoding) in input, read given char
                LongChar longChar;
                char newChar = readChar();
                longChar.push(newChar);

                int charLength = utf8CharLength(newChar); //Check character length in bytes (if UTF-8)
                if (charLength == 0) {
                    return false;
                }

                while (bits.size() < (unsigned long)((charLength - 1) * 8)) { //If UTF-8 character, read enough bits for whole character
                    try {
                        readBits();
                    }
                    catch (...) {
                        return false;
                    }
                }

                int firstCharAtLimitValue = 0;
                if (!isValidUTF8Char(charLength, 0, firstCharAtLimitValue, newChar)) { //Check if first character is valid
                    return false;
                }
                for (int i = 1; i < charLength; i++) { //Read other char in UTF-8 character anc check if they are valid
                    newChar = readChar();

                    if (!isValidUTF8Char(charLength, i, firstCharAtLimitValue, newChar)) {
                        return false;
                    }

                    longChar.push(newChar);
                }


                characters.push(longChar);
                readingChars = false;

                if (numberOfLists == numberOfNodes + 1) { //If tree is full, end reading
                    break;
                }
            } else if (!readingChars) { //Add positions (0s) until position signals 1 (leaf), so character encoded by this position should come next
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

    /**
     * @brief Reads the encoded characters in the input file and writes the decoded characters to the output file
     *
     * @return true The input file has been decoded to the end
     * @return false An unexpected error occurred during reading or decoding
     */
    bool readBody() {
        unsigned long remainingToRead = 0; //Count of characters that should be decoded (length of chunk of characters)
        bool lastChunk = false;

        while (true) {

            if (remainingToRead == 0) {
                if (lastChunk) {
                    ifilestream.peek(); //Check if there trurly are no other characters encoded (after decoded what should be last chunk)
                    if (!ifilestream.eof() || bits.size() >= 8) {
                        return false;
                    }
                    break;
                }

                //Calculate length of next chunk
                if (bits.size() < 8) {
                    try {
                        readBits();
                    }
                    catch (...) {
                        return false;
                    }
                }
                if (bits.front() == 1) { //If contains 1, chunk should be 4096 characters
                    bits.pop_front();
                    remainingToRead = 4096;
                } else { //If contains 0, length of chunk is stored in next 12 bits

                    bits.pop_front();
                    while (bits.size() < 12) {
                        try {
                            readBits();
                        }
                        catch (...) {
                            return false;
                        }
                    }

                    unsigned long number = 0; //
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

            LongChar c; //Extract encoded character using bits read in file
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

            //Write encoded character into output file
            if (ofilestream.fail()) {
                return false;
            }

            for (size_t i = 0; i < c.size(); i++) { //If character is UTF-8, write all chars
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

    /**
     * @brief Decodes input file into text output file (specified in the object constructor)
     *
     * @return true Decoding was successful
     * @return false An error occurred during the process
     */
    bool decode() {

        if (!readHeader()) { //Read header and create decoding tree
            return false;
        }

        if (!readBody()) { //Read body and decode characters into file
            return false;
        }

        return true;
    }
};

/**
 * @brief File compression using Huffman encoding
 *
 */
class Compresser : public Compression {
private:
    EncodeTree tree; //Binary tree storing encoding
    deque<LongChar> characters; //Memory of characters read from the input file

    /**
     * @brief Converts bits to bytes and writes to the output file
     *
     * If specified, pads the last bits with zeros to the whole byte, otherwise leaves bits giving less than a whole byte in memory.
     *
     * @param last Fill the last byte with zeros
     */
    void writeFile(bool last = false) {
        while (bits.size() > 0) {
            if (bits.size() < 8) { //If not enough bits for full byte, either pad with 0s or break based on parameter
                if (last) {
                    while (bits.size() < 8) {
                        bits.push_back(false);
                    }
                } else {
                    break;
                }
            }

            if (ofilestream.fail()) {
                throw "Failed writing";
            }

            char c = 0; //Using bitshift and mask create char out of 8 bits
            for (int i = 7; i >= 0; i--) {
                c = c << 1;
                if (bits.front() == true) {
                    c = (c | 1);
                }
                bits.pop_front();
            }

            ofilestream.put(c); //Write created char
        }
    }

    /**
     * @brief Reads the characters from the input file and writes the encoded characters to the output file
     *
     * Encoded characters also include notation about their count.
     *
     * @return true All characters have been encoded
     * @return false An unexpected error occurred during reading or encoding
     */
    bool convertChars() {
        unsigned int remainingCharacters = 0; //Count of characters that should be encoded (length of chunk of characters)
        bool lastChunkFull = false; // Was last chunk full (4096 characters)

        while (characters.size() > 0) {
            if (remainingCharacters == 0) { //If filled chunk, write new information about chunk length
                if (characters.size() > 4096) {
                    remainingCharacters = 4096;
                    bits.push_back(true);
                    lastChunkFull = true;
                } else {
                    remainingCharacters = characters.size();
                    bits.push_back(false);
                    for (int j = 11; j >= 0; j--) {
                        bits.push_back((remainingCharacters >> j) & 1 ? true : false);
                    }
                    lastChunkFull = false;
                }
            }

            if (!tree.retrieveCoding(characters.front(), bits)) { //Recieve and write encoding for character in file
                return false;
            }

            characters.pop_front();
            remainingCharacters--;
            try {
                writeFile();
            }
            catch (...) {
                return false;
            }
        }

        if (lastChunkFull) { //If chunk was long 4096 characters and was long, write information about chunk with length 0
            for (unsigned int i = 0; i < 13; i++) {
                bits.push_back(false);
            }
        }

        try { //Write last bits into file (and pad nonfull bytes with 0s)
            writeFile(true);
        }
        catch (...) {
            return false;
        }

        return true;
    }

    /**
     * @brief Reads characters from the input file and stores them in memory
     *
     * If error is encoutered, throws.
     *
     * @param[out] c Read character
     * @return true The read was a success (and can continue)
     * @return false The reading has been completed
     */
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

    /**
     * @brief Reads the input file and stores all characters in memory
     *
     * @return true The input file has been read to the end
     * @return false An unexpected error occurred during reading
     */
    bool readFile() {
        while (true) { //Read character from input file
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

            int charLength = utf8CharLength(newChar); //Check character length in bytes (if UTF-8)
            if (charLength == 0) {
                return false;
            }

            int firstCharAtLimitValue = 0;
            if (!isValidUTF8Char(charLength, 0, firstCharAtLimitValue, newChar)) { //Check if first character is valid
                return false;
            }
            for (int i = 1; i < charLength; i++) { //Read other char in UTF-8 character anc check if they are valid
                try {
                    if (!readChar(newChar)) {
                        return false;
                    }
                }
                catch (...) {
                    return false;
                }

                if (!isValidUTF8Char(charLength, i, firstCharAtLimitValue, newChar)) {
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

    /**
     * @brief Encodes input file into binary output file (specified in the object constructor)
     *
     * @return true Encoding was successful
     * @return false An error occurred during the process
     */
    bool encode() {

        if (!readFile()) { //Read all characters from input file and store them in memory
            return false;
        }

        if (!tree.generateTree(characters)) { //Generate encoding tree from characters in memory (from input file)
            return false;
        }

        tree.renderTreePaths(bits); //Generate header for output file containing encoding information (and store it as bits)

        if (!convertChars()) { //For each character in memory get encoding from tree (as bits), and write bits into output file
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




    assert(compressFile("tests/test0.orig", "tmp.huf"));
    assert(decompressFile("tmp.huf", "tmp.orig"));
    assert(identicalFiles("tests/test0.orig", "tmp.orig"));

    assert(compressFile("tests/test1.orig", "tmp.huf"));
    assert(decompressFile("tmp.huf", "tmp.orig"));
    assert(identicalFiles("tests/test1.orig", "tmp.orig"));

    assert(compressFile("tests/test2.orig", "tmp.huf"));
    assert(decompressFile("tmp.huf", "tmp.orig"));
    assert(identicalFiles("tests/test2.orig", "tmp.orig"));

    assert(compressFile("tests/test3.orig", "tmp.huf"));
    assert(decompressFile("tmp.huf", "tmp.orig"));
    assert(identicalFiles("tests/test3.orig", "tmp.orig"));

    assert(compressFile("tests/test4.orig", "tmp.huf"));
    assert(decompressFile("tmp.huf", "tmp.orig"));
    assert(identicalFiles("tests/test4.orig", "tmp.orig"));


    assert(compressFile("tests/extra0.orig", "tmp.huf"));
    assert(decompressFile("tmp.huf", "tmp.orig"));
    assert(identicalFiles("tests/extra0.orig", "tmp.orig"));

    assert(compressFile("tests/extra1.orig", "tmp.huf"));
    assert(decompressFile("tmp.huf", "tmp.orig"));
    assert(identicalFiles("tests/extra1.orig", "tmp.orig"));

    assert(compressFile("tests/extra2.orig", "tmp.huf"));
    assert(decompressFile("tmp.huf", "tmp.orig"));
    assert(identicalFiles("tests/extra2.orig", "tmp.orig"));

    assert(compressFile("tests/extra3.orig", "tmp.huf"));
    assert(decompressFile("tmp.huf", "tmp.orig"));
    assert(identicalFiles("tests/extra3.orig", "tmp.orig"));

    assert(compressFile("tests/extra4.orig", "tmp.huf"));
    assert(decompressFile("tmp.huf", "tmp.orig"));
    assert(identicalFiles("tests/extra4.orig", "tmp.orig"));

    assert(compressFile("tests/extra5.orig", "tmp.huf"));
    assert(decompressFile("tmp.huf", "tmp.orig"));
    assert(identicalFiles("tests/extra5.orig", "tmp.orig"));

    assert(compressFile("tests/extra6.orig", "tmp.huf"));
    assert(decompressFile("tmp.huf", "tmp.orig"));
    assert(identicalFiles("tests/extra6.orig", "tmp.orig"));

    assert(compressFile("tests/extra7.orig", "tmp.huf"));
    assert(decompressFile("tmp.huf", "tmp.orig"));
    assert(identicalFiles("tests/extra7.orig", "tmp.orig"));

    assert(compressFile("tests/extra8.orig", "tmp.huf"));
    assert(decompressFile("tmp.huf", "tmp.orig"));
    assert(identicalFiles("tests/extra8.orig", "tmp.orig"));

    assert(compressFile("tests/extra9.orig", "tmp.huf"));
    assert(decompressFile("tmp.huf", "tmp.orig"));
    assert(identicalFiles("tests/extra9.orig", "tmp.orig"));
    return 0;
}
#endif /* __PROGTEST__ */
