#include "pqueue-heap.h"
using namespace std;

HeapPQueue::HeapPQueue() {}
HeapPQueue::~HeapPQueue() {}

const string& HeapPQueue::peek() const {
    // placeholder so method compiles..
	// replace with your own implementation
    return kEmptyString;
}

string HeapPQueue::extractMin() {
	// placeholder so method compiles..
	// replace with your own implementation
	return peek();
}

void HeapPQueue::enqueue(const string& /* elem */) {
    // placeholder so method compiles..
	// replace with your own implementation
}

HeapPQueue *HeapPQueue::merge(HeapPQueue * /* one */, HeapPQueue * /* two */) {
	// placeholder so method compiles..
	// replace with your own implementation
	return new HeapPQueue();
}
