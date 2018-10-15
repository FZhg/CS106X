#pragma once
#include "pqueue.h"
#include <string>

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
};
