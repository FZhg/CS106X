/*
 * File: pqueue-vector.cpp
 * -------------------
 * This file implements the VectorPQueue class.
 */
#include "pqueue-vector.h"
#include "error.h"
using namespace std;

VectorPQueue::VectorPQueue() {}
VectorPQueue::~VectorPQueue() {}

const string& VectorPQueue::peek() const {
    if (isEmpty()){
     error("peek: Attempting to extractMin an empty VectorPQue");
    }
    else{
        return data[data.size() - 1];
    }
}

string VectorPQueue::extractMin() {
    if(isEmpty()) error("extractMin: Attempting to extractMin an empty VectorPQue");
    //find the min prority string
    int minIndex = 0;
    for(size_t i=0 ; i < data.size(); i ++){
        minIndex = data[i] < data[minIndex]? i : minIndex;
    }
    string minPriority = data[minIndex];
    data.remove(minIndex);
    logSize = data.size();
    return minPriority;
}

void VectorPQueue::enqueue(const string& elem) {
    data.add(elem);
    logSize = data.size();
}

VectorPQueue *VectorPQueue::merge(VectorPQueue * one, VectorPQueue * two) {
    VectorPQueue *merged = new VectorPQueue;
    merged->data = one->data + two->data;
    merged->logSize = merged->data.size();
//    //destructively
    one->~VectorPQueue();
    two->~VectorPQueue();
    return merged;
}
