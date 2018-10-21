/*
 * File:sorted-double-linked-list.h
 * --------------------------------
 * This interface exports a primitive version a double linked list and a  sorted
 * double linked-list class.
 * Attention: your valueType must support < or > operator.
 */
#ifndef SORTEDDOUBLELINKEDLIST_H
#define SORTEDDOUBLELINKEDLIST_H

#include "error.h"
#include <iostream>
#include "vector.h"

template <typename ValueType>
class SortedDoubleLinkedList
{
public:
    /*
     * Constructor: DoubleLinkedList
     * Usgae: DoubleLinkedList<ValueType> sdll;
     * ---------------------------------------
     * Intilize an empty double linked list
     */
    SortedDoubleLinkedList();

    /*
     * Constructor: ~DoubleLiknedList
     * Usgae: (usually implicit)
     * ------------------------------
     * Frees any heap strage associated with this double linked list
     */
    ~SortedDoubleLinkedList();
    /*
     * Method: get
     * Usage: sdll.get(index);
     * ----------------------
     * get the entry at index in the list.
     */
    ValueType get(const unsigned int index) const;

    /*
     * Method: size
     * Usage: sdll.size();
     * ------------------
     * get the number of the entries in the list;
     */
    unsigned int size() const;

    /*
     * Method: isEmpty();
     * Usage: sdll.isEmpty();
     * ---------------------
     * True if no entries in the list;
     */
    bool isEmpty() const;

    /*
     * Method: insert
     * Usage: sdll.insert(value);
     * -------------------------
     * insert an entry into the list, while maintaining the sorted order.
     */
    void insert(const ValueType value);

    /*
     * Method: remove
     * Usage: sdll.remove(index);
     * -------------------------
     * remove the entry at index in the list while maintaining the sorted order.
     */
    void remove(const unsigned int index);

    /*
     * Method: insertAll
     * Usage: minheap.insertAll(array);
     * ---------------------------------
     * Insert all the data entries of the array
     */
    void insertAll(const Vector<ValueType>& vec);

    /*
     * Method: merge
     * Usage: sdllTotal = merge(sdll1, sdll2);
     * ------------------------------------
     * Returns a union of two sorted double linked list.
     */
    static SortedDoubleLinkedList<ValueType>* merge(const SortedDoubleLinkedList& sdll1,
                                                    const SortedDoubleLinkedList& sdll2);

    /*
     * Method: display
     * Usage: sdll.display();
     * -------------------------
     * Show the data in the array order; mainly used by debug;
     */
    void display() const;


    /*
     * Method: testSortedness
     * Usage: sdll.testSortedness();
     * -------------------------------
     * return true if in order
     */
    bool testSortedness() const;

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

    /*
     * Implementation notes:
     * ---------------------
     * The SortedDoubleLinkedList is based double-linked list; The insertion will
     * take O(N) time. However remove will be constant time. When the list is empty,
     * the head pointer will be set to NULL.
     */
private:
    /* Type for the double linked list cell */
    struct Cell
    {
        ValueType value;
        Cell *prev=NULL;
        Cell *next=NULL;
        Cell(const ValueType valuePara){
            value = valuePara;
        }
        bool operator <(Cell& c2){
            return value < c2.value;
        }
    };
    /* Instance Varibles */
    unsigned int count;
    Cell* head;
    Cell* tail;

    /* Private Methods */
    /*
     * Implementation Notes: getCell
     * ------------------------------
     * A little helper function for other methods.
     */
    Cell* getCell(const unsigned int index) const{
        if(isEmpty()) error("getCell: no element in the list!");
        if(index >= count) error("getCell: illegal index!");
        Cell* curr = head;
        if(index == 0) return curr; // the first element
        for(unsigned int i = 0; i < index; i++){
            curr = curr->next;
        }
        return curr;
    }
};

/*
 * Implementation Notes: DoubleLinkedList
 * ---------------------------------------
 * In this implementation, all entries are chained by a double link list.
 * This double linked list is sorted throughout the whole process.
 */
template<typename ValueType>
SortedDoubleLinkedList<ValueType>::SortedDoubleLinkedList(){
    count = 0;
    head = NULL;
    tail = NULL;
}

template<typename ValueType>
SortedDoubleLinkedList<ValueType>::~SortedDoubleLinkedList(){
    while(!isEmpty()){
        Cell* oldHead = head;
        head  = head->next;
        delete oldHead;
        count --;
    }
}

/*
 * Implementation notes: get, remove
 * --------------------------
 * No need to check upon the index range and empty list case.
 * Since we have decompose that part into getCell helper function.
 */
template<typename ValueType>
ValueType SortedDoubleLinkedList<ValueType>::get(const unsigned int index) const{
    return  getCell(index)->value;
}

template<typename ValueType>
unsigned int SortedDoubleLinkedList<ValueType>::size() const{
    return count;
}

template<typename ValueType>
bool SortedDoubleLinkedList<ValueType>::isEmpty() const{
    return count == 0;
}

/*
 * Implementation notes: Insert
 * --------------------------
 * Take care of the empty list case;
 * Compare one entry a time.(That is why we need skip list.
 * Take care of all special cases!!
 */
template<typename ValueType>
void SortedDoubleLinkedList<ValueType>::insert(const ValueType value){
    Cell *newCell = new Cell(value);
    if(head == NULL){
        head = newCell;
        tail = newCell;
    } else{
        Cell* currCell = head;
        while((currCell->next != NULL)&&(*currCell < *newCell)){
                currCell = currCell->next; // hit the tail or
                // just bigger than newCell
            }
        if((currCell->next == NULL) && (currCell->prev == NULL)){
            if(*currCell < *newCell){
                currCell->next = newCell;
                newCell->prev = currCell;
                tail = newCell;
            } else{
                currCell->prev = newCell;
                newCell->next = currCell;
                head = newCell;
            }
        }else if((currCell->prev == NULL) && (currCell->next != NULL)){
            currCell->prev = newCell;
            newCell->next = currCell;
            head = newCell;  // head
        } else if ((currCell->next == NULL) && (currCell->prev != NULL)) {
            if(*currCell < *newCell){
                currCell->next = newCell;
                newCell->prev = currCell;
                tail = newCell;
            }  else{
                Cell *prevCell = currCell->prev;
                prevCell->next = newCell;
                newCell->prev = prevCell;
                currCell->prev = newCell;
                newCell->next = currCell;
            }
        }else{
            Cell *prevCell = currCell->prev;
            prevCell->next = newCell;
            newCell->prev = prevCell;
            currCell->prev = newCell;
            newCell->next = currCell;
        }
    }
    count ++;
}

template<typename ValueType>
void SortedDoubleLinkedList<ValueType>::remove(const unsigned int index){
    Cell *toBeRemvoed = getCell(index);
    count --;
    if((toBeRemvoed->next == NULL)&&(toBeRemvoed->prev == NULL)){
        head = NULL; // need to remove the head and the tail
        tail = NULL;
        delete toBeRemvoed;
        return;
    }else if((toBeRemvoed->prev == NULL) && (toBeRemvoed->next != NULL)) {
        //head not tail
        head = toBeRemvoed->next;
        head->prev = NULL;
        delete toBeRemvoed;
        return;
    } else if ((toBeRemvoed->next == NULL) && (toBeRemvoed->prev != NULL)) {
        tail = toBeRemvoed->prev; // need to remove the tail
        tail->next = NULL;
        delete toBeRemvoed;
        return;
    } else{
        Cell *prevCell = toBeRemvoed->prev;
        prevCell->next = toBeRemvoed->next;
        Cell *nextCell  = toBeRemvoed->next;
        nextCell->prev = prevCell;
        delete toBeRemvoed;
    }
}

template<typename ValueType>
SortedDoubleLinkedList<ValueType>* SortedDoubleLinkedList<ValueType>::merge(const SortedDoubleLinkedList& sdll1,
                                                                                 const SortedDoubleLinkedList& sdll2){
    SortedDoubleLinkedList<ValueType> *sdllTotal = new SortedDoubleLinkedList();
    sdllTotal->count = sdll1.count + sdll2.count;
    unsigned int index1 = 0;
    unsigned int index2 = 0;
    while ((index1 < sdll1.count) && (index2 < sdll2.count)) {
        Cell *c1 = new Cell(sdll1.getCell(index1)->value);
        Cell *c2 = new Cell(sdll2.getCell(index2)->value);
        if(*c1 < *c2){
            index1 ++;
            if(sdllTotal->tail == NULL) {
                // initails
                sdllTotal->head = c1;
                sdllTotal->tail = c1;
            } else{
                sdllTotal->tail->next = c1;
                c1->prev = sdllTotal->tail;
                sdllTotal->tail = c1;
            }
        } else{
            index2 ++;
            if(sdllTotal->tail == NULL) {
                // initails
                sdllTotal->head = c2;
                sdllTotal->tail = c2;
            } else{
                sdllTotal->tail->next = c2;
                c2->prev = sdllTotal->tail;
                sdllTotal->tail = c2;
            }
        }
    }
    // watch for the remains
    while(index1 < sdll1.count){
        Cell *c = new Cell(sdll1.getCell(index1)->value);
        sdllTotal->tail->next = c;
        c->prev = sdllTotal->tail;
        sdllTotal->tail = c;
        index1 ++;
    }
    while (index2 < sdll2.count) {
        Cell *c = new Cell(sdll2.getCell(index2)->value);
        sdllTotal->tail->next = c;
        c->prev = sdllTotal->tail;
        sdllTotal->tail = c;
        index2 ++;
    }
    return sdllTotal;

}

template<typename ValueType>
void SortedDoubleLinkedList<ValueType>::insertAll(const Vector<ValueType>& vec){
    for(int i =0; i < vec.size(); i ++){
        insert(vec[i]);
    }
}

template<typename ValueType>
void SortedDoubleLinkedList<ValueType>::display() const{
    std::cout << "{";
    Cell *curr = head;
    std::cout << head->value;
    while(curr->next != NULL){
        curr = curr->next;
        std::cout << ", " << curr->value;
    }
    std::cout << "}" << std::endl;
}

template<typename ValueType>
bool SortedDoubleLinkedList<ValueType>::testSortedness() const{
    Cell *currCell = head;
    unsigned int index = 0;
    while(currCell->next != NULL){
        index ++;
        if(!(*currCell < *currCell->next)){
            std::cout << index << ": " << currCell->value << std::endl;
            std::cout << (index  + 1) << ": " << currCell->next->value << std::endl;
            error("testSortedness: violate sortedness!");
            return false;
        }
    }
    return true;
}
#endif // SORTEDDOUBLELINKEDLIST_H
