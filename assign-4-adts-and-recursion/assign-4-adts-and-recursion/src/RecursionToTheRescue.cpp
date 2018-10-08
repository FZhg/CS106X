#include "RecursionToTheRescue.h"
#include "map.h"
#include <climits>
#include <iostream>
#include <cmath>
#include "string.h"
using namespace std;

/* * * * Doctors Without Orders * * * */
/* Credit:
 * I took inspirations from GeeksForGeeks,Backtracking 4, subset sum page.
 * The performance for tooManyEven testCase is too long. Dp can help??
 * /

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
    Map<string, Vector<Patient>> scheduleBoard;
    bool result = canSolveSchedule(doctors, patients, scheduleBoard);
    toSchedule(doctors, scheduleBoard, schedule);
    return result;
}

bool canPatientBeAssigned(const Doctor& doctor,
                          const Patient& patientToBeAssigned,
                          const Map<string, Vector<Patient>>& scheduleBoard){
    int hoursAssigned = getPatientsHours(scheduleBoard[doctor.name]);
    return ((hoursAssigned + patientToBeAssigned.hoursNeeded) <= (doctor.hoursFree));
}

int getPatientsHours(const Vector<Patient>& patients){
    int totalHoursNeeded = 0;
    for(int patientNum =0; patientNum < patients.size(); patientNum ++){
        totalHoursNeeded += patients[patientNum].hoursNeeded;
    }
    return totalHoursNeeded;
}

int getDoctorHours(const Vector<Doctor>& doctors){
    int totalHoursFree = 0;
    for(int doctorNum = 0; doctorNum < doctors.size(); doctorNum ++){
        totalHoursFree += doctors[doctorNum].hoursFree;
    }
    return totalHoursFree;
}

void toSchedule(const Vector<Doctor>& doctors,
                const Map<string, Vector<Patient>>& scheduleBoard,
                Map<string, Set<string> > &schedule){
    for(int doctorNum = 0; doctorNum < doctors.size(); doctorNum ++){
        Doctor doctor = doctors[doctorNum];
        for(int patientNum = 0; patientNum < scheduleBoard[doctor.name].size(); patientNum ++){
            Patient patient = scheduleBoard[doctor.name][patientNum];
            schedule[doctor.name].add(patient.name);
        }
    }
}

bool canSolveSchedule(const Vector<Doctor>& doctors,
                      const Vector<Patient>& patients,
                      Map<string, Vector<Patient>>& scheduleBoard){
    if(patients.isEmpty()) return true;
    if(getDoctorHours(doctors) < getPatientsHours(patients)) return false;
    Vector<Doctor> doctorsLeft = doctors;
    Doctor doctor = doctorsLeft[0];
    doctorsLeft.remove(0);
    if(canSolveSchedule(doctorsLeft, patients, scheduleBoard)){
        return true;
    } else{
        Vector<Patient> patientsLeft = patients;
        for(int patientNum = 0; patientNum < patients.size(); patientNum ++){
            Patient patientToAssigned = patients[patientNum];
            if(canPatientBeAssigned(doctor, patientToAssigned, scheduleBoard)){
                scheduleBoard[doctor.name].add(patientToAssigned);
                patientsLeft.remove(patientNum);
                if(canSolveSchedule(doctors, patientsLeft, scheduleBoard)) return true;
                //backtracking
                scheduleBoard[doctor.name].remove(scheduleBoard[doctor.name].size() - 1);
                patientsLeft.insert(patientNum, patientToAssigned);
            }
        }
    }
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
                            const int numCities,
                            Set<string>& locations) {
    Vector<string> cities = getKeysAsVector(roadNetwork);
    Set<string> coveredCities;
    return solveLocations(cities, coveredCities, roadNetwork, numCities, locations);
}

bool solveLocations(const Vector<string> &cities,
                    Set<string> &coveredCities,
                    const Map<string, Set<string> > &roadNetwork,
                    const int numCities,
                    Set<string> &locations){
    // base cases
    if(locations.size() > numCities) return false;
    if(isDisasterReady(coveredCities, cities) && locations.size() <= numCities) return true;

    // two choices either cover the city or cover the adjecent city, then backtrack
    for(int citiyNum = 0; citiyNum < cities.size(); citiyNum ++){
        string city = cities[citiyNum];
        // first choice
        if(!coveredCities.contains(city)){
            Vector<string> citiesChanged = chooseCity(city, roadNetwork, locations, coveredCities);
            if(solveLocations(cities, coveredCities, roadNetwork, numCities,locations)) return true;
            // backtracking
            unchooseCity(city, roadNetwork, locations, coveredCities, citiesChanged);

            //second choice
            for(auto const& aCity : roadNetwork[city]){
                Vector<string> citiesChanged = chooseCity(aCity, roadNetwork, locations, coveredCities);
                if(solveLocations(cities, coveredCities, roadNetwork, numCities, locations)) return true;
                // backtracking
                unchooseCity(aCity, roadNetwork, locations, coveredCities, citiesChanged);
            }
        }
    }
    return false;
}

bool isDisasterReady(const Set<string> &coveredCities, const Vector<string> &cities){
    return coveredCities.size() == cities.size();
}

Vector<string> getKeysAsVector(const Map<string, Set<string> > &roadNetwork){
    Vector<string> keys;
    for(auto const& x : roadNetwork){
        keys.add(x);
    }
    return keys;
}

Vector<string> chooseCity(string city,
                          const Map<string, Set<string> > &roadNetwork,
                          Set<string> &locations,
                          Set<string> &coveredCities){
    locations.add(city);
    Vector<string> citiesChanged;
    if(!coveredCities.contains(city)){
        coveredCities.add(city);
        citiesChanged.add(city);
    }
    for(auto const& aCity : roadNetwork[city]){
        if(!coveredCities.contains(aCity)){
            coveredCities.add(aCity);
            citiesChanged.add(aCity);
        }
    }
    return citiesChanged;
}

void unchooseCity(string city,
                  const Map<string, Set<string> > &roadNetwork,
                  Set<string> &locations,
                  Set<string> &coveredCities,
                  Vector<string>& citiesChanged){
    locations.remove(city);
    for(int cityNum =0; cityNum < citiesChanged.size(); cityNum ++){
        string cityChanged = citiesChanged[cityNum];
        coveredCities.remove(cityChanged);
    }
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
    return solveEditDistance(one, two, 0, maxDistance);
}

bool solveEditDistance(const string &one, const string &two, int distance, int maxDistance){
    //base cases
    if(distance > maxDistance) return false;
    if(one.empty()){
        distance += two.length();
        return distance <= maxDistance;
    }
    if(two.empty()){
        distance += one.length();
        return distance <= maxDistance;
    }
    if(one[0] == two[0]){
        return solveEditDistance(one.substr(1), two.substr(1), distance, maxDistance);
    } else{
        string oneCopy = one;
        // insertion
        oneCopy.insert(oneCopy.begin(), two[0]);
        distance ++;
        if(solveEditDistance(oneCopy.substr(1), two.substr(1), distance, maxDistance)) return true;
        //backtracking
        oneCopy.erase(0, 1);
        distance --;

        //deletions
        char deleted = oneCopy[0];
        oneCopy.erase(0, 1);
        distance ++;
        if(solveEditDistance(oneCopy, two, distance, maxDistance)) return true;
        //backtracking
        oneCopy.insert(oneCopy.begin(), deleted);
        distance--;

        //replacement
        distance ++;
        oneCopy.replace(0, 1, 1, two[0]);
        if(solveEditDistance(oneCopy.substr(1) ,two.substr(1), distance, maxDistance)) return true;
        //backtracking no needed
    }
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
    int majorityEvotes = getMajorityElecVotes(states);
    SparseGrid<MinInfo> table(1000, 1000);
    return getVElecVotesByminPopVotes(states, 0, majorityEvotes, table);
}

MinInfo getVElecVotesByminPopVotes(const Vector<State> &states,
                                   int stateStartIndex,
                                   int V,
                                   SparseGrid<MinInfo>& table){
    MinInfo result; // holder for final result;
    //base case 1
    if(stateStartIndex == states.size() || V == 0){
        result.popularVotesNeeded = INT_MAX;
        return result;
    }

    //base case 2
    State stateTobeAdded = states[stateStartIndex];
    //check can meet the V constrainst
    if((stateStartIndex == states.size() -1)  && (stateTobeAdded.electoralVotes < V)){
        result.popularVotesNeeded = INT_MAX;
        result.statesUsed.add(stateTobeAdded);
        table[stateStartIndex][V] = result;
        return result;
    }

    //base case 3
    if(stateStartIndex == states.size() - 1){
        result.popularVotesNeeded = getMajorityPopVotes(stateTobeAdded);
        result.statesUsed.add(stateTobeAdded);
        table[stateStartIndex][V] = result;
        return result;
    }

    //already computed
    if(table.isSet(stateStartIndex, V)){
        return table[stateStartIndex][V];
    }

    //add the state to optimal subset
    int newElecVotes = V - stateTobeAdded.electoralVotes;
    MinInfo preResult;
    if(newElecVotes < 0){
        preResult = getVElecVotesByminPopVotes(states, stateStartIndex + 1, 1, table);
    } else{
        preResult = getVElecVotesByminPopVotes(states, stateStartIndex + 1, newElecVotes, table);
    }
    result.popularVotesNeeded = preResult.popularVotesNeeded + getMajorityPopVotes(stateTobeAdded);
    result.statesUsed = preResult.statesUsed;
    result.statesUsed.add(stateTobeAdded);

    // don't add the state to the optimal subset
    MinInfo result2 = getVElecVotesByminPopVotes(states, stateStartIndex + 1, V, table);
    if(result.popularVotesNeeded < result2.popularVotesNeeded){
        table[stateStartIndex][V] = result;
    } else{
        table[stateStartIndex][V] = result2;
        result = result2;
    }

    // use the state as optimal subset
    MinInfo result3;
    result3.popularVotesNeeded = getMajorityPopVotes(stateTobeAdded);
    result3.statesUsed.add(stateTobeAdded);
    if(result.popularVotesNeeded < result3.popularVotesNeeded){
        return result;
    } else if(stateTobeAdded.electoralVotes >= V){
        table[stateStartIndex][V] = result3;
        result = result3;
    }
    return result;
}


int getMajorityPopVotes(const State& state){
    return getMajorityVotes(state.popularVotes);
}

int getAllElecVotes(const Vector<State> &states){
    unsigned int numElecVotes = 0;
    for(int stateIndex = 0; stateIndex < states.size(); stateIndex ++){
        numElecVotes += states[stateIndex].electoralVotes;
    }
    return numElecVotes;
}

int getMajorityVotes(const unsigned int votesAvailable){
    return votesAvailable / 2 + 1;
}

int getMajorityElecVotes(const Vector<State> &states){
    unsigned int totalElectVotes = getAllElecVotes(states);
    return getMajorityVotes(totalElectVotes);
}
