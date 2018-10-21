/**
 * File: pqueue-test.cpp
 * ---------------------
 * Framework in place to exercise all of the different
 * implemenations of the PQueue subclasses.
 */

#include <iostream>
#include "pqueue.h"
#include "lexicon.h"
#include "hashset.h"
#include "random.h"
#include "console.h"
#include "min-heap.h"
#include "sorted-double-linked-list.h"
using namespace std;

/**
 * Function: buildPQueue
 * ---------------------
 * Dynamically allocates an instance of the specifed PQueue, enqueues
 * up to size strings (drawn from the supplied Iterable), and then
 * returns the address of the new PQueue to the client.  If
 * the size of the pqueue is 0 just before returning, we throw an
 * error to signal the user that enqueue hasn't been implemented yet.
 */
template <typename Iterable>
static PQueue *buildPQueue(PQueue::PQueueType pqtype, Iterable iter, int size) {
	PQueue *pq = PQueue::createPQueue(pqtype);
	int count = 0;
    for (const string& key: iter) {
		pq->enqueue(key);
		count++;
		if (count == size) break;
	}

    if (pq->size() == 0) {
        error("Looks like " + PQueue::typeToName(pqtype) + "::enqueue " +
              "isn't implemented properly, because your queue is empty, and " +
              "that shouldn't be the case.");
    }

    cout << "+ Inserted " << pq->size() << " words." << endl;
	return pq;
}

/**
 * Function: extractElements
 * -------------------------
 * Accepts the priority queue of the specified type and confirms that
 * all of the elements in the queue are properly extracted, and extracted
 * in the expected order.
 */
static void extractElements(PQueue::PQueueType pqtype, PQueue *pq) {
	string leastSoFar;
	cout << "+ Extracting all elements, and confirming they come in alphabetical order." << endl;
	cout << "     [You'll see a dot for every 1000 extracted.]" << endl << "     ";

	int count = 0;
    int numExtractionsExpected = pq->size();
	while (!pq->isEmpty()) {
		string next = pq->extractMin();
        if (next < leastSoFar){
            cout << next << " " << leastSoFar << endl;
            error("Priority Queue is returning elements in the wrong order.");
        }
		leastSoFar = next;
		count++;
		if (count % 1000 == 0) {
			cout << ".";
			cout.flush();
			if (count % 40000 == 0) {
				cout << endl << "     ";
				cout.flush();
			}
		}
	}
	
	cout << endl;
	if (count != numExtractionsExpected) {
		ostringstream os;
		os << "Priority queue of type " << PQueue::typeToName(pqtype) << " had " << numExtractionsExpected <<
			  " elements, but we only extracted " << count << " of them." << endl;
		error(os.str());
	}
	cout << "+ Properly extracted all " << count << " elements from the priority queue." << endl;
}

/**
 * Function: sortedTest
 * --------------------
 * Confirms the supplied priority queue works well even when all words of the
 * English language are inserted in alphabetical order.
 */
static void sortedTest(PQueue::PQueueType pqtype, const Lexicon& english, int size) {
	PQueue *pq = buildPQueue(pqtype, english, size);
	extractElements(pqtype, pq);
	delete pq;
	cout << "+ SortedTest seems to work well with the " << PQueue::typeToName(pqtype) << ", provided destructor works properly." << endl;
}

/**
 * Function: scrambleTest
 * ----------------------
 * Confirms the supplied priority queue works well when all words of the
 * English language are inserted is a seemingly random order.
 */
static void scrambleTest(PQueue::PQueueType pqtype, const Lexicon& english, int size) {
    HashSet<string> set;
    for (const string& word: english) set.insert(word);

    PQueue *pq = buildPQueue(pqtype, set, size);
	extractElements(pqtype, pq);
	delete pq;
	cout << "+ ScrambledTest seems to work well with the " << PQueue::typeToName(pqtype) << ", provided destructor works properly." << endl;
}

/**
 * Function: createRandomString
 * ----------------------------
 * Returns a string of the supplied length (defaulting to 16) where
 * all of the characters are randomly chosen lowercase letters.
 */
static string createRandomString(int length = 16) {
    string s(length, '-');
    for (size_t i = 0; i < s.size(); i++)
		s[i] = randomInteger('a', 'z');
	return s;
}

/**
 * Function: randomizedTest
 * ------------------------
 * Similar to other tests, but enqueues random strings of length 16 instead
 * of actual words from the English language.
 */
static void randomizedTest(PQueue::PQueueType pqtype, int size) {
	Vector<string> randomStrings;
	size = min(size, 100000);
	for (int i = 0; i < size; i++) randomStrings.add(createRandomString());
	PQueue *pq = buildPQueue(pqtype, randomStrings, size);
    extractElements(pqtype, pq);
	delete pq;
	cout << "+ RandomizedTest seems to work well with the " << PQueue::typeToName(pqtype)
         << ", provided destructor works properly." << endl;
}

/**
 * Function: mergeTest
 * -------------------
 * Confirms that the merge operation for the specific PQueue type works properly.
 */
static void mergeTest(PQueue::PQueueType pqtype, const Lexicon& english, int size) {
    HashSet<string> set;
    int count = 0;
    for (const string& word: english) {
        set.insert(word);
        count++;
        if (count == size) break;
    }

    cout << "+ Constructing two priority queues to exercise merge." << endl;
    PQueue *pq1 = buildPQueue(pqtype, set, size);
    PQueue *pq2 = buildPQueue(pqtype, set, size);
	int size1 = pq1->size();
	int size2 = pq2->size();
    if (size1 == 0 || size2 == 0) {
        error("Looks like you have yet to implement basic functionality for the " +
              PQueue::typeToName(pqtype) + ", because your nonempty queues look to be empty anyway.");
    }

	PQueue *pq = PQueue::merge(pq1, pq2);

	if (size1 + size2 != pq->size()) {
		error("Priority Queue merge clearly failed, because size of "
              "merge doesn't equal the sum of the two original sizes.");
	}
		
	while (!pq->isEmpty()) {
		string one = pq->extractMin();
		string two = pq->extractMin();
		if (one != two) error("Merge of two identical priority queues "
                              "should have two copies of all entries.");
        set.remove(one);
    }
	
    if (!set.isEmpty()) {
        error("It looks like the merged priority queue wasn't fully drained of all of its elements.");
    }
    delete pq;
    cout << "+ MergeTest seems to work well with the " << PQueue::typeToName(pqtype)
         << ", provided destructor works properly." << endl;
}

static const struct {
	PQueue::PQueueType type;
    int reasonableTestSize;
} testParameters[] = {
    { PQueue::UnsortedVector, 10000},
    { PQueue::Heap, INT8_MAX},
    { PQueue::LinkedList, 10000},
    { PQueue::BinomialHeap, INT8_MAX}
};

/**
 * @brief testMinHeap test MinHeap class
 * @param testcase
 */
static void testMinHeap(const Vector<int>& testcase){
    cout << "**************************************************" << endl;
    cout << "test for MinHeap Class" << endl;

    cout << "Test for Method: insert" << endl;
    cout <<"Input:" <<testcase << endl;
    MinHeap<int> *minheap = new MinHeap<int>(20);
    minheap->insertAll(testcase);
    cout << "Output:";
    minheap->display();
    minheap->testMinHeapity();
    cout << "Test for Method: extractMin" << endl;
    while (!minheap->isEmpty()) {
        minheap->testMinHeapity();
        minheap->display();
        minheap->extractMin();
    }
    cout << "**************************************************" << endl;

}

static void runTestMinHeap(){
    const Vector<int> testcase0 = {22, 11, 10, 9};
    testMinHeap(testcase0);
    const Vector<int> testcase1 = { 22, 11, 10, 9, 8 , 7, 6, 5, 6, 1, 2, 4};
    testMinHeap(testcase1);
    const Vector<int> testcase2 = { 1, 1, 1,1 ,1 ,1, 1,1 ,1, 1, 1,1 ,1, 1, 1, 1};
    testMinHeap(testcase2);
    Vector<int> testcase3;
    for(int i = 30; i > 0; i --){
        testcase3.add(i);
    }
    testMinHeap(testcase3);
}

/**
 * @brief testMinHeap test MinHeap class
 * @param testcase
 */
static void testSortedDoubleLinkedList(const Vector<int>& testcase){
    cout << "**************************************************" << endl;
    cout << "test for SortedDoubleLinkedList  Class" << endl;

    cout << "Test for Method: insert" << endl;
    cout <<"Input:" <<testcase << endl;
    SortedDoubleLinkedList<int> *sdll = new SortedDoubleLinkedList<int>;
    sdll->insertAll(testcase);
    cout << "Output:";
    sdll->display();
    cout << "Test for sortedness" << endl;
//    sdll->testSortedness();
    cout << "Test for Method: remove(0)" << endl;
    while (!sdll->isEmpty()) {
        sdll->display();
        sdll->remove(0);
    }
    cout << "**************************************************" << endl;

}

static void runTestSortedDoubleLinkedList(){
//    const Vector<int> testcase0 = {22, 11, 10, 9};
//    testSortedDoubleLinkedList(testcase0);
//    const Vector<int> testcase1 = { 22, 11, 10, 9, 8 , 7, 6, 5, 6, 1, 2, 4};
//    testSortedDoubleLinkedList(testcase1);
//    const Vector<int> testcase2 = { 1, 1, 1,1 ,1 ,1, 1,1 ,1, 1, 1,1 ,1, 1, 1, 1};
//    testSortedDoubleLinkedList(testcase2);
//    Vector<int> testcase3;
//    for(int i = 30; i > 0; i --){
//        testcase3.add(i);
//    }
//    testSortedDoubleLinkedList(testcase3);
//    Vector<int> testcase4;
//    for(int i = 23; i > 0; i --){
//        testcase4.add(rand() % 23);
//    }
//    testSortedDoubleLinkedList(testcase4);
//    const Vector<int> testcase5 = {11, 0, 6, 2, 6, 0, 14, 1, 11, 1, 17, 16};
//    testSortedDoubleLinkedList(testcase5);
    cout << "Merged Test" << endl;
    const Vector<int> testcaseMerge1 = {22, 11, 10, 9};
    SortedDoubleLinkedList<int> *sdll1 = new SortedDoubleLinkedList<int>;
    sdll1->insertAll(testcaseMerge1);
    sdll1->display();
    const Vector<int> testcaseMerge2 = {22, 11, 10, 9};
    SortedDoubleLinkedList<int> *sdll2 = new SortedDoubleLinkedList<int>;
    sdll2->insertAll(testcaseMerge1);
    sdll2->display();
    SortedDoubleLinkedList<int> *sdll = SortedDoubleLinkedList<int>::merge(*sdll1, *sdll2);
    sdll->display();
}

/**
 * Function: runAllTests
 * ---------------------
 * Runs all of the various tests against each of the four subclasses.
 */
static void runAllTests() {
    Lexicon english("dictionary.txt");
////    for (unsigned long i = 0; i < sizeof(testParameters)/sizeof(*testParameters); i++) {
    for (unsigned long i = 2; i < sizeof(testParameters)/sizeof(*testParameters) - 1; i++) {
        cout << "Exercising the " << PQueue::typeToName(testParameters[i].type) << endl;
        cout << "---------------" << string(PQueue::typeToName(testParameters[i].type).size(), '-') << endl;
        sortedTest(testParameters[i].type, english, testParameters[i].reasonableTestSize);
        scrambleTest(testParameters[i].type, english, testParameters[i].reasonableTestSize);
        randomizedTest(testParameters[i].type, testParameters[i].reasonableTestSize);
        mergeTest(testParameters[i].type, english, testParameters[i].reasonableTestSize / 10);
    }
}

/**
 * Function: main
 * --------------
 * Defines the test framework for all PQueue subclasses.  The very first
 * test failure results in an error message and program termination.
 */
int main() {
    try {
        runAllTests();
        cout << "All done!" << endl;
    } catch (const ErrorException& e) {
        cout << "Oops! " << e.what() << endl;
        cout << "Aborting the test framework so you can fix everything. :)" << endl;

    }
    return 0;
}
