#include "pqueue-linked-list.h"
using namespace std;

LinkedListPQueue::LinkedListPQueue() {
    sdll = new SortedDoubleLinkedList<string>();
}
LinkedListPQueue::~LinkedListPQueue() {
    delete sdll;
}

const string& LinkedListPQueue::peek() const {
    // placeholder so method compiles..
	// replace with your own implementation
    return sdll->get(0);
}

string LinkedListPQueue::extractMin() {
    const string min = sdll->get(0);
    sdll->remove(0);
    logSize --;
    return min;
}

void LinkedListPQueue::enqueue(const string& st) {
    logSize ++;
    sdll->insert(st);
}

LinkedListPQueue *LinkedListPQueue::merge(LinkedListPQueue * one, LinkedListPQueue * two) {
    LinkedListPQueue *merged = new LinkedListPQueue;
    merged->logSize = one->logSize + two->logSize;
    merged->sdll = SortedDoubleLinkedList<string>::merge(*one->sdll, *two->sdll);
    one->~LinkedListPQueue();
    two->~LinkedListPQueue();
    return merged;
}

