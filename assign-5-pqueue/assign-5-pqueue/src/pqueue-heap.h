/*
 * File: pqueue-heap.h
 * -------------------
 * This interface defines the HeapPQueue class, which implements
 * the Priority Queue backed by heap for strings.
 */
#pragma once
#include "pqueue.h"
#include "min-heap.h"
#include <string>

/*
 * Class: HeapPQueue
 * -----------------
 * This class models a prioty queue for strings. The functional operations
 * are same as those for VectorPQueue class.
 */
class HeapPQueue : public PQueue {
public:
	HeapPQueue();
	~HeapPQueue();
	
	static HeapPQueue *merge(HeapPQueue *one, HeapPQueue *two);
	
	void enqueue(const std::string& elem);
    std::string extractMin();
    const std::string& peek() const;
    
    //TO-DO: overload + and [] operator
private:
    // provide data methods and helper methods to
    // help realize the binary heap-backed PQueue
    /*private constants*/
    static const unsigned int INITIAL_CAPACITY = 4;
    /* Instance variables */
    MinHeap<std::string> *minheap;
    int capacity;
};
