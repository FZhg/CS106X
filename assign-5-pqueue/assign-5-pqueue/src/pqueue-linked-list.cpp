#include "pqueue-linked-list.h"
using namespace std;

LinkedListPQueue::LinkedListPQueue() {}
LinkedListPQueue::~LinkedListPQueue() {}

const string& LinkedListPQueue::peek() const {
    // placeholder so method compiles..
	// replace with your own implementation
    return kEmptyString;
}

string LinkedListPQueue::extractMin() {
	// placeholder so method compiles..
	// replace with your own implementation
	return peek();
}

void LinkedListPQueue::enqueue(const string& /* elem */) {
	// placeholder so method compiles..
	// replace with your own implementation
}

LinkedListPQueue *LinkedListPQueue::merge(LinkedListPQueue * /* one */, LinkedListPQueue * /* two */) {
	// placeholder so method compiles..
	// replace with your own implementation
	return new LinkedListPQueue();
}

