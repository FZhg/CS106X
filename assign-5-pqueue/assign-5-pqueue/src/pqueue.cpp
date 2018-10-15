#include "pqueue.h"
#include "pqueue-binomial-heap.h"
#include "pqueue-heap.h"
#include "pqueue-vector.h"
#include "pqueue-linked-list.h"
#include "error.h"
#include <typeinfo>
using namespace std;

/**
 * Convenience function that converts a PQueue type
 * to a string, so that information about PQueue type can
 * be printed.
 */
string PQueue::typeToName(PQueueType type) {
	switch (type) {
		case UnsortedVector: return "VectorPQueue";
		case LinkedList: return "LinkedListPQueue";
		case Heap: return "HeapPQueue";
		case BinomialHeap: return "BinomialHeapPQueue";
		default: error("PQueue type not recognized.");
				 return "";
			
	}
}

/**
 * Factory method that produces an empty, dynamically
 * allocated PQueue instance of the identified type.
 * This abstraction is a common one when selecting among
 * several different implementations that match the same
 * exact interface.
 */
PQueue *PQueue::createPQueue(PQueueType type) {
	switch (type) {
		case UnsortedVector: return new VectorPQueue();
		case LinkedList: return new LinkedListPQueue();
		case Heap: return new HeapPQueue();
		case BinomialHeap: return new BinomialHeapPQueue();
		default: error("PQueue type not recognized.");
				 return NULL;

	}
}

/**
 * Generic merge method that accepts two dynamically allocated
 * PQueue instances, confirms that they're each of the same class,
 * and then manually calls the relevant merge operation on the
 * subclass.  To whatever extent you want to, you should ignore
 * the implementation of this particular method, treating it
 * as nothing more than glue to connect the generate call
 * to the type-specific implementation that knows how to do the merge.
 */
PQueue *PQueue::merge(PQueue *one, PQueue *two) {
	if (one == NULL || two == NULL) {
		error("PQueues passed to PQueue::merge must be non-NULL.");
	}
	
	if (typeid(*one) != typeid(*two)) {
		error("PQueues passed to PQueue::merge must be of the same type.");
	}

	if (dynamic_cast<VectorPQueue *>(one)) {
		return VectorPQueue::merge(dynamic_cast<VectorPQueue *>(one), dynamic_cast<VectorPQueue *>(two));
	}
	
	if (dynamic_cast<LinkedListPQueue *>(one)) {
		return LinkedListPQueue::merge(dynamic_cast<LinkedListPQueue *>(one), dynamic_cast<LinkedListPQueue *>(two));
	}
	
	if (dynamic_cast<HeapPQueue *>(one)) {
		return HeapPQueue::merge(dynamic_cast<HeapPQueue *>(one), dynamic_cast<HeapPQueue *>(two));
	}
	
	if (dynamic_cast<BinomialHeapPQueue *>(one)) {
		return BinomialHeapPQueue::merge(dynamic_cast<BinomialHeapPQueue *>(one), dynamic_cast<BinomialHeapPQueue *>(two));
	}
	
	return NULL;
}

