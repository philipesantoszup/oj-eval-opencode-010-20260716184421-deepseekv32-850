#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
/**
 * a data container like std::list
 * allocate random memory addresses for data and they are doubly-linked in a list.
 */
template<typename T>
class list {
protected:
    class node {
    public:
        T *value;
        node *prev;
        node *next;
        
        node() : value(nullptr), prev(nullptr), next(nullptr) {}
        
        node(const T &val) : prev(nullptr), next(nullptr) {
            value = new T(val);
        }
        
        ~node() {
            if (value != nullptr) {
                delete value;
                value = nullptr;
            }
        }
    };

protected:
    node *head;
    node *tail;
    size_t listSize;
    
    /**
     * insert node cur before node pos
     * return the inserted node cur
     */
    node *insert(node *pos, node *cur) {
        if (pos == head) {
            // insert at beginning
            cur->next = head;
            cur->prev = nullptr;
            head->prev = cur;
            head = cur;
        } else {
            cur->prev = pos->prev;
            cur->next = pos;
            pos->prev->next = cur;
            pos->prev = cur;
        }
        listSize++;
        return cur;
    }
    
    /**
     * remove node pos from list (no need to delete the node)
     * return the removed node pos
     */
    node *erase(node *pos) {
        if (pos == head) {
            head = head->next;
            if (head != nullptr) {
                head->prev = nullptr;
            }
        } else if (pos == tail) {
            tail = tail->prev;
            if (tail != nullptr) {
                tail->next = nullptr;
            }
        } else {
            pos->prev->next = pos->next;
            pos->next->prev = pos->prev;
        }
        
        pos->prev = nullptr;
        pos->next = nullptr;
        listSize--;
        return pos;
    }

public:
    class const_iterator;
    class iterator {
    private:
        node *current;
        const list *parentList;
        
    public:
        iterator(node *n = nullptr, const list *p = nullptr) : current(n), parentList(p) {}
        
        /**
         * iter++
         */
        iterator operator++(int) {
            iterator temp = *this;
            if (current == nullptr) {
                throw invalid_iterator();
            }
            current = current->next;
            return temp;
        }
        
        /**
         * ++iter
         */
        iterator & operator++() {
            if (current == nullptr) {
                throw invalid_iterator();
            }
            current = current->next;
            return *this;
        }
        
        /**
         * iter--
         */
        iterator operator--(int) {
            iterator temp = *this;
            if (current == nullptr) {
                // end() iterator should be able to be decremented to last element
                if (parentList == nullptr || parentList->tail == nullptr) {
                    throw invalid_iterator();
                }
                current = parentList->tail;
            } else {
                current = current->prev;
            }
            return temp;
        }
        
        /**
         * --iter
         */
        iterator & operator--() {
            if (current == nullptr) {
                // end() iterator should be able to be decremented to last element
                if (parentList == nullptr || parentList->tail == nullptr) {
                    throw invalid_iterator();
                }
                current = parentList->tail;
            } else {
                current = current->prev;
            }
            return *this;
        }
        
        /**
         * TODO *it
         * remember to throw if iterator is invalid
         */
        T & operator *() const {
            if (current == nullptr || current->value == nullptr) {
                throw invalid_iterator();
            }
            return *(current->value);
        }
        
        /**
         * TODO it->field
         * remember to throw if iterator is invalid
         */
        T * operator ->() const {
            if (current == nullptr || current->value == nullptr) {
                throw invalid_iterator();
            }
            return current->value;
        }
        
        /**
         * a operator to check whether two iterators are same (pointing to the same memory).
         */
        bool operator==(const iterator &rhs) const {
            return current == rhs.current;
        }
        bool operator==(const const_iterator &rhs) const {
            return current == rhs.getNode();
        }
        
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return current != rhs.current;
        }
        bool operator!=(const const_iterator &rhs) const {
            return current != rhs.getNode();
        }
        
        node* getNode() const { return current; }
        const list* getParentList() const { return parentList; }
    };
    /**
     * TODO
     * has same function as iterator, just for a const object.
     * should be able to construct from an iterator.
     */
    class const_iterator {
    private:
        const node *current;
        const list *parentList;
        
    public:
        const_iterator(const node *n = nullptr, const list *p = nullptr) : current(n), parentList(p) {}
        const_iterator(const iterator &it) : current(it.getNode()), parentList(it.getParentList()) {}
        
        const_iterator operator++(int) {
            const_iterator temp = *this;
            if (current == nullptr) {
                throw invalid_iterator();
            }
            current = current->next;
            return temp;
        }
        
        const_iterator & operator++() {
            if (current == nullptr) {
                throw invalid_iterator();
            }
            current = current->next;
            return *this;
        }
        
        const_iterator operator--(int) {
            const_iterator temp = *this;
            if (current == nullptr) {
                // end() iterator should be able to be decremented to last element
                if (parentList == nullptr || parentList->tail == nullptr) {
                    throw invalid_iterator();
                }
                current = parentList->tail;
            } else {
                current = current->prev;
            }
            return temp;
        }
        
        const_iterator & operator--() {
            if (current == nullptr) {
                // end() iterator should be able to be decremented to last element
                if (parentList == nullptr || parentList->tail == nullptr) {
                    throw invalid_iterator();
                }
                current = parentList->tail;
            } else {
                current = current->prev;
            }
            return *this;
        }
        
        const T & operator *() const {
            if (current == nullptr || current->value == nullptr) {
                throw invalid_iterator();
            }
            return *(current->value);
        }
        
        const T * operator ->() const {
            if (current == nullptr || current->value == nullptr) {
                throw invalid_iterator();
            }
            return current->value;
        }
        
        bool operator==(const iterator &rhs) const {
            return current == rhs.getNode();
        }
        bool operator==(const const_iterator &rhs) const {
            return current == rhs.getNode();
        }
        
        bool operator!=(const iterator &rhs) const {
            return current != rhs.getNode();
        }
        bool operator!=(const const_iterator &rhs) const {
            return current != rhs.getNode();
        }
        
        const node* getNode() const { return current; }
        const list* getParentList() const { return parentList; }
    };
    /**
     * TODO Constructs
     * Atleast two: default constructor, copy constructor
     */
    list() : head(nullptr), tail(nullptr), listSize(0) {}
    
    list(const list &other) : head(nullptr), tail(nullptr), listSize(0) {
        node *current = other.head;
        while (current != nullptr) {
            push_back(*(current->value));
            current = current->next;
        }
    }
    
    /**
     * TODO Destructor
     */
    virtual ~list() {
        clear();
    }
    
    /**
     * TODO Assignment operator
     */
    list &operator=(const list &other) {
        if (this == &other) {
            return *this;
        }
        
        clear();
        
        node *current = other.head;
        while (current != nullptr) {
            push_back(*(current->value));
            current = current->next;
        }
        
        return *this;
    }
    /**
     * access the first / last element
     * throw container_is_empty when the container is empty.
     */
    const T & front() const {
        if (empty()) {
            throw container_is_empty();
        }
        return *(head->value);
    }
    
    const T & back() const {
        if (empty()) {
            throw container_is_empty();
        }
        return *(tail->value);
    }
    
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(head, this);
    }
    
    const_iterator cbegin() const {
        return const_iterator(head, this);
    }
    
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(nullptr, this);
    }
    
    const_iterator cend() const {
        return const_iterator(nullptr, this);
    }
    
    /**
     * checks whether the container is empty.
     */
    virtual bool empty() const {
        return listSize == 0;
    }
    
    /**
     * returns the number of elements
     */
    virtual size_t size() const {
        return listSize;
    }

    /**
     * clears the contents
     */
    virtual void clear() {
        node *current = head;
        while (current != nullptr) {
            node *next = current->next;
            delete current;
            current = next;
        }
        head = tail = nullptr;
        listSize = 0;
    }
    /**
     * insert value before pos (pos may be the end() iterator)
     * return an iterator pointing to the inserted value
     * throw if the iterator is invalid
     */
    virtual iterator insert(iterator pos, const T &value) {
        if (pos.getParentList() != this) {
            throw invalid_iterator();
        }
        
        node *newNode = new node(value);
        
        if (pos.getNode() == nullptr) {
            // Insert at end
            if (tail == nullptr) {
                // Empty list
                head = tail = newNode;
            } else {
                newNode->prev = tail;
                tail->next = newNode;
                tail = newNode;
            }
            listSize++;
            return iterator(newNode, this);
        } else {
            node *current = pos.getNode();
            insert(current, newNode);
            return iterator(newNode, this);
        }
    }
    
    /**
     * remove the element at pos (the end() iterator is invalid)
     * returns an iterator pointing to the following element, if pos pointing to the last element, end() will be returned.
     * throw if the container is empty, the iterator is invalid
     */
    virtual iterator erase(iterator pos) {
        if (empty()) {
            throw container_is_empty();
        }
        if (pos.getParentList() != this || pos.getNode() == nullptr) {
            throw invalid_iterator();
        }
        
        node *current = pos.getNode();
        node *nextNode = current->next;
        
        erase(current);
        delete current;
        
        return iterator(nextNode, this);
    }
    /**
     * adds an element to the end
     */
    void push_back(const T &value) {
        node *newNode = new node(value);
        
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
        listSize++;
    }
    
    /**
     * removes the last element
     * throw when the container is empty.
     */
    void pop_back() {
        if (empty()) {
            throw container_is_empty();
        }
        
        node *oldTail = tail;
        if (head == tail) {
            head = tail = nullptr;
        } else {
            tail = tail->prev;
            tail->next = nullptr;
        }
        
        delete oldTail;
        listSize--;
    }
    
    /**
     * inserts an element to the beginning.
     */
    void push_front(const T &value) {
        node *newNode = new node(value);
        
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        listSize++;
    }
    
    /**
     * removes the first element.
     * throw when the container is empty.
     */
    void pop_front() {
        if (empty()) {
            throw container_is_empty();
        }
        
        node *oldHead = head;
        if (head == tail) {
            head = tail = nullptr;
        } else {
            head = head->next;
            head->prev = nullptr;
        }
        
        delete oldHead;
        listSize--;
    }
    /**
     * sort the values in ascending order with operator< of T
     */
    void sort() {
        if (listSize <= 1) return;
        
        // Create an array to hold values temporarily
        // We need to handle types without default constructor
        char *buffer = new char[listSize * sizeof(T)];
        T *arr = reinterpret_cast<T*>(buffer);
        
        // Copy construct elements into array
        node *current = head;
        for (size_t i = 0; i < listSize; i++) {
            new (&arr[i]) T(*(current->value));
            current = current->next;
        }
        
        // Sort using sjtu::sort
        std::function<bool(const T&, const T&)> cmp = [](const T &a, const T &b) {
            return a < b;
        };
        sjtu::sort(arr, arr + listSize, cmp);
        
        // Copy back to list and destroy temporary objects
        current = head;
        for (size_t i = 0; i < listSize; i++) {
            *(current->value) = arr[i];
            arr[i].~T(); // Destruct temporary
            current = current->next;
        }
        
        delete[] buffer;
    }
    /**
     * merge two sorted lists into one (both in ascending order)
     * compare with operator< of T
     * container other becomes empty after the operation
     * for equivalent elements in the two lists, the elements from *this shall always precede the elements from other
     * the order of equivalent elements of *this and other does not change.
     * no elements are copied or moved
     */
    void merge(list &other) {
        if (this == &other) return;
        
        node *p1 = head;
        node *p2 = other.head;
        node *newHead = nullptr;
        node *newTail = nullptr;
        
        // Merge two sorted lists
        while (p1 != nullptr && p2 != nullptr) {
            node *selected;
            if (*(p1->value) < *(p2->value)) {
                selected = p1;
                p1 = p1->next;
            } else {
                selected = p2;
                p2 = p2->next;
            }
            
            // Detach selected node
            if (selected->prev) selected->prev->next = selected->next;
            if (selected->next) selected->next->prev = selected->prev;
            
            // Add to new list
            selected->prev = newTail;
            selected->next = nullptr;
            
            if (newTail == nullptr) {
                newHead = newTail = selected;
            } else {
                newTail->next = selected;
                newTail = selected;
            }
        }
        
        // Append remaining nodes from p1
        while (p1 != nullptr) {
            node *selected = p1;
            p1 = p1->next;
            
            if (newTail == nullptr) {
                newHead = newTail = selected;
            } else {
                newTail->next = selected;
                selected->prev = newTail;
                newTail = selected;
            }
        }
        
        // Append remaining nodes from p2
        while (p2 != nullptr) {
            node *selected = p2;
            p2 = p2->next;
            
            if (newTail == nullptr) {
                newHead = newTail = selected;
            } else {
                newTail->next = selected;
                selected->prev = newTail;
                newTail = selected;
            }
        }
        
        // Update this list
        head = newHead;
        tail = newTail;
        listSize += other.listSize;
        
        // Clear other list
        other.head = other.tail = nullptr;
        other.listSize = 0;
    }
    /**
     * reverse the order of the elements
     * no elements are copied or moved
     */
    void reverse() {
        if (listSize <= 1) return;
        
        node *current = head;
        node *temp = nullptr;
        
        // Swap prev and next pointers for all nodes
        while (current != nullptr) {
            temp = current->prev;
            current->prev = current->next;
            current->next = temp;
            current = current->prev; // Move to next node (which was prev)
        }
        
        // Swap head and tail
        temp = head;
        head = tail;
        tail = temp;
    }
    /**
     * remove all consecutive duplicate elements from the container
     * only the first element in each group of equal elements is left
     * use operator== of T to compare the elements.
     */
    void unique() {
        if (listSize <= 1) return;
        
        node *current = head;
        while (current != nullptr && current->next != nullptr) {
            if (*(current->value) == *(current->next->value)) {
                // Remove next node
                node *toDelete = current->next;
                current->next = toDelete->next;
                if (toDelete->next != nullptr) {
                    toDelete->next->prev = current;
                } else {
                    tail = current;
                }
                delete toDelete;
                listSize--;
            } else {
                current = current->next;
            }
        }
    }
};

}

#endif //SJTU_LIST_HPP
