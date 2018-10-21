/*
 * File: pqueue-vector.h
 * -------------------
 * This interface defines the HeapPQueue class, which implements
 * the Priority Queue backed by vector for strings.
 */

#pragma once
#include "pqueue.h"
#include <string>
#include "vector.h"

/*
 * Class: HeapPQueue
 * -----------------
 * This class models a prioty queue for strings. The functional operations
 * are same as those for PQueue class.
 */
class VectorPQueue : public PQueue {
public:
	VectorPQueue();
	~VectorPQueue();
	
	static VectorPQueue *merge(VectorPQueue *one, VectorPQueue *two);
	
	void enqueue(const std::string& elem);
    std::string extractMin();
    const std::string& peek() const;
	
private:
	// provide data methods and helper methods to
    // help realize the Vector-backed PQueue
    Vector<std::string> data;
};
