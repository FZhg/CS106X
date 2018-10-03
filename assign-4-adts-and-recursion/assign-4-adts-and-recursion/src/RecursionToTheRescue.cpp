#include "RecursionToTheRescue.h"
#include "map.h"
#include <climits>
#include <iostream>
using namespace std;

/* * * * Doctors Without Orders * * * */

/**
 * Given a list of doctors and a list of patients, determines whether all the patients can
 * be seen. If so, this function fills in the schedule outparameter with a map from doctors
 * to the set of patients that doctor would see.
 *
 * @param doctors  The list of the doctors available to work.
 * @param patients The list of the patients that need to be seen.
 * @param schedule An outparameter that will be filled in with the schedule, should one exist.
 * @return Whether or not a schedule was found.
 */
bool canAllPatientsBeSeen(const Vector<Doctor>& doctors,
                          const Vector<Patient>& patients,
                          Map<string, Set<string>>& schedule) {
    // [TODO: Delete these lines and implement this function!]
    (void)(doctors, patients, schedule);
    return false;
}

/* * * * Disaster Planning * * * */

/**
 * Given a transportation grid for a country or region, along with the number of cities where disaster
 * supplies can be stockpiled, returns whether it's possible to stockpile disaster supplies in at most
 * the specified number of cities such that each city either has supplies or is connected to a city that
 * does.
 * <p>
 * This function can assume that every city is a key in the transportation map and that roads are
 * bidirectional: if there's a road from City A to City B, then there's a road from City B back to
 * City A as well.
 *
 * @param roadNetwork The underlying transportation network.
 * @param numCities   How many cities you can afford to put supplies in.
 * @param locations   An outparameter filled in with which cities to choose if a solution exists.
 * @return Whether a solution exists.
 */
bool canBeMadeDisasterReady(const Map<string, Set<string>>& roadNetwork,
                            int numCities,
                            Set<string>& locations) {
    // [TODO: Delete these lines and implement this function!]
    (void)(roadNetwork, numCities, locations);
    return false;
}

/* * * * DNA Detective * * * */

/**
 * Given two DNA strands and a maximum edit distance, returns whether those two strands are
 * within that edit distance of one another.
 *
 * @param one The first DNA strand.
 * @param two The second DNA strand.
 * @param maxDistance The maximum distance between them.
 * @return Whether the two strands are within that edit distance of one another.
 */
bool approximatelyMatch(const string& one, const string& two, int maxDistance) {
    // [TODO: Delete these lines and implement this function!]
    (void)(one, two, maxDistance);
    return false;
}

/* * * * Winning the Election * * * */

/**
 * Given a list of all the states in an election, including their popular and electoral vote
 * totals, returns information about how few popular votes you'd need to win in order to win
 * the presidency.
 *
 * @param states All the states in the election (plus DC, if appropriate)
 * @return Information about how few votes you'd need to win the election.
 */
MinInfo minPopularVoteToWin(const Vector<State>& states) {
    // [TODO: Delete these lines and implement this function!]
    (void)(states);
    return { 0, {} };
}
