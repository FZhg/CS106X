#ifndef RecursionToTheRescue_Included
#define RecursionToTheRescue_Included

#include <string>
#include <ostream>
#include "vector.h"
#include "set.h"
#include "map.h"
#include "sparsegrid.h"


/* * * * * Doctors Without Orders * * * * */

/**
 * A type representing a doctor and the number of hours they can work.
 */
struct Doctor {
    std::string name; // Their name, for testing purposes.
    int hoursFree;    // How many hours they have free to work.
};

/**
 * A type representing a patient and the number of hours they need to be seen for.
 */
struct Patient {
    std::string name; // Their name, for testing purposes.
    int hoursNeeded;  // How many hours they must be seen for.
};

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
                          Map<std::string, Set<std::string>>& schedule);

/**
 * @brief canPatientBeAssigned determined whether the patient can be assigned to the doctor
 * according to the schedule right now; Given that the doctor is already a key in the
 * scheduleBoard.
 * @param doctor
 * @param patientToBeAssigned
 * @param scheduleBoard an record Map to show that schedule
 * @return true if the doctor still has hours free for the patient; else, false;
 */
bool canPatientBeAssigned(const Doctor& doctor,
                     const Patient& patientToBeAssigned,
                     const Map<std::string, Vector<Patient>>& scheduleBoard);
/**
 * @brief getPatientsHours return Total Hours needed for a list of patients
 * @param patients a list of patients
 * @return an integer, the total hours for patients in the list
 */
int getPatientsHours(const Vector<Patient>& patients);

/**
 * @brief getDoctorHours add up the total hours free for a list of doctors
 * @param doctors a list of doctors
 * @return an integer. the total hours free for all doctors in the list
 */
int getDoctorHours(const Vector<Doctor>& doctors);

/**
 * @brief toSchedule tranform the convient doctor patient scheduleBoard to string schedule
 * @param doctors the original doctors list
 * @param scheduleBoard the Keys are doctors, the values are lists of patients who are assigned
 *  to a specific doctor
 * @param schedule the outpramater for the canAllPateintsBeSeen Method.
 */
void toSchedule(const Vector<Doctor>& doctors,
                const Map<std::string, Vector<Patient>>& scheduleBoard,
                Map<std::string, Set<std::string>>& schedule);
/**
 * @brief canSolveSchedule store the possible schedule in scheduleBoard and check such
 * possibility exits
 * @param doctors a list of doctors
 * @param patients a list of patients
 * @param scheduleBoard the outParameter
 * @return true, if a schedule is found; false, if not.
 */
bool canSolveSchedule(const Vector<Doctor>& doctors,
                      const Vector<Patient>& patients,
                      Map<std::string, Vector<Patient>>& scheduleBoard);

/* * * * * Disaster Preparation * * * * */

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
bool canBeMadeDisasterReady(const Map<std::string, Set<std::string>>& roadNetwork,
                            const int numCities,
                            Set<std::string>& locations);
/**
 * @brief solveLocations Find cities that should be covered in the constrians of numCities
 * @param CoveredCities A Set that record the cities being covered
 * @param roadNetwork The roadNetwork Map
 * @param numCities  The limit of how many cities can we put emergency  supplies
 * @param locations The locations that record the where we put emergency supplies
 * @return True if the limit can be fulfilled and the roadNetwork is disaster Ready; False, otherwise;
 */
bool solveLocations(const Vector<std::string>& cities,
                    Set<std::string>& coveredCities,
                    const Map<std::string, Set<std::string>>& roadNetwork,
                    const int numCities,
                    Set<std::string>& locations);

/**
 * @brief isDisasterReady Take granted that coveredCities will always be a subset of cities
 * @param coveredCities The cities that has been covered
 * @param cities The total cities on the map
 * @return true, if they are really all covered; false, otherwise;
 */
bool isDisasterReady(const Set<std::string>& coveredCities,
                     const Vector<std::string>& cities);
/**
 * @brief getKeysAsVector extract all the cities keys from the roadNetwork map so
 *  that we can iterate more easily
 * @param roadNetwork: the map that represent the highways between cities
 * @return a list of cities names that are the keys of the map
 */
Vector<std::string> getKeysAsVector(const Map<std::string, Set<std::string>>& roadNetwork);

/**
 * @brief chooseCity Change all the adjecent cities to be covered, add the city to location
 * @param city The city that we will place emergency supplies
 * @param roadNetwork The map of the high ways between cities
 * @param locations The cities that we haved drop emergency supplies on
 * @param coveredCities The cites that has been covered
 * @return a list of cities that been changed
 */
Vector<std::string> chooseCity(std::string city,
               const Map<std::string, Set<std::string>>& roadNetwork,
               Set<std::string>& locations,
               Set<std::string>& coveredCities);

/**
 * @brief unchooseCity remove the city from location list;
 *  remove all cities changed from the previous city choice
 * form coveredcities list;
 * @param city The city that we will place emergency supplies
 * @param roadNetwork The map of the high ways between cities
 * @param locations The cities that we haved drop emergency supplies on
 * @param coveredCities The cites that has been covered
 */
void unchooseCity(std::string city,
               const Map<std::string, Set<std::string>>& roadNetwork,
               Set<std::string>& locations,
               Set<std::string>& coveredCities,
               Vector<std::string>& citiesChanged);
/* * * * * DNA Detective * * * * */


/**
 * Given two DNA strands and a maximum edit distance, returns whether those two strands are
 * within that edit distance of one another.
 *
 * @param one The first DNA strand.
 * @param two The second DNA strand.
 * @param maxDistance The maximum distance between them.
 * @return Whether the two strands are within that edit distance of one another.
 */
bool approximatelyMatch(const std::string& one, const std::string& two, int maxDistance);
/**
 * @brief solveEditDistance Find out possible edit distance that is less than maxDistance
 * @param one DNA sequence
 * @param two another DNA sequence
 * @param distance the distance right now, outparameter
 * @param maxDistance the limit
 * @return true, if the distance < maxDistance in the end; false, otherwise;
 */
bool solveEditDistance(const std::string& one, const std::string& two, int distance, int maxDistance);



/* * * * * Winning the Election * * * * */

/**
 * Type representing a state in an election.
 */
struct State {
    std::string name;   // The name of the state
    unsigned int electoralVotes; // How many electors it has
    unsigned int popularVotes;   // The number of people in that state who voted

};

/**
 * Type representing information about how to win an election with the fewest number
 * of popular votes.
 */
struct MinInfo {
    unsigned int popularVotesNeeded;   // How many popular votes you'd need.
    Vector<State> statesUsed; // Which states you'd carry in the course of doing so.
};

/**
 * Given a list of all the states in an election, including their popular and electoral vote
 * totals, returns information about how few popular votes you'd need to win in order to win
 * the presidency.
 *
 * @param states All the states in the election (plus DC, if appropriate)
 * @return Information about how few votes you'd need to win the election.
 */
MinInfo minPopularVoteToWin(const Vector<State>& states);

/**
 * @brief getVElecVotesByminPopVotes Find the minium number of popular votes needed to get at
 *  least V eletoral votes;
 * @param states The states vector;
 * @param stateIndex We only use states fron stateIndex and greater in the states Vector
 * @param V The number of Electoral Votes that we need to get
 * @param minPopVoteTable The parameter for memoization
 * @return the MinInfo struct Object that contains states we have used and the minimum popular votes
 */
MinInfo getVElecVotesByminPopVotes(const Vector<State>& states,
                                   int stateIndex,
                                   int V,
                                   SparseGrid<MinInfo>& table);
/**
 * @brief getMorePopVotes strict more popular votes to win the electrol votes; E.g., 583, 539 total popular
 * votes needs 270 to win electoral votes
 * @param state The state that we are examing
 * @return the strict more popular
 */
int getMajorityPopVotes(const State& state);

/**
 * @brief getAllElecVotes Count all the Eletoral Votes in the states lists
 * @param states A states lists
 * @return number of All Electoral Votes-
 */
int getAllElecVotes(const Vector<State>& states);
/**
 * @brief getMajorityVotes
 * @param votesAvailable
 * @return majority votes needed to win
 */
int getMajorityVotes(const unsigned int votesAvailable);

int getMajorityElecVotes(const Vector<State> &states);
/* * * * * Printing Routines * * * * */

/* These functions allow you to print out our structs using cout. You don't need to use these
 * functions, but they might be helpful during testing. They're implemented in Printing.cpp
 * in case you're curious how they work.
 */
std::ostream& operator<< (std::ostream& out, const Doctor& doc);
std::ostream& operator<< (std::ostream& out, const Patient& patient);
std::ostream& operator<< (std::ostream& out, const State& state);
std::ostream& operator<< (std::ostream& out, const MinInfo& info);

#endif
