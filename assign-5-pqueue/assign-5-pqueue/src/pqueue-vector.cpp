#include "pqueue-vector.h"
using namespace std;

VectorPQueue::VectorPQueue() {}
VectorPQueue::~VectorPQueue() {}

const string& VectorPQueue::peek() const {
    // placeholder so method compiles..
	// replace with your own implementation
    return kEmptyString;
}

string VectorPQueue::extractMin() {
	// placeholder so method compiles..
	// replace with your own implementation
	return peek();
}

void VectorPQueue::enqueue(const string& /* elem */) {
	// placeholder so method compiles..
	// replace with your own implementation
}

VectorPQueue *VectorPQueue::merge(VectorPQueue * /* one */, VectorPQueue * /* two */) {
	// placeholder so method compiles..
	// replace with your own implementation
	return new VectorPQueue();
}
