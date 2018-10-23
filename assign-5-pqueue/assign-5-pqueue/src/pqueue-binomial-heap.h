#pragma once
#include "pqueue.h"
#include <string>
#include "binomial-heap.h"

class BinomialHeapPQueue : public PQueue {
public:
    BinomialHeapPQueue();
    ~BinomialHeapPQueue();
	
    static BinomialHeapPQueue *merge(BinomialHeapPQueue *one, BinomialHeapPQueue *two);
	
	void enqueue(const std::string& elem);
    std::string extractMin();
    const std::string& peek() const;
	
private:
    BinomialMinHeap<std::string> *bnheap;
};

