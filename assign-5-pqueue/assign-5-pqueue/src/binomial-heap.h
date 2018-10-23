/*
 * File: binomial-heap.h
 * ----------------------
 * This interface exports a very simple version of
 * BinomialMinHeap class.
 */
#ifndef BinomialMinHeap_H
#define BinomialMinHeap_H

#include <math.h> //std:power
#include "vector.h"
#include <tuple>
#include "error.h"
#include <algorithm>    // std::min
#include <cstdlib> //std::abs
/*
 * Class: BinomialMinHeap<ValueType>
 * ----------------------------------
 * This class give the interface of a Binomial Tree based a node struct. Methods insert and
 * extractMin all depends on Method Merge.
 * This type must support less or greater operator.
 */
template<typename ValueType>
class BinomialMinHeap
{
public:
    /*
     * Constructor
     * Usage: BinomialMinHeap<ValueType> bnHeap;
     * --------------------------------------
     * Intialize am empty vector for a series of different
     * orders of binomial heaps.
     */
    BinomialMinHeap();

    /*
     * Destructor
     * Usage: (usaully implicit);
     * --------------------------
     * delet all the heap memory associated with the binomial
     * heaps.
     */
    ~BinomialMinHeap();

    /*
     * Method: size
     * Usage: unsigned int numElement = bnheap.size();
     * -----------------------------------------------
     * Return the number of elements in the binomial heap trees.
     */
    unsigned int size() const;

    /*
     * Method: isEmpty()
     * Usage: if(bnheap.isEmpty()) { .....;}
     * -------------------------------------
     * Return true if no element in the binomial heap trees.
     */
    bool isEmpty() const;

    /*
     * Method: merge
     * Usage: BinomialMinHeap<ValueType> *bnheapTotal =
     * merge(bnheap1, bnheap2);
     * ----------------------------------------------
     * Return a BinomialMinHeap pointer who points to the union
     * of the the bnheap1 and bnheap2 non-destructively.
     */
    static BinomialMinHeap<ValueType>* merge(
            const BinomialMinHeap<ValueType>& bnheap1,
            const BinomialMinHeap<ValueType>& bnheap2);
    /*
     * Method: insert
     * Usage: bnheap.insert(value);
     * ----------------------------
     * Insert the new element into the binomial heap,
     * while maintaining the heap property.
     */
    void insert(ValueType value);

    /*
     * Method: peak
     * Usage: ValueType minValue = bnheap.peek();
     * ------------------------------------------
     * Return the smallest element in the
     */
    ValueType peek() const;

    /*
     * Method: extractMin
     * Usage: bnheap.extractMin();
     * ---------------------------
     * Return the smallest elment in the whole bunch of binomial heap tree.
     */
    ValueType extractMin();

    /*
     * Method: insertAll
     * Usage: bnheap.insertAll(vec);
     * -----------------------------
     * Insert all the elements in the vec into the binomial heap tree.
     */
    void insertAll(const Vector<ValueType>& vec);

    /*
     * Method:display
     * Usage: bnheap.display()
     * ------------------------
     * //TO-BE-DELETED; TO HARD TO IMPLEMENT. DEBUG USE ONLY!
     * Show all the binomial heap tree that represent the group of elements.
     */
    void display() const;

    /*
     * Method: include
     * Usage: bnheap1.include(bnheap2);
     * --------------------------------
     * include all the elements into the the current binomial heap tree
     */
    void include(const BinomialMinHeap& other);


    /* Private Section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

    /*
     * Implementation notes:
     * ---------------------
     * The BinomialMinHeap class is represented by custom struct node, within which there is a
     * Vector of children nodes.
     */
private:
    struct node
    {
        ValueType elem;
        int degree;
        Vector<node *> children;
        /*
         * Constuctor
         * Usage: node *bnheapNode = new node(value);
         * ------------------------------------------
         * Construct a 0-order binomial heap tree, which is just a single node.
         */
        node(ValueType value) {
            elem = value;
            degree = 0;
            children = {};
        }

        node(){
            degree = -1;
            children = {};
        }

        ~node(){
            for(unsigned  int i = 0; i < children.size(); i ++){
                delete children[i];
            }
        }
        /*
         * Method: display
         * Usage: bnheapNode.display();
         * ----------------------------
         * recursively show the node and its children.
         * //TO-BE-DELETED: Debug use only, to hard to implement.
         */
        void display(unsigned int level){
            std::cout << " ***************" << "Level" << level<<
                         "******************** " << std::endl;
            std::cout << "Root is " << elem << ";" <<std::endl;
            std::cout << " The children are ";
            for(unsigned int i = 0; i < children.size(); i ++){
                node* child = children[i];
                std::cout << " " <<child->elem;
            }
            std::cout << ". "<<std::endl;
            for(unsigned int i = 0; i < children.size(); i ++){
                node* child = children[i];
                child->display(level + 1);
            }

        }

        /*
         * Operator: =
         * Usage: node newNode = oldNode;
         * -----------------------------------------------
         * Copy into the current object.
         */
        node& operator =(const node& toBeCopied){
            elem = toBeCopied.elem;
            degree = toBeCopied.degree;
            for(unsigned i = 0; i < toBeCopied.children.size(); i ++){
                node* child = toBeCopied.children[i];
                node* newChild = deepCopy(*child);
                children.add(newChild);
            }
            return *this;
        }

        /*
         * Method: deepCopy
         * Usage: node* newNode = deepCopy(oldNode)
         * Return a pointer of the deepcied object in the heap.
         */
        static node* deepCopy(const node& toBeCopied){
            node* newNode = new node(toBeCopied.elem);
            newNode->degree = toBeCopied.degree;
            for(unsigned int i = 0; i < toBeCopied.children.size(); i ++){
                node* child = toBeCopied.children[i];
                node* newChildren = deepCopy(*child);
                newNode->children.add(newChildren);
            }
            return newNode;
        }
        /*
         * Operator: <
         * Usage: if(node1 < node2) {..........;}
         * ---------------------------------------
         * Return true if value stored in current node is smaller than node2;
         */
        bool operator<(node& node2){
            return this->elem < node2.elem;
        }

        /*
         * Method: add
         * Usage: auto[currRoot, carry]  = add(bnheapNode1, bnheapNode2, prevCarry);
         * ------------------------------------------------------------------------
         *After this the two nodes will stay the same.
         */
        static std::tuple<node*, node*> add(node* bnheapNode1 ,node* bnheapNode2, node* preCarry){
            node* currRoot = new node(); // dump holder
            node* carry = new node(); // dump holder
            //take of the two adding node
            if((bnheapNode1 == NULL) && bnheapNode2 == NULL) {
                // no change needed
            } else if ((bnheapNode1 == NULL) && (bnheapNode2 != NULL)) {
                *currRoot = *bnheapNode2;
            } else if ((bnheapNode1 != NULL)&&( bnheapNode2 == NULL)) {
                *currRoot = *bnheapNode1;
            } else if((bnheapNode1 != NULL) && (bnheapNode2 != NULL)){
                if(bnheapNode1->degree != bnheapNode2->degree) error("add: Two nodes are not on the same degree!");
                if(*bnheapNode1 < *bnheapNode2){
                    *carry = *bnheapNode1;
                    node* newChild = deepCopy(*bnheapNode2);
                    carry->children.add(newChild);
                } else{
                    *carry = *bnheapNode2;
                    node* newChild = deepCopy(*bnheapNode1);
                    carry->children.add(newChild);
                }
                carry->degree ++;
            }

            // take of precarry
            if((currRoot->degree == -1) && preCarry == NULL) {
                // no need to change
            } else if ((preCarry == NULL) && (currRoot->degree != -1)) {
               // no need to change
            } else if ((preCarry != NULL)&&( currRoot->degree == -1)) {
                *currRoot = *preCarry;
            } else if((preCarry != NULL) && (currRoot->degree != -1)){
                if(preCarry->degree != currRoot->degree) error("add: the degree of precarry is not right!");
                if(*preCarry < *currRoot){
                    *carry = *preCarry;
                    node* newChild = deepCopy(*currRoot);
                    carry->children.add(newChild);
                } else{
                    *carry = *currRoot;
                    node* newChild = deepCopy(*preCarry);
                    carry->children.add(newChild);
                }
                currRoot = new node();
                carry->degree ++;
            }
            if(preCarry != NULL){
                preCarry->~node(); //free the precarry.
            }
            if(carry->degree == -1){
                carry->~node();
                carry = NULL;
            }
            if(currRoot->degree == -1){
                currRoot->~node();
                currRoot = NULL;
            }
            return std::make_tuple(currRoot, carry);
        }
    };

    /* Instance Variables */
    Vector<node *> bnheapVec;
    unsigned int count;

    /* Private Method*/
    /*
     * Method: clear
     * Usage; bnheap.clear();
     * ----------------------
     * Clear the vector for binomial heap tree roots nodes and release associated memory.
     */
    void clear(){
        for(unsigned int i = 0; i < bnheapVec.size(); i ++){
            delete bnheapVec[i];
        }
    }

    /*
     *  Method: findMinNode
     * Usgae:node * minNode = bnheap.findMinNode();
     * -------------------------------------------------------------------
     * Returns a node pointer to smallest root in among the the binomial heap trees.
     * Serves as a helper function for peek and extracMin.
     */
    node* findMinNode() const{
        node* lestSofar = bnheapVec[0];
        for(unsigned int i = 0; i < bnheapVec.size(); i ++){
            if((lestSofar == NULL) && bnheapVec[i] == NULL) {
                lestSofar = bnheapVec[i]; // move forward
            } else if((lestSofar != NULL) && (bnheapVec[i] == NULL)){
                // nothing changed
            } else if((lestSofar == NULL)&&(bnheapVec[i] != NULL)){
                lestSofar = bnheapVec[i];
            } else{
                lestSofar = (*lestSofar < *bnheapVec[i])? lestSofar:bnheapVec[i];
            }
        }
        return lestSofar;
    }

    /*
     * Method: insertSubtreeNode(node* subtreeNode)
     * Usage: bnheap.insertSubtreeNode(subtreeNode);
     * ---------------------------------------------
     * Insert the node and all its children recursively into the current object
     * destructively. This is a helper function for extractMin.
     */
    void insertSubtreeNode(node* subtreeNode){
        insert(subtreeNode->elem);
        for(unsigned int i = 0; i < subtreeNode->children.size(); i ++){
            insertSubtreeNode(subtreeNode->children[i]);
        }
    }

    /*
     * Private Method: shrinkVec
     * Usage: bnheap.shrinkVec()
     * -------------------------
     * Remove the nulls at the end of the vector.
     */
    void shrinkVec(){
        if(isEmpty()){
            bnheapVec = {};
        } else{
            unsigned int i = bnheapVec.size() -1;
            while((bnheapVec[i] == NULL)&&(i >=0)){
                bnheapVec.remove(i);
                i--;
            }
        }
    }

};

/*
 * Implementation notes:
 * ---------------------
 * The general merge is base for extractMin and insert method. Attention: free memories
 * in extract min or insert. Another note: take care of size count event hough I can take
 * care of them in the upper layer because I still need size to check about the
 * correctness of  merge.
 * TO-BE-DELETED: DISPLAY() NODE AND HEAP.
 */
template<typename ValueType>
BinomialMinHeap<ValueType>::BinomialMinHeap(){
    count = 0;
    bnheapVec = {};
}

template<typename ValueType>
BinomialMinHeap<ValueType>::~BinomialMinHeap(){
    clear();
}

template<typename ValueType>
unsigned int BinomialMinHeap<ValueType>::size() const{
    return count;
}

template<typename ValueType>
bool BinomialMinHeap<ValueType>::isEmpty() const{
    return count == 0;
}

/*
 * Impelementation notes: include, insert, extractMin
 * --------------------------------------------------
 * Insert is to include a zero-order tree and free the zero-order tree.
 * ExtractMin is to remove the smallest and then
 * include all its children one by one recursively.
 * Include is basically the same with merge. But it will make changes to the
 * current object.
 */
template<typename ValueType>
void BinomialMinHeap<ValueType>::include(const BinomialMinHeap &other){
    node* carry = NULL;
    node* newNode = NULL;
    unsigned int commonSize = std::min(this->bnheapVec.size(), other.bnheapVec.size());
    for(unsigned int i = 0; i < commonSize; i++){
        std::tie(newNode, carry) = node::add(this->bnheapVec[i], other.bnheapVec[i], carry);
        this->bnheapVec[i] = newNode;
    }
    //take care  of the rest
    if(this->bnheapVec.size() > other.bnheapVec.size()){
        for(unsigned int i = commonSize; i < this->bnheapVec.size(); i++){
            std::tie(newNode, carry) = node::add(this->bnheapVec[i], NULL, carry);
            bnheapVec[i] = newNode;
        }
    } else{
        for(unsigned int i = commonSize; i < other.bnheapVec.size(); i++){
            std::tie(newNode, carry) = node::add(NULL, other.bnheapVec[i], carry);
            bnheapVec.add(newNode);
        }
    }
    if(carry != NULL){
        this->bnheapVec.add(carry);
    } else{
        delete carry;
    }
    this->count += other.count;
}

template<typename ValueType>
void BinomialMinHeap<ValueType>::insert(ValueType value){
    // construct a 0-order tree
    BinomialMinHeap<ValueType> *bnheap2 = new BinomialMinHeap<ValueType>();
    node* zeroOder = new node(value);
    bnheap2->bnheapVec.add(zeroOder);
    bnheap2->count ++;
    include(*bnheap2);
    bnheap2->~BinomialMinHeap();
    // include already take care of the count
}

template<typename ValueType>
ValueType BinomialMinHeap<ValueType>::peek() const{
    return findMinNode()->elem;
}

template<typename ValueType>
void BinomialMinHeap<ValueType>::insertAll(const Vector<ValueType>& vec){
    for(unsigned int i = 0; i < vec.size(); i ++){
        insert(vec[i]);
    }
}

template<typename ValueType>
ValueType BinomialMinHeap<ValueType>::extractMin(){
    node* minNode = findMinNode();
    ValueType min = minNode->elem;
    bnheapVec[minNode->degree] = NULL;
    count -= pow(2, minNode->degree);
    for(unsigned int i = 0; i < minNode->children.size(); i ++){
        insertSubtreeNode(minNode->children[i]);
    }
    minNode->~node();
    shrinkVec();
    return min;

}

template<typename ValueType>
void BinomialMinHeap<ValueType>::display() const{
    for(unsigned int i = 0; i < bnheapVec.size(); i++){
        std::cout << "##################" << "the  " << i << "th Binomial Heap"
             << "###########################" << std::endl;
        if(bnheapVec[i] == NULL) {
            std::cout << "NULL" << std::endl;
        } else{
            bnheapVec[i]->display(0);
        }
    }
}

/*
 * Implementation Notes: merge
 * ----------------------------
 * Be sure to delete the carrry node in the heap since we have deepcopy
 * a new one if the carry needs to take up the current position.
 */
template<typename ValueType>
BinomialMinHeap<ValueType>* BinomialMinHeap<ValueType>::merge(const BinomialMinHeap<ValueType>& bnheap1,
                                                         const BinomialMinHeap<ValueType>& bnheap2){
            Vector<node *> vec1 = bnheap1.bnheapVec;
            Vector<node *> vec2 = bnheap2.bnheapVec;
            Vector<node *> vec;
            node* carry = NULL;
            node* newNode = NULL;
            unsigned int commonSize = std::min(vec1.size(), vec2.size());
            for(unsigned int i = 0; i < commonSize; i ++){
                std::tie(newNode, carry) = node::add(vec1[i], vec2[i], carry);
                vec.add(newNode);
            }
            // take care of the rest
            if( vec1.size() > vec2.size()){
                for(unsigned int i = commonSize; i < vec1.size(); i++){
                    std::tie(newNode, carry) = node::add(vec1[i], NULL, carry);
                    vec.add(newNode);
                }
            } else{
                for(unsigned int i = commonSize; i < vec2.size(); i ++){
                    std::tie(newNode, carry) = node::add(NULL, vec2[i], carry);
                    vec.add(newNode);
                }
            }
            if(carry != NULL){
                vec.add(carry);
            } else{
                delete carry; // free heap memory and do not delete newNode!!
            }
            BinomialMinHeap<ValueType> *bnheapTotoal = new BinomialMinHeap<ValueType>;
            bnheapTotoal->count = bnheap1.count + bnheap2.count;
            bnheapTotoal->bnheapVec = vec;
            return bnheapTotoal;
}
#endif // BinomialMinHeap_H
