#ifndef Disasters_Included
#define Disasters_Included

#include "map.h"
#include "vector.h"
#include "set.h"
#include "gtypes.h"
#include <string>
#include <istream>

/**
 * Type representing a test case for the Disaster Preparation problem.
 */
struct DisasterTest {
    Map<std::string, Set<std::string>> network; // The road network
    Map<std::string, GPoint> cityLocations;     // Where each city should be drawn
    int minCitiesNeeded;                        // True minimum number of cities needed
};

/**
 * Given a stream pointing at a test case for Disaster Preparation,
 * pulls the data from that test case.
 *
 * @param source The stream containing the test case.
 * @return A test case from the file.
 * @throws ErrorException If an error occurs or the file is invalid.
 */
DisasterTest loadDisaster(std::istream& source);

/**
 * Draws the given road network, optionally highlighting some number
 * of the cities in it.
 *
 * @param network The test case containing the network
 * @param selected A set of selected cities to highlight.
 */
void visualizeNetwork(const DisasterTest& network,
                      const Set<std::string>& selected);

#endif
