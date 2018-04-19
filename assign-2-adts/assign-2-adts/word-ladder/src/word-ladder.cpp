/**
 * File: word-ladder.cpp
 * ---------------------
 * Implements a program to find word ladders connecting pairs of words.
 */

#include <iostream>
using namespace std;

#include "console.h"
#include "lexicon.h"
#include "strlib.h"
#include "simpio.h"
#include "vector.h"
#include "foreach.h"
#include <string>
#include "queue.h"

static string getWord(const Lexicon &english, const string &prompt);

/**
 * @brief getOneLetterDiffWords
 * @param english, the English word lexicon
 * @param topWord, the top word of the ladder
 * @return a lexicon of strings which is both words and one different letter from the topword
 */
Lexicon getOneLetterDiffWords(const Lexicon &english, const string &topWord);

/**
 * @brief isOneLetterDiff
 * @param word1, an English word
 * @param word2, an English word that has the same length as word1
 * @return True is the two words are different by one letter
 */
bool isOneLetterDiff(const string &word1, const string &word2);
static void generateLadder(const Lexicon &english, const string &start, const string &end);
static void playWordLadder();

/* the test for above method */
void testGetOneLetterWords();


static string getWord(const Lexicon& english, const string& prompt) {
    while (true) {
        string response = trim(toLowerCase(getLine(prompt)));
        if (response.empty() || english.contains(response)) return response;
        cout << "Your response needs to be an English word, so please try again." << endl;
    }
}

Lexicon getOneLetterDiffWords(const Lexicon& english, const string& topWord) {
    Lexicon oneLetterDiffWords;
    static const string kLetters = "letters.txt";
    Lexicon letters(kLetters);
    for(int i = 0; i < topWord.length(); i ++) {
        foreach (string letter in letters) {
            string word = topWord.substr(0,  i) + letter + topWord.substr(i + 1, topWord.length() - i -1);
            if(english.contains(word)) {
                oneLetterDiffWords.add(word);
            }
        }
    }
    return oneLetterDiffWords;
}

bool isOneLetterDiff(const string& word1, const string& word2) {
    if (word1.length() != word2.length()) {
        return false;
    } else if (word1 == word2) {
        return false;
    } else {
        int difference = 0;
        for (int i = 0;  i < word1.length(); i ++) {
            if (word1[i] != word2[i]) {
                difference ++ ;
            }
            if (difference > 1)  return false;
        }
        return true;
    }
}

static void generateLadder(const Lexicon& english, const string& start, const string& end) {
    cout << "Here's where you'll search for a word ladder connecting \"" << start << "\" to \"" << end << "." << endl;
    Vector<string> initialLadder;
    initialLadder.add(start);

    Queue<Vector<string>> partialLadders;
    partialLadders.enqueue(initialLadder);

    Vector<string> ladder;
    Lexicon usedWords;
    Lexicon cloneEnglish = english;
    usedWords.add(start);  // to avoid used words apear again.
    while(!partialLadders.isEmpty()) {
        Vector<string> partialLadder = partialLadders.dequeue();
        string topWord = partialLadder[partialLadder.size() - 1];
        if(isOneLetterDiff(topWord, end)) {
            ladder = partialLadder;
            break;
        } else {
            foreach (string word in getOneLetterDiffWords(cloneEnglish ,topWord)) {
                if(!usedWords.contains(word)) {
                    Vector<string> clonePartialLadder = partialLadder;
                    clonePartialLadder += word;
                    partialLadders.enqueue(clonePartialLadder);
                    usedWords.add(word);
                    cloneEnglish.remove(word);
                } else {
                    continue;
                }
            }
        }
    }
    if(ladder.isEmpty()) {
        cout << "Sorry! No ladder can be Found between" << start << " and " << end << " ." << endl;
    } else {
        cout << "Found ladder:" << " ";
        for(int i = 0; i < ladder.size(); i ++) {
            cout << ladder[i] << ", ";
        }
        cout << end << ". " << endl;
        cout <<" " << endl;
    }
}


static const string kEnglishLanguageDatafile = "dictionary.txt";
static const Lexicon english(kEnglishLanguageDatafile);
static void playWordLadder() {
    while (true) {
        string start = getWord(english, "Please enter the source word [return to quit]: ");
        if (start.empty()) break;
        string end = getWord(english, "Please enter the destination word [return to quit]: ");
        if (end.empty()) break;
        generateLadder(english, start, end);
    }
}


/* test for  Methods*/
void testGetOneLetterWords() {
    static const string kTestcase1 = "testcase1.txt";
   Lexicon testcases(kTestcase1);
   foreach (string testcase in testcases) {
       cout << "The top word is " << testcase << endl;
       cout << getOneLetterDiffWords(english, testcase) << endl;
   }
}


int main() {
    setConsoleFont("Times-Bold-18");
    cout << "Welcome to the CS106 word ladder application!" << endl << endl;
    playWordLadder();
    cout << "Thanks for playing!" << endl;
    return 0;
}
