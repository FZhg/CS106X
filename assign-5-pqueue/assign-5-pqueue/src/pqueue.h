/**
 * File: pqueue.h
 * --------------
 * Defines the interface that all PQueues must implement.  VectorPQueue,
 * HeapPQueue, and BinomialHeapPQueue all subclass PQueue so that they
 * all agree on the same interface.
 */

#pragma once
#include <string>

/**
 * Declared as a shared global just so the placeholder
 * peek() methods in all four subclasses have something
 * permanent to return a reference to.  Your final
 * implementation shouldn't make use of kEmptyString
 * anywhere at all.
 */

static const std::string kEmptyString;

/**
 * Defines the PQueue container type for strings. The priority queue
 * holds all enqueued strings, but rather than making them accessible
 * in a FIFO manner, extractMin produces the alphabetically smaller strings
 * before the alphabetically larger ones, regardless of insertion order.
 * 
 * The generalization of this would be a templated priority queue, useful
 * in a large number of algorithmic settings (optimization problems, simulations,
 * shortest path problems, network routing, etc.)
 */

class PQueue {
public:
	
/**
 * Defines a small set of constants that can be used to identify
 * the particular implementation of interest.  They're most vital to
 * the construction of a PQueue using the factory method, as with
 *
 *     PQueue *pq = PQueue::createPQueue(PQueue::BinomialHeap);
 *    
 * Enumerated constants are programmatically easier to manage than
 * type names, and it's easy to internally dispatch on enumerated
 * type constants than it is to on type names.
 */
	
	enum PQueueType {
		UnsortedVector, LinkedList, Heap, BinomialHeap
	};
	
/**
 * Convenience function that gives us a string name for the
 * PQueue represented by type.
 */
	
	static std::string typeToName(PQueueType type);
	
public:
    
/**
 * Manages the initialization of the material managed at the PQueue,
 * base class, level.  We assume that all subclasses, regardless of
 * implementation, track their logical size using the logSize
 * parameter held at the PQueue class level.  By doing so, we can
 * rely on a single implementation of the isEmpty() and size() methods
 * that never have to be overridden.  Construction at this level is
 * so obvious that we just inline the implementation.
 */

	PQueue() { logSize = 0; }

/**
 * Disposes of any external resources held at the PQueue level.
 * Nothing, save for an internal int, is managed at the PQueue level,
 * so the destructor is inlined here.
 */
	
	virtual ~PQueue() {}
	
	static PQueue *createPQueue(PQueueType type);
	static PQueue *merge(PQueue *one, PQueue *two);

/**
 * Returns the number of elements inside the PQueue.  This
 * method should not be overridden, and subclasses properly
 * maintain the value of logSize so that the implementation
 * provided here is always correct.
 */
	
	int size() const { return logSize; }

/**
 * Returns true if and only if the PQueue is empty.  Self-explanatory,
 * save for the fact that isEmpty should not be overridden.
 */
	
	bool isEmpty() const { return size() == 0; }

/**
 * Inserts the provided string into the priority queue as the
 * implementation sees fit.  The virtual keyword, paired with the
 * = 0, is C++ notation stating that no sensible implementation
 * exists at this level, and that concrete subclasses should 
 * provide one.
 */
	
	virtual void enqueue(const std::string& elem) = 0;

/**
 * Identifies, removes, and returns the alphabetically smallest string
 * in the priority queue.  The behavior is undefined if called against
 * an empty queue.  The virtual keyword, paired with the
 * = 0, is C++ notation stating that no sensible implementation
 * exists at this level, and that concrete subclasses should
 * provide one.
 */

	virtual std::string extractMin() = 0;

/**
 * Returns an immutable reference to the lexicographically smallest
 * item currently within the queue. The virtual keyword, paired with the
 * = 0, is C++ notation stating that no sensible implementation
 * exists at this level, and that concrete subclasses should
 * provide one.
 */

    virtual const std::string& peek() const = 0;
		
protected:

/**
 * Maintains a copy of the size of the priority queue, regardless of the
 * subclass's implementation.  Care much be taken to ++ and -- this
 * field (visible to all suclasses because of the protected access modifier)
 * with each call to enqueue and extractMin, and that the logSize is properly
 * updated with each merge.
 */
	
	int logSize;
};
