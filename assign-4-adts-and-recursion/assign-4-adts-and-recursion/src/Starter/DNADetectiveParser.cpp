#include "strlib.h"
#include <string>
#include <fstream>
#include <sstream>
#include "vector.h"
#include "DNADetective.h"
#include "RecursionToTheRescue.h"
using namespace std;

/* Everything in here is private to the file. */
namespace {
    /* Given a line from a file and a prefix, trims and capitalizes everything
     * after that prefix and returns it.
     */
    string contentsAfter(const string& line, const string& prefix) {
        return toUpperCase(trim(line.substr(prefix.length())));
    }

    /* Reports whether a string is a valid DNA strand. */
    bool isValidDNA(const string& strand) {
        return strand.find_first_not_of("ACGT") == string::npos;
    }

    /* Parses the result out of a line. */
    bool parseAnswer(const string& line) {
        string answerStr = contentsAfter(line, "Result: ");
        if (answerStr == "TRUE") return true;
        if (answerStr == "FALSE") return false;

        /* We shouldn't see anything else here. */
        error("Unknown answer: " + answerStr);
        return false; // Suppress compiler warnings
    }
}

DNATestCase loadDNATestCase(istream& input) {
    DNATestCase result;
    bool haveReadFirst  = false;
    bool haveReadSecond = false;
    bool haveReadDist   = false;
    bool haveReadAnswer = false;

    for (string line; getline(input, line); ) {
        /* Skip blank lines or lines starting with hash marks. */
        if (trim(line).empty() || line[0] == '#') continue;

        /* See what this line is. */
        if (startsWith(line, "First: ")) {
            if (haveReadFirst) error("Two first strands in the same test file.");
            result.first = contentsAfter(line, "First: ");
            if (!isValidDNA(result.first)) error("Found a string, but it wasn't a DNA strand.");

            haveReadFirst = true;
        } else if (startsWith(line, "Second: ")) {
            if (haveReadSecond) error("Two second strands in the same test file.");
            result.second = contentsAfter(line, "Second: ");
            if (!isValidDNA(result.second)) error("Found a string, but it wasn't a DNA strand.");

            haveReadSecond = true;
        } else if (startsWith(line, "Distance: ")) {
            if (haveReadDist) error("Two distances in the same test file.");
            result.k = stringToInteger(contentsAfter(line, "Distance: "));
            if (result.k < 0) error("Found a distance, but it was negative.");

            haveReadDist = true;
        } else if (startsWith(line, "Result: ")) {
            if (haveReadAnswer) error("Two answers recorded in the same test file.");

            result.result = parseAnswer(line);
            haveReadAnswer = true;
        } else {
            error("Not sure how to handle this line: " + line);
        }
    }

    /* Confirm we got everything we needed. */
    if (!haveReadFirst)  error("No first strand found in test case.");
    if (!haveReadSecond) error("No second strand found in test case.");
    if (!haveReadDist)   error("No max distance found in test case.");
    if (!haveReadAnswer) error("No reference answer found in the test case.");
    return result;
}
