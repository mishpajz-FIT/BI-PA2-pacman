/**
 * @file littleheap.cpp
 * @author Michal Dobeš
 * @date 2022-03-31
 *
 * @brief Implementation of (Min/Max) Heap using Vector
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <cassert>
#include <vector>

using namespace std;

/**
 * @brief Heap abstract template
 *
 */
template <typename T>
class LittleHeap {
private:
    vector<T> nodes; //Vector used for storing data

    virtual bool compare(T & lhs, T rhs) const = 0; //Compare function supplied by ancestor

public:

    /**
     * @brief Construct a new Heap object
     *
     */
    LittleHeap() { }

    virtual ~LittleHeap() = default;

    /**
     * @brief Add element into heap
     *
     * @param value
     */
    void push(const T & value) {
        size_t currentIndex = nodes.size();

        nodes.push_back(value); //Add to the back of the vectors

        size_t parentIndex;
        while (true) {
            parentIndex = (currentIndex - 1) / 2; //Calculate index of parent node

            if (currentIndex == 0) { //If in root, stop the loop
                break;
            }

            if (compare(nodes[parentIndex], value)) { //Compare value with parent, if parent is larger/smaller (depending on heap type) switch values
                swap(nodes[parentIndex], nodes[currentIndex]);
            } else {
                break;
            }

            currentIndex = parentIndex;
        }
    }

    /**
     * @brief Remove top element from heap
     *
     * Throws if heap is empty
     *
     * @return unsigned int Removed element
     */
    T pop() {
        if (nodes.size() == 0) {
            throw (-1);
        }

        T value = nodes.front();
        //Remove root (first value), add last value as root
        nodes.front() = nodes.back();
        nodes.pop_back();

        if (nodes.size() < 2) {
            return value;
        }

        size_t currentIndex = 0;
        size_t leftChild;
        size_t rightChild;

        size_t greaterNode;
        size_t secondGreaterNode;

        while (true) { //Reorder until in correct order

            leftChild = (2 * currentIndex) + 1; //Calculate left child of current node
            rightChild = (2 * currentIndex) + 2;

            if (nodes.size() > rightChild) { // If both childern are in the heap
                if (compare(nodes[leftChild], nodes[rightChild])) { // Get childern with larger/smaller (depending on heap type) value
                    greaterNode = rightChild;
                    secondGreaterNode = leftChild;
                } else {
                    greaterNode = leftChild;
                    secondGreaterNode = rightChild;
                }

                if (compare(nodes[currentIndex], nodes[greaterNode])) { //If current node larger/smaller (depending on heap type) than first childern, switch values
                    swap(nodes[currentIndex], nodes[greaterNode]);

                    currentIndex = greaterNode; //Move to the switched node for next loop
                    continue;
                } else if (compare(nodes[currentIndex], nodes[secondGreaterNode])) { //If current node larger/smaller (depending on heap type) than second childern, switch values
                    swap(nodes[currentIndex], nodes[secondGreaterNode]);

                    currentIndex = secondGreaterNode;   //Move to the switched node for next loop

                    continue;
                } else { //If cant switch nodes break (node is in its place)
                    break;
                }
            } else if (nodes.size() > leftChild) { //If only one childern in the heap
                if (compare(nodes[currentIndex], nodes[leftChild])) { //If current node larger/smaller  (depending on heap type) than childern, switch values
                    swap(nodes[currentIndex], nodes[leftChild]);

                    currentIndex = leftChild;
                    continue;
                } else { //If cant switch nodes break (node is in its place)
                    break;
                }
            } else { //If no childern break (node is in its place)
                break;
            }
        }

        return value;
    }

    /**
     * @brief Get top element in heap
     *
     * Throws if heap is empty
     *
     * For Max heap largest element, for Min heap smallest element
     *
     * @return unsigned int Top element
     */
    T top() const {
        if (nodes.size() == 0) {
            throw (-1);
        }
        return nodes.front();
    }

    /**
     * @brief Get amount of elements in heap
     *
     * @return size_t amount of elements in heap
     */
    size_t size() const {
        return nodes.size();
    }
};

/**
 * @brief Min heap
 *
 * Adds compare function, rest of logic is handled by LittleHeap
 * @see LittleHeap
 *
 * @tparam T Heap data type
 */
template <typename T>
class MinHeap : public LittleHeap<T> {
    bool compare(T & lhs, T rhs) const override {
        return lhs > rhs;
    }
};

/**
 * @brief Max heap
 *
 * Adds compare function, rest of logic is handled by LittleHeap
 * @see LittleHeap
 *
 * @tparam T Heap data type
 */
template <typename T>
class MaxHeap : public  LittleHeap<T> {
    bool compare(T & lhs, T rhs) const override {
        return lhs < rhs;
    }
};



int main(void) {
    MaxHeap<int> lhMax;
    assert(lhMax.size() == 0);
    lhMax.push(1);
    assert(lhMax.size() == 1);
    assert(lhMax.top() == 1);
    assert(lhMax.pop() == 1);
    assert(lhMax.size() == 0);
    lhMax.push(10);
    lhMax.push(25);
    lhMax.push(5);
    lhMax.push(50);
    lhMax.push(25);
    lhMax.push(7);
    lhMax.push(9);
    assert(lhMax.size() == 7);
    assert(lhMax.top() == 50);
    lhMax.push(1000);
    lhMax.push(1000);
    lhMax.push(124);
    lhMax.push(145);
    lhMax.push(50);
    lhMax.push(347);
    lhMax.push(345);
    assert(lhMax.size() == 14);
    assert(lhMax.pop() == 1000);
    assert(lhMax.pop() == 1000);
    assert(lhMax.pop() == 347);

    MinHeap<int> lhMin;
    assert(lhMin.size() == 0);
    lhMin.push(1);
    assert(lhMin.size() == 1);
    assert(lhMin.top() == 1);
    assert(lhMin.pop() == 1);
    assert(lhMin.size() == 0);
    lhMin.push(10);
    lhMin.push(25);
    lhMin.push(5);
    lhMin.push(50);
    lhMin.push(25);
    lhMin.push(7);
    lhMin.push(9);
    assert(lhMin.size() == 7);
    assert(lhMin.top() == 5);
    lhMin.push(0);
    lhMin.push(0);
    lhMin.push(1);
    lhMin.push(4);
    lhMin.push(50);
    lhMin.push(10000);
    lhMin.push(564);
    assert(lhMin.size() == 14);
    assert(lhMin.pop() == 0);
    assert(lhMin.pop() == 0);
    assert(lhMin.pop() == 1);

    return 0;
}