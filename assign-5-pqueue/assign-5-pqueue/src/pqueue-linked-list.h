#pragma once
#include "pqueue.h"
#include <string>
#include "sorted-double-linked-list.h"

class LinkedListPQueue : public PQueue {
public:
	LinkedListPQueue();
	~LinkedListPQueue();
	
	static LinkedListPQueue *merge(LinkedListPQueue *one, LinkedListPQueue *two);
	
	void enqueue(const std::string& elem);
    std::string extractMin();
    const std::string& peek() const;
	
private:
	// provide data methods and helper methods to
    // help realize the linked list-backed PQueue
    SortedDoubleLinkedList<std::string> *sdll;
};
