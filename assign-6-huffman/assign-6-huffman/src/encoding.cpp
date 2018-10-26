// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include "bitstream.h"
#include "filelib.h"
#include "simpio.h"
#include "pqueue.h"
#include <string>

Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    freqTable.add(PSEUDO_EOF, 1); // the occurrence of EOF
    while(true){
        if(input.fail()) break;
        freqTable[input.get()] ++;
    }
    // remove the char(255) or int -1
    freqTable.remove(-1);
    return freqTable;          
}

HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> pq;
    for(const auto ch: freqTable){
        HuffmanNode *hfm = new HuffmanNode(ch, freqTable[ch], NULL, NULL);
        pq.add(hfm, hfm->count);
    } // construct the priorityQueue
    while(pq.size() != 1){
        HuffmanNode* left = pq.dequeue();
        HuffmanNode* right = pq.dequeue();
        HuffmanNode* root = new HuffmanNode(NOT_A_CHAR, left->count + right->count, left, right);
        pq.add(root, root->count);
    }
    return pq.peek();
}

/*
 * Implementation notes: buildingEncodingMap
 * --------------------------------------------
 * Just a tree trasverse by backtracking. This is will be an
 * preorder version. what if we have only one EOF??
 * Let's assume that the file cannot be empty.
 */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap = {};
    trasverseHuffmanTree(encodingMap, encodingTree, "");
    return encodingMap;
}

void trasverseHuffmanTree(Map<int, string>& encodingMap,
                          HuffmanNode *root,
                          const string& path){
    //base case
    if(root == NULL) return;
    int ch = root->character;
    if(ch != NOT_A_CHAR){
        encodingMap.add(ch, path);
    }
    trasverseHuffmanTree(encodingMap, root->one, path+"1");
    trasverseHuffmanTree(encodingMap, root->zero, path+"0");
}

void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    while(!input.fail()){
        int ch = input.get();
        string bnCode = encodingMap[ch];
        writeBinaryCode(bnCode, output);
    }
    // EOF
    string bnCodeEOF = encodingMap[PSEUDO_EOF];
    writeBinaryCode(bnCodeEOF, output);
}

void writeBinaryCode(const string &bnCode, obitstream &output){
    for(int i = 0; i < bnCode.size(); i++){
        output.writeBit(bnCode[i] == '1'? 1:0);
    }
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    HuffmanNode* root = encodingTree;
    while(!input.fail()){
        int ch = root->character;
        if(root->character != NOT_A_CHAR){
            output.put(ch);
            root = encodingTree;
        } else{
            int bit = input.readBit();
            if(bit == 1){
                root = root->one;
            } else{
                root = root->zero; // no need worry about NULL
            }
        }
    }
}

void compress(istream& input, obitstream& output) {
    Map<int ,int> freqTable = buildFrequencyTable(input);
    HuffmanNode* hfmNode = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(hfmNode);
    output << freqTable; // the header
    rewindStream(input); // start from the start again
    encodeData(input, encodingMap, output);
    freeTree(hfmNode);
}

void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable;
    input >> freqTable;
    HuffmanNode* hfmNode = buildEncodingTree(freqTable);
    // reconstruct the tree
    decodeData(input, hfmNode, output);
    freeTree(hfmNode);
}

void freeTree(HuffmanNode* node) {
    if(node == NULL) return; // base case
    freeTree(node->one); // bottom-up
    freeTree(node->zero);
    delete node;
}
