#include "pqueue-binomial-heap.h"
using namespace std;

BinomialHeapPQueue::BinomialHeapPQueue() {
    bnheap = new BinomialMinHeap<string>();
}
BinomialHeapPQueue::~BinomialHeapPQueue() {
    bnheap->~BinomialMinHeap();
}

const string& BinomialHeapPQueue::peek() const {
    return bnheap->peek();
}

string BinomialHeapPQueue::extractMin() {
    logSize --;
    return bnheap->extractMin();
}

void BinomialHeapPQueue::enqueue(const string& elem) {
    logSize ++;
    return bnheap->insert(elem);
}

BinomialHeapPQueue *BinomialHeapPQueue::merge(BinomialHeapPQueue * one, BinomialHeapPQueue * two) {
    BinomialHeapPQueue *bnheapPQueueTotoal = new BinomialHeapPQueue();
    bnheapPQueueTotoal->logSize = one->logSize + two->logSize;
    bnheapPQueueTotoal->bnheap = BinomialMinHeap<string>::merge(*(one->bnheap), *(two->bnheap));
    one->~BinomialHeapPQueue();
    two->~BinomialHeapPQueue();
    return bnheapPQueueTotoal;
}
