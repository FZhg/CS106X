/*
 * File:min-heap.h
 * ---------------
 * This interface exports a simplified version of the MinHeap class.
 */
#ifndef _minheap_h
#define _minheap_h

#include "error.h"
#include <iostream>
#include "vector.h"
/*
 * Class: MinHeap<ValueType>
 * ----------------------------------
 * This class miantains a limited  version of array-based heap structure for values of the specified
 *  type; This type must support less or greater operator.
 */
template <typename ValueType>
class MinHeap{
public:
    /*
     * Constructor:MinHeap
     * Usage: MinHeap<ValueType> minHeap;
     * ----------------------------------
     * Intialize a new empty minHeap for the upcoming data of specified data.
     */
    MinHeap(int sizeHint = 10001);

    /*
     * Destructor: ~MinHeap()
     * Usage: (usually implicit used by the complier when the current object
     * is out scope.)
     * ---------------------------------------------------------------------
     * Frees any memory of the current object
     */
    ~MinHeap();

    /*
     * Method: size
     * Usage: int nEntries = minHeap.size();
     * ------------------------------------
     * Returns the number of the nodes in the current min-heap tree.
     */
    int size() const;

    /*
     * Method: isEmpty
     * Usage: if(minHeap.isEmpty()) ...
     * --------------------------------
     * Returns true if the current min-heap tree contains no nodes.
     */
    bool isEmpty() const;

    /*
     * Method: insert
     * Usage: minHeap.insert(value);
     * ----------------------------
     * Insert the value into the current min-heap tree while maintaining its property
     */
    void insert(const ValueType &value);

    /*
     * Method: min
     * Usage: ValueType minValue = minHeap.min();
     * ------------------------------------------
     * return the min value in min-heap tree, which is the node.
     */
    ValueType min() const;

    /*
     * Method: extractMin
     * Usage: ValueType minValue = minHeap.extractMin();
     * -------------------------------------------------
     * return and remove the root of the min-heap tree with its properties intact.
     */
    ValueType extractMin();


    /* Method: merge
     * Usage: minHeapTotal = merge(minheap2 ,minheap2);
     * ------------------------------------------
     * Returns the union of the two minHeap.
     */
    static MinHeap<ValueType>* merge(MinHeap& minheap1, MinHeap& minheap2);

    /*
     * Method: display
     * Usage: minheap.display();
     * -------------------------
     * Show the data in the array order; mainly used by debug;
     */
    void display() const;

    /*
     * Method: insertAll
     * Usage: minheap.insertAll(array);
     * ---------------------------------
     * Insert all the data entries of the array
     */
    void insertAll(const Vector<ValueType>& vec);

    /*
     * Method: testMinHeapity
     * Usage: minheap.testMinHeapity();
     * --------------------------------
     * testify every parent and its children about the min-heap tree property.
     */
    bool testMinHeapity();

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

    /*
     * Implementation notes:
     * ---------------------
     * The MinHeap class is represented using a array.
     */
private:
/* Instance variables */
    ValueType *array; /* Dynamic array of data of the Valuetypes */
    unsigned int capacity; /* The capacity of the dynamic array */
    unsigned int count; /* The current number of entries in the dyanamic array */

/* Private Method prototypes */
    /*
     * Private Method: expandCapacity
     * Usage: expandCapacity();
     * -------------------------------
     * when the array is full, double the capactiy of the array and copy
     * every entries into the larger new array and delete the old smaller
     * array.
     */
    void expandCapacity(){
        if(count == capacity){
            ValueType* oldArray = array;
            capacity *= 2;
            array = new ValueType[capacity + 1];
            for(unsigned int i = 1; i <= count; i ++){
                array[i] = oldArray[i];
            }
            delete[] oldArray;
        }
    }

    /*
     * Private Method: shrinkCapacity
     * Usage: shrinkCapacity();
     * ---------------------------------
     * When the array is  only 1/4 full, shrink the capacity of the array to its half.
     * Still copy every entries into the larger new array and delete the old smaller array.
     */
    void shrinkCapacity(){
        if(count <= capacity / 4){
            ValueType* oldArray = array;
            capacity = 2 * count;
            array = new ValueType[capacity  + 1];
            for(unsigned int i = 1; i <= count; i ++){
                array[i] = oldArray[i];
            }
            delete[] oldArray;
        }
    }

    /*
     * Private method: heapify
     * Usage: minHeap.heapify(index);
     * ------------------------------
     * heapify the subtree rooted at node of this index.
     * This method will always assume that the largest part of min-heap tree is still maintaining the
     * property. This method will take the O(log(N)), which is the worst case.
     */
    void heapify(int rootIndex){
        if(rootIndex <=0) error("heapify: start at root index 1 or bigger");
        if(isEmpty()) error("heapify: no element in the minheap!");
        unsigned int leftChildIndex = getLeftChildIndex(rootIndex);
        unsigned int minIndex = leftChildIndex <= count &&
                array[leftChildIndex] < array[rootIndex] ? leftChildIndex : rootIndex;
        unsigned int rightChildIndex = getRightChildIndex(rootIndex);
        minIndex = rightChildIndex <= count && array[rightChildIndex] < array[minIndex] ?
                    rightChildIndex : minIndex;
        if(minIndex != rootIndex){
            // exchange the root with the min
            ValueType temp = array[rootIndex];
            array[rootIndex] = array[minIndex];
            array[minIndex] = temp;
            // recursively minheapify
            heapify(minIndex);
        }

    }

    /*
     * Private method: getParentIndex
     * Usage: int parentIndex = getParentIndex(childIndex);
     * ----------------------------------------------------
     * return the index of the parent node in the min-heap tree.
     */
    unsigned int getParentIndex(const unsigned int childIndex) const{
        return childIndex / 2;
    }

    /*
     * Private method: getLeftChildIndex
     * Usage: int leftChildIndex = getLeftChildIndex(parentIndex);
     * -----------------------------------------------------------
     * return the index of the left child node in the min-healp tree.
     */
    unsigned int getLeftChildIndex(const unsigned int parentIndex) const {
        return 2 * parentIndex;
    }

    /*
     * Private method: getRightChildIndex;
     * Usage: int rightChildIndex = getRightChildIndex(parentIndex);
     * return the index of the right child node in the min-heap tree.
     */
    unsigned int getRightChildIndex(const unsigned int parentIndex) const{
        return 2 * parentIndex + 1;
    }
};

/*
 * Implementation Notes: MinHeap
 * -----------------------------
 * In this implementation, all entries are stored in an array; their position in the array is the inorder
 * is the inoder walk of the min-heap tree. Amortized analysis can give us log(N) insert and extractMin,
 * since we minheapify in the same time. We dynamically double our array whenever it is full.
 */

template <typename ValueType>
MinHeap<ValueType>::MinHeap(int sizeHint){
    array = new ValueType[sizeHint + 1];
    capacity = sizeHint;
    count = 0;
}

template <typename ValueType>
MinHeap<ValueType>::~MinHeap(){
    delete[] array;
}

template <typename ValueType>
int MinHeap<ValueType>::size() const{
    return count;
}

template <typename ValueType>
bool MinHeap<ValueType>::isEmpty() const{
    return count == 0;
}

template <typename ValueType>
void MinHeap<ValueType>::insert(const ValueType& value){
    expandCapacity();
    count++; // add one more entries
    unsigned int index = count;
    array[index] = value;
    //go up and exchange accordingly
    unsigned int parentIndex = getParentIndex(index);
    while ((parentIndex > 0) && (array[parentIndex] > array[index])) {
        ValueType temp = array[parentIndex];
        array[parentIndex] = value;
        array[index] = temp;
        index = parentIndex;
        parentIndex = getParentIndex(parentIndex);//update and keep going up
    }
}

template<typename ValueType>
ValueType MinHeap<ValueType>::min() const{
    if(isEmpty()) error("min: No element in the minHeap!");
    return array[0];
}

template<typename ValueType>
ValueType MinHeap<ValueType>::extractMin(){
    if(isEmpty()) error("etractMin: No element in the minHeap!");
    ValueType min = array[1];
    array[1] = array[count];
    heapify(1); // heapify from the root
    count  --; // don't need discard array[count - 1]
    shrinkCapacity();
    return min;
}

/*
 * Implementation Notes: merge
 * --------------------------------
 * Instead of insert elements of this array and the array of minheap2 into the the new union, which takes
 * about O(Nlog(N)), we first concatenate the two arrays and heapify upwards. This implementation will take
 * O(N + log(N)) = O(N);
 * The original minheap &2 stays the same.
 */
template<typename ValueType>
MinHeap<ValueType>* MinHeap<ValueType>::merge(MinHeap& minheap1, MinHeap& minheap2){
    MinHeap *minheapTotal = new MinHeap(minheap1.capacity + minheap2.capacity);
    minheapTotal->count = minheap1.count + minheap2.count;
    copy(minheap1.array + 1, minheap1.array + 1 + minheap1.count, minheapTotal->array + 1);
    copy(minheap2.array + 1, minheap2.array + 1 + minheap2.count, minheapTotal->array + 1 + minheap1.count);
    for(unsigned int i = minheapTotal->size(); i > 0; i --){
        minheapTotal->heapify(i);
    }
    return minheapTotal;
}

/*
 * Implementation Notes: display
 * -----------------------------
 * TO-DO:  Try to learn how overlaod << operator under the template.
 */
template<typename ValueType>
void MinHeap<ValueType>::display() const{
    std::cout << "{";
    if(!isEmpty()){
        std::cout << array[1];
        for(unsigned int i = 2; i <= size(); i ++){
            std::cout << ", " << array[i];
        }
    }
    std::cout << "}" << std::endl;
}

template<typename ValueType>
void MinHeap<ValueType>::insertAll(const Vector<ValueType>& vec){
    for(int i =0; i < vec.size(); i ++){
        insert(vec[i]);
    }
}

template<typename ValueType>
bool MinHeap<ValueType>::testMinHeapity(){
    if(isEmpty()) error("testMinHeapity: No element in the MinHeap.");
    unsigned int index = count;
    unsigned int parent = getParentIndex(index);
    while(parent > 0){
        if(array[index] < array[parent]){
            display();
            std::cout << "Index: " << index <<  std::endl;
            std::cout << "Parent: " << parent << std::endl;
            error("One child is smaller than its parent.");
            return false;
        }
        index --;
        parent = getParentIndex(index);
    }
    return true;
}
#endif // MINHEAP_H
