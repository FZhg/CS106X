#include "pqueue-binomial-heap.h"
using namespace std;

BinomialHeapPQueue::BinomialHeapPQueue() {}
BinomialHeapPQueue::~BinomialHeapPQueue() {}

const string& BinomialHeapPQueue::peek() const {
    // placeholder so method compiles..
	// replace with your own implementation
    return kEmptyString;
}

string BinomialHeapPQueue::extractMin() {
	// placeholder so method compiles..
	// replace with your own implementation
	return peek();
}

void BinomialHeapPQueue::enqueue(const string& /* elem */) {
    // placeholder so method compiles..
	// replace with your own implementation
}

BinomialHeapPQueue *BinomialHeapPQueue::merge(BinomialHeapPQueue */* one */, BinomialHeapPQueue * /* two */) {
	// placeholder so method compiles..
	// replace with your own implementation
	return new BinomialHeapPQueue();
}
