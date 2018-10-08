#ifndef DNADetective_Included
#define DNADetective_Included

#include "RecursionToTheRescue.h"
#include <string>
#include <istream>

/**
 * Type representing a test case for the DNA Detective assignment.
 */
struct DNATestCase {
    std::string first;   // The first DNA strand
    std::string second;  // The second DNA strand
    int k;               // The edit distance given in the test case
    bool result;         // The expected answer
};

/**
 * Given a stream containing a DNA test case file, reads the
 * data from that file and returns a DNATestCase containing
 * the relevant information.
 *
 * @param source The stream to read from.
 * @return The test case from that file.
 * @throws ErrorException If the test case is malformed.
 */
DNATestCase loadDNATestCase(std::istream& source);

#endif
