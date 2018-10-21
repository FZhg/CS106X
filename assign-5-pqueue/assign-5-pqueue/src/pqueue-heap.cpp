/* #include "pqueue-heap.h"
 * File: pqueue-vector.cpp
 * -------------------
 * This file implements the VectorPQueue class.I thought only write a min-heap for strings
 * is waste of human labor. Thus, I implement a tempalte MinHeap Class seperately.
 * All the heapPQueue methods will be implemented as wrappers.
 */
#include "pqueue-heap.h"
#include "error.h"
using namespace std;

/*
 * Implementation notes: constructor and destructor
 * ------------------------------------------------
 * The constuctor must allocate the array storage for string elements in the HEAP memory
 * and intialize for the fields of the object. The destructoir frees any HEAP memory
 * allocated by the class, which is just the array of elements.
 */
HeapPQueue::HeapPQueue() {
    capacity = INITIAL_CAPACITY;
    minheap = new MinHeap<string>(INITIAL_CAPACITY);
}

HeapPQueue::~HeapPQueue() {
    delete minheap;
}

const string& HeapPQueue::peek() const {
    return minheap->min();
}

string HeapPQueue::extractMin() {
    if(isEmpty()) error("extractMin: No Element in HeapPQueue!");
    logSize -= 1;
    return minheap->extractMin();
}

void HeapPQueue::enqueue(const string& st) {
    logSize += 1;
    minheap->insert(st);
}

HeapPQueue* HeapPQueue::merge(HeapPQueue * one, HeapPQueue  * two) {
    HeapPQueue *merged = new HeapPQueue;
    merged->minheap = MinHeap<string>::merge(*one->minheap, *two->minheap);
    merged->logSize = one->logSize + two->logSize;
    one->~HeapPQueue();
    two->~HeapPQueue();
    return merged;
}
