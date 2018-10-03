#include "strlib.h"
using namespace std;

/* Inserts commas into a numeric string. */
string addCommasTo(int val) {
    string asStr = to_string(val);
    const int length = int(asStr.length());

    string result;
    for (int i = 0; i < length; i++) {
        /* Run backwards through the source string so determining where commas go
         * becomes a lot easier.
         */
        result = asStr[length - 1 - i] + result;

        /* Put commas in provided we've already added three characters, but aren't
         * about to use all the digits up.
         */
        if (i % 3 == 2 && i < length - 1) {
            result = ',' + result;
        }
    }
    return result;
}
