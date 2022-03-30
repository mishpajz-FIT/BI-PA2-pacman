#include <cstdlib>
#include <cassert>

using namespace std;

/**
 * @brief Data container
 *
 * Supports O(1) insertion and removal.
 * Does not have fast random access.
 *
 * @tparam T Datatype
 */
template <typename T>
class List {
    // Implemented as doubly linked list


    /**
     * @brief List Node which stores element of List
     *
     * Linked list node.
     *
     */
    struct Node {
        T value; // Data
        Node * next; // Next node
        Node * prev; // Previous node

        Node(T newValue) : value(newValue), next(nullptr), prev(nullptr) { }

        Node(const Node & toCopy) : value(toCopy.value), next(nullptr), prev(nullptr) { }
    };

    Node * head; // First Node
    Node * tail; // Last Node

    size_t size; // Nodes in this list (updated manually by insertion/deletion)

    /**
     * @brief Insert element before Node
     *
     * If pointer to Node is nullptr, inserts at end.
     *
     * @param before Pointer to Node before which to insert element
     * @param value Element which should be inserted
     */
    void insert(Node * before, const T & value) {
        Node * newNode = new Node(value);

        if (before == nullptr) { //If inserting at end, set new tail
            newNode->prev = tail;
            tail = newNode;
        } else { //Set new node's prev from 'before' node's prev, set before's node prev as new node
            newNode->prev = before->prev;
            before->prev = newNode;
        }

        if (newNode->prev == nullptr) { //If at begin, set new hail
            head = newNode;
        } else { //Set next of new node's prev to new node
            (newNode->prev)->next = newNode;
        }
        newNode->next = before; //Set next of new node to before

        size++;
    }

    /**
     * @brief Remove Node
     *
     * @param at Pointer to Node which should be removed
     * @return T Element that was removed
     */
    T remove(Node * at) {
        T value(at->value); // Store value that will be removed

        if (at->prev != nullptr) {
            at->prev->next = at->next; // Set next of previous Node
        } else {
            head = at->next; // Set new head if removing current head
        }

        if (at->next != nullptr) {
            at->next->prev = at->prev; // Set previous of next Node
        } else {
            tail = at->prev; // Set new tail if removing current tail
        }

        delete at;
        size--;

        return value;
    }

public:
    /**
     * @brief Iterator for List
     *
     * Support forward/backward movement by one, element is accessed by dereferencing.
     * Can also deduce if at end of the list, however, if at end can not be moved forward nor backward.
     *
     */
    struct ListIterator {
        Node * ptr; // Pointer to Node which this Iterator represents

        ListIterator(const List & list, size_t position) : ptr(list.head) {
            for (size_t i = 0; i < position; i++) {
                ptr = ptr->next;
            }
        }

        ListIterator(Node * pointer = nullptr) : ptr(pointer) { }

        T & operator * () {
            return (*ptr).value;
        }

        const T & operator * () const {
            return (*ptr).value;
        }

        bool isAtEnd() {
            return (ptr == nullptr);
        }

        ListIterator & operator ++() {
            ptr = ptr->next;
            return (*this);
        }

        ListIterator operator ++(int) {
            ListIterator old(*this);
            ++(*this);
            return old;
        }

        ListIterator & operator --() {
            ptr = ptr->prev;
            return (*this);
        }

        ListIterator operator --(int) {
            ListIterator old(*this);
            --(*this);
            return old;
        }
    };

    List() : head(nullptr), tail(nullptr), size(0) { }

    List(const List & copyFrom) : head(nullptr), tail(nullptr), size(copyFrom.size) {
        if (copyFrom.head != nullptr) {
            head = new Node(*(copyFrom.head)); // Copy head
            Node * iter = head;
            Node * iterCopy = copyFrom.head->next;
            while (iterCopy != nullptr) { // Iteratively copy all nodes
                iter->next = new Node(*(iterCopy)); // Set next and allocate new Node
                iter->next->prev = iter; // Set prev
                iter = iter->next; // Move iterators
                iterCopy = iterCopy->next;
            }
            tail = iter;
        }
    }

    ~List() {
        Node * iter = head;
        while (iter != nullptr) { // Iteratively dealloc nodes
            Node * tmp = iter->next;
            delete iter;
            iter = tmp;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    List & operator = (const List & toAssign) {
        if (this == &toAssign) {
            return *this;
        }

        // Copy list and swap all values
        List copy(toAssign);

        size = copy.size;

        Node * tmp = head;
        head = copy.head;
        copy.head = tmp;

        tmp = tail;
        tail = copy.tail;
        copy.tail = tmp;

        return (*this);
    }

    /**
     * @brief Insert element before Iterator
     *
     * Moves Iterator to inserted element.
     *
     * @param before Iterator before which to insert element
     * @param value Element which should be inserted
     */
    void insert(ListIterator & before, const T & value) {
        insert(before.ptr, value);

        // Move iterator to inserted Node
        if (before.ptr == nullptr) {
            before.ptr = tail;
        } else {
            before.ptr = (before.ptr)->prev;
        }
    }

    /**
     * @brief Remove element selected by Iterator
     *
     * Moves Iterator to next element after removed element.
     *
     * @param at Iterator which element should be removed
     * @return T Element that was removed
     */
    T remove(ListIterator & at) {
        ListIterator old = at;
        at++;
        return remove(old.ptr);
    }

    /**
     * @brief Insert element at beggining of List
     *
     * @param value Element which should be inserted
     */
    void pushFront(const T & value) {
        insert(head, value);
    }

    /**
     * @brief Remove element at beggining of List
     *
     * @return T Element that was removed
     */
    T popFront() {
        return remove(head);
    }

    /**
     * @brief Insert element at end of List
     *
     * @param value Element which should be inserted
     */
    void pushBack(const T & value) {
        insert(nullptr, value);
    }

    /**
     * @brief Remove element at end of List
     *
     * @return T Element that was removed
     */
    T popBack() {
        return remove(tail);
    }

    /**
     * @brief Get last element in list
     *
     * @return T& Reference to last element
     */
    T & back() {
        return tail->value;
    }

    const T & back() const {
        return tail->value;
    }

    /**
     * @brief Get first element in list
     *
     * @return T& Reference to first element
     */
    T & front() {
        return head->value;
    }

    const T & front() const {
        return head->value;
    }

    /**
     * @brief Get iterator to beggining of list
     *
     * @return ListIterator Iterator
     */
    ListIterator begin() const {
        return ListIterator(head);
    }

    /**
     * @brief Get iterator to end of list
     *
     * Note this iterator points behind last element of list, can't be increased or decreased and does not point to any value.
     *
     * @return ListIterator Iterator
     */
    ListIterator end() const {
        return ListIterator(nullptr);
    }

    /**
     * @brief
     *
     * @param at
     * @return ListIterator
     */
    ListIterator iterator(size_t at = 0) const {
        return ListIterator(*this, at);
    }

    /**
     * @brief Amount of elements in List
     *
     * @return size_t Amount of elements
     */
    size_t getSize() const {
        return size;
    }
};