#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include "RecursionToTheRescue.h"
#include "DoctorsWithoutOrders.h"
#include "DNADetective.h"
#include "Disasters.h"
#include "Common.h"
#include "console.h"
#include "strlib.h"
#include "vector.h"
#include "Elections.h"
#include "simpio.h"
#include "filelib.h"
using namespace std;

/* Everything in here is private to this file. */
namespace {   

    /* * * * * General I/O Logic * * * * */
    /* Prompts the user for a filename. This should eventually be switched to use openFileDialog,
     * but it seems that doing so has some kind of data race on Macs?
     */
    string haveUserChooseFile(ifstream& input, const string& prompt, const string& directory) {
        while (true) {
            string filename = getLine(prompt);
            string fullname = directory + "/" + filename;

            /* First, try to open the file using the updated path name. */
            {
                ifstream fileReader(fullname);
                if (fileReader) {
                    input = std::move(fileReader);
                    return fullname;
                }
            }

            /* Try to open the file just using the name given. */
            {
                ifstream fileReader(filename);
                if (fileReader) {
                    input = std::move(fileReader);
                    return filename;
                }
            }

            cerr << "Sorry, I can't open that file." << endl;
        }
    }

    /* Prompts the user for a natural number (a nonnegative integer.) */
    int getNaturalNumber(const string& prompt) {
        while (true) {
            int result = getInteger(prompt);
            if (result >= 0) return result;

            cout << "Please enter a nonnegative integer." << endl;
        }
    }

    /* Prompts the user for an answer, reprompting as necessary until they enter something
     * that starts with 'y' or 'n'.
     */
    bool askNicely(const string& prompt) {
        while (true) {
            string line = toLowerCase(getLine(prompt));
            if (startsWith(line, "y")) return true;
            if (startsWith(line, "n")) return false;

            cerr << "Sorry, I didn't understand." << endl;
        }
    }

    /* Given a filename and an expected suffix, checks whether the file
     * ends with that suffix. If not, issues a warning and asks if the
     * user wants to proceed anyway.
     */
    bool proceedWithTest(const string& filename, const string& suffix) {
        if (endsWith(getTail(filename), suffix)) return true;

        cerr << "Warning: This doesn't look like a test case for this part" << endl;
        cerr << "of the assignment. Test cases for this part of the assignment" << endl;
        cerr << "should have the " << suffix << " suffix." << endl;

        return askNicely("Do you want to continue anyway? ");
    }

    /* * * * * Testing logic for Winning the Presidency * * * * */

    /* Range of election years supported. We could in principle pull this data from the files in the
     * directory, but for simplicity we don't do this.
     */
    const int kMinElectionYear = 1828;
    const int kMaxElectionYear = 2016;

    /* Prompts the user for an election year, reprompting as necessary. */
    int chooseYear() {X
        while (true) {
            int year = getInteger("Enter an election year (" + to_string(kMinElectionYear) + " - " + to_string(kMaxElectionYear) + "): ");
            if (year >= kMinElectionYear && year <= kMaxElectionYear && year % 4 == 0) {
                return year;
            }

            cerr << "Please enter a valid election year." << endl;
        }
    }

    /* Returns a test case based on a user-chosen year. If the user wants to simplify the test case,
     * the data is first reduced down to a smaller subset.
     */
    ElectionTest userChosenElectionData(bool shouldSimplify) {
        return electionDataFrom(chooseYear(), shouldSimplify);
    }

    /* Sums up and returns the popular vote for an election year given the voting states. */
    int popularVote(const Vector<State>& states) {
        int result = 0;
        for (const auto& state: states) {
            result += state.popularVotes;
        }
        return result;
    }

    /* Sums up and returns the electoral vote for an election year given the voting states. */
    int electoralVote(const Vector<State>& states) {
        int result = 0;
        for (const auto& state: states) {
            result += state.electoralVotes;
        }
        return result;
    }

    /* Width to pad each state name up to. */
    const int kStatePadding = 35;

    /* Number of states per line. */
    const int kStatesPerLine = 2;

    /* Output all the states included in the winning set for an election. */
    void listStatesUsed(const MinInfo& info) {
        cout << "You'd carry these states: " << endl;

        int index = 0;
        for(const auto& state: info.statesUsed) {
            ostringstream builder;
            builder << state.name << " (" << pluralize(state.electoralVotes, "elector") << ")";

            cout << setw(kStatePadding) << builder.str();

            index++;
            if (index % kStatesPerLine == 0) cout << endl;
        }

        cout << endl;
    }

    /* Shows the result of an election, reporting individual states to the console and displaying
     * the map in the window.
     */
    void reportElectionResult(const ElectionTest& testCase,
                              const MinInfo& info) {
        cout << "Total votes cast that year: " << setw(15) << addCommasTo(popularVote(testCase.allStates)) << endl;
        cout << "Minimum needed to win:      " << setw(15) << addCommasTo(info.popularVotesNeeded) << " " << flush;
        if (testCase.minPopularVoteNeeded != info.popularVotesNeeded) {
            cerr << "(Incorrect; you should need " << addCommasTo(testCase.minPopularVoteNeeded) << ")" << endl;
        } else {
            cout << "(Correct)" << endl;
        }

        cout << "Electoral votes that year:  " << setw(15) << electoralVote(testCase.allStates) << endl;
        cout << "Electoral votes you'd earn: " << setw(15) << electoralVote(info.statesUsed) << " " << flush;
        if (electoralVote(info.statesUsed) * 2 <= electoralVote(testCase.allStates)) {
            cerr << "(That isn't a majority!)" << endl;
        } else {
            cout << "(A majority)" << endl;
        }

        listStatesUsed(info);
        visualizeElection(testCase.allStates, info.statesUsed, testCase.year, popularVote(testCase.allStates), info.popularVotesNeeded);
    }

    /* Prompts the user to test out their election code on given years. */
    void testMinPopularVoteToWin(bool shouldSimplify) {
        do {
            ElectionTest testCase = userChosenElectionData(shouldSimplify);
            reportElectionResult(testCase, minPopularVoteToWin(testCase.allStates));
        } while (askNicely("Another year? "));
    }

    /* Takes a trip through time, reporting how skewed an election
     * could be over time.
     */
    void visualizeHistoricalElections() {
        for (int year = kMinElectionYear; year <= kMaxElectionYear; year += 4) {
            cout << "Year: " << year << endl;
            auto data = electionDataFrom(year, false);

            MinInfo result = minPopularVoteToWin(data.allStates);
            reportElectionResult(data, result);
            getLine("Press ENTER to continue...");
        }
    }

    /* * * * * Logic for Doctors Without Orders * * * * */

    /* Prompts the user to choose a hospital test case, returning the contents of that test. */
    HospitalTestCase userChooseHospitalTest() {
        while (true) {
            ifstream input;
            string filename = haveUserChooseFile(input, "Enter test file name: ", "DoctorsWithoutOrders");

            if (!filename.empty() && input && proceedWithTest(filename, ".dwo")) {
                return loadHospitalTestCase(input);
            }
        }
    }

    /* Prints out a human-readable representation of the schedule. */
    void printScheduleNicely(const Set<string>& toSee, const HospitalTestCase& testCase, int hoursFree) {
        /* Convert the patients into a map from patients to hours. */
        Map<string, int> patientsToHours;
        for (const auto& patient: testCase.patients) {
            patientsToHours[patient.name] = patient.hoursNeeded;
        }

        /* List all the patients with their hours needed */
        int totalHours = 0;
        for (const auto& patient: toSee) {
            cout << "    Patient " << patient << " (" << pluralize(patientsToHours[patient], "hour") << " needed)" << endl;
            totalHours += patientsToHours[patient];
        }

        if (totalHours > hoursFree) {
            cerr << "  This doctor's demands exceed their available hours!" << endl;
        }
    }

    /* Given a schedule and the list of all doctors and patients, reports whether all patients
     * were actually seen.
     */
    bool allPatientsSeen(const HospitalTestCase& testCase, const Map<string, Set<string>>& schedule) {
        Set<string> seen;
        for (const string& doctor: schedule) {
            seen += schedule[doctor];
        }

        Set<string> allPatients;
        for (const auto& patient: testCase.patients) {
            allPatients += patient.name;
        }

        return seen == allPatients;
    }

    /* Given a test case, runs the student code on that test case, reporting the result and any
     * mismatches with the file.
     */
    void processHospitalTestCase(const HospitalTestCase& testCase) {
        Map<string, Set<string>> schedule;
        if (canAllPatientsBeSeen(testCase.doctors, testCase.patients, schedule)) {
            /* Report the answer either to stdout or to stderr depending
             * on whether they got the answer right
             */
            cout << "Good news! Everyone can indeed be seen." << endl;
            for (const auto& doctor: testCase.doctors) {
                cout << "  Doctor " << doctor.name << " (" << pluralize(doctor.hoursFree, "hour") << " free) sees these patients: " << endl;
                printScheduleNicely(schedule[doctor.name], testCase, doctor.hoursFree);
            }

            if (!allPatientsSeen(testCase, schedule)) {
                cerr << "Not all patients were scheduled to be seen!" << endl;
            }
            if (!testCase.isPossible) {
                cerr << "The test case reports that there should not be a way to schedule everyone!" << endl;
            }
        } else {
            cout << "Oh no! Not all patients can be seen." << endl;

            if (testCase.isPossible) {
                cerr << "The test case reports that a schedule should exist!" << endl;
            }
        }
    }

    /* Displays the doctors and patients in a given hospital test case. */
    void showHospitalTest(const HospitalTestCase& test) {
        cout << "Here are the available doctors: " << endl;
        for (const auto& doctor: test.doctors) {
            cout << "   Doctor " << doctor.name << " (" << pluralize(doctor.hoursFree, "hour") << " free)" << endl;
        }
        cout << "Here are the patients that need to be seen: " << endl;
        for (const auto& patient: test.patients) {
            cout << "  Patient " << patient.name << " (" << pluralize(patient.hoursNeeded, "hour") << " needed)" << endl;
        }
    }

    /* Prompts the user for a hospital test, runs it, then asks to continue. */
    void testCanPatientsBeSeen() {
        do {
            HospitalTestCase testCase = userChooseHospitalTest();
            showHospitalTest(testCase);
            processHospitalTestCase(testCase);
        } while (askNicely("Another test case? "));
    }

    /* * * * * Disaster Preparation Test Logic * * * * */

    /* Prompts the user to choose a disaster test file, returning the contents of whatever
     * file was chosen.
     */
    DisasterTest userChooseDisasterTest() {
        while (true) {
            ifstream input;
            string filename = haveUserChooseFile(input, "Enter test file name: ", "DisasterPlanning");

            if (!filename.empty() && input && proceedWithTest(filename, ".dst")) {
                return loadDisaster(input);
            }
        }
    }

    /* Confirms that all cities are indeed covered by a set of test cities. */
    bool allCitiesCovered(const Set<string>& stockpileCities, const Map<string, Set<string>>& network) {
        Set<string> covered = stockpileCities;
        for (const string& city: stockpileCities) {
            covered += network[city];
        }

        Set<string> allCities;
        for (const string& city: network) {
            allCities += city;
        }

        return allCities == covered;
    }

    /* Runs the student code on the given disaster test, showing the result and comparing it against
     * our reference answer.
     */
    void runOneDisasterTest(const DisasterTest& test, int numCities) {
        Set<string> stockpileCities;
        if (canBeMadeDisasterReady(test.network, numCities, stockpileCities)) {
            cout << "Yes! You can prepare for a disaster." << endl;
            cout << "Specifically, stockpile in these cities: " << endl;
            for (const string& city: stockpileCities) {
                cout << "  " << city << endl;
            }
            visualizeNetwork(test, stockpileCities);

            if (numCities < test.minCitiesNeeded) {
                cerr << "Your answer uses fewer cities than the test case says should be possible." << endl;
            }
            if (stockpileCities.size() > numCities) {
                cerr << "You've used more than " << pluralize(numCities, "city", "cities") << " in your solution!" << endl;
            }
            if (!allCitiesCovered(stockpileCities, test.network)) {
                cerr << "Your answer does not provide disaster coverage to all cities." << endl;
            }
        } else {
            visualizeNetwork(test, {});
            cout << "Unfortunately, that's not enough cities to stockpile." << endl;
            if (numCities >= test.minCitiesNeeded) {
                cerr << "The test case says that this should be possible." << endl;
            }
        }
    }

    /* Given a disaster test, loops while the student wants to run that test case on a bunch of
     * different numbers.
     */
    void processDisasterTest(const DisasterTest& test) {
        /* Pull up the map so we can see what we're looking at. */
        visualizeNetwork(test, {});

        while (true) {
            string input = getLine("Enter a number of cities, or press ENTER to end: ");
            if (trim(input).empty()) break;

            /* The input might not be numeric. If it isn't, reprompt. */
            int numCities;
            try {
                numCities = stringToInteger(input);
            } catch (...) {
                cout << "Please enter an integer." << endl;
                continue;
            }

            runOneDisasterTest(test, numCities);
        }
    }

    /* Prompts the user to try out the disaster prep code on a number of test cases. */
    void testCanBeMadeDisasterReady() {
        do {
            auto testCase = userChooseDisasterTest();
            processDisasterTest(testCase);
        } while (askNicely("Another test case?" ));
    }

    /* Uses binary search to find the optimal number of cities to use for disaster
     * preparedness, populating the result field with the minimum group of cities
     * that ended up being needed.
     */
    int minCitiesNeededForPreparedness(const DisasterTest& test, Set<string>& result) {
        /* The variable 'low' is the lowest number that might be feasible.
         * The variable 'high' is the highest number that we know is feasible.
         */
        int low = 0, high = test.network.size();

        while (low < high) {
            /* This line looks weird, but it's designed to avoid integer overflows
             * on large inputs. The idea that (high + low) can overflow, but
             * (high - low) / 2 never will.
             */
            int mid = low + (high - low) / 2;
            Set<string> thisResult;

            cout << "  Checking whether it's feasible to stockpile in " << pluralize(mid, "city", "cities") << "..." << endl;

            /* If this option works, decrease high to it, since we know all is good. */
            if (canBeMadeDisasterReady(test.network, mid, thisResult)) {
                high = mid;
                result = thisResult; // Remember this result for later.
            }
            /* Otherwise, rule out anything less than or equal to it. */
            else {
                low = mid + 1;
            }
        }

        return low; // == high
    }

    /* Tests student code for disaster preparedness for optimality. */
    void confirmDisasterOptimality(const DisasterTest& test) {
        /* Pull up the test so they can at least see the network. */
        visualizeNetwork(test, {});

        Set<string> bestCities;
        int result = minCitiesNeededForPreparedness(test, bestCities);
        visualizeNetwork(test, bestCities);

        cout << "Your code reports that " << pluralize(result, "city", "cities") << " are required." << endl;
        if (result != test.minCitiesNeeded) {
            cerr << "Our test case reports that the correct answer is " << test.minCitiesNeeded << endl;
        }
    }

    /* Prompts the user to choose a test file that will be hit with an optimality test. */
    void testOptimalDisasterPrep() {
        cout << "In this setting, we're going to use binary search, plus your" << endl;
        cout << "canBeMadeDisasterReady function, to determine the minimal number" << endl;
        cout << "of cities in which you could stockpile supplies for an emergency." << endl;
        cout << endl;
        cout << "These tests may take some time to complete depending on how you" << endl;
        cout << "implemented your function and which test case you choose." << endl;
        do {
            auto testCase = userChooseDisasterTest();
            confirmDisasterOptimality(testCase);
        } while (askNicely("Another test case?" ));
    }

    /* * * * * Testing code for DNA Detective * * * * */

    /* Prompts the user to enter a genome. */
    string getDNA(const string& prompt) {
        while (true) {
            string line = toUpperCase(getLine(prompt));
            if (line.find_first_not_of("ACGT") == string::npos) return line;

            cerr << "Please enter a valid DNA strand using the characters A, C, G, and T." << endl;
        }
    }

    /* Tests whether two DNA strands approximately match one another. */
    void analyzeDNAMatch(const string& first, const string& second, int k) {
        if (approximatelyMatch(first, second, k)) {
            cout << "Those strands approximately match." << endl;
        } else {
            cout << "Those strands do not match." << endl;
        }
    }

    /* Sits in a loop to let the user test out the DNA Detective assignment on direct inputs. */
    void testApproximatelyMatchDirect() {
        do {
            string first  = getDNA("Enter first strand:  ");
            string second = getDNA("Enter second strand: ");
            int k         = getInteger("Enter max edit distance: ");

            if (approximatelyMatch(first, second, k)) {
                cout << "Those strands approximately match." << endl;
            } else {
                cout << "Those strands do not match." << endl;
            }
        } while (askNicely("Another test case? "));
    }

    /* Prompts the user to choose a DNA Detective test case file, reprompting as necessary
     * until one is selected.
     */
    DNATestCase userChooseDNATestCase() {
        while (true) {
            ifstream input;
            string filename = haveUserChooseFile(input, "Enter test file name: ", "DNADetective");

            if (!filename.empty() && input && proceedWithTest(filename, ".dna")) {
                return loadDNATestCase(input);
            }
        }
    }

    /* Prompts the user for a DNA Detective test file, then tests the student code against
     * what's in the test file.
     */
    void testApproximatelyMatchFile() {
        do {
            DNATestCase test = userChooseDNATestCase();

            cout << "  First strand:  " << test.first << endl;
            cout << "  Second strand: " << test.second << endl;
            cout << "  Max Distance:  " << test.k << endl;

            if (approximatelyMatch(test.first, test.second, test.k)) {
                ostream& out = (test.result? cout : cerr);
                out << "Those strands approximately match." << endl;

                if (!test.result) {
                    cerr << "The test case says that these strands should not match!" << endl;
                }

            } else {
                ostream& out = (!test.result? cout : cerr);
                out << "Those strands do not match." << endl;

                if (test.result) {
                    cerr << "The test case says that these strands should match!" << endl;
                }
            }
        } while (askNicely("Another test case? "));
    }

    /* * * * * Main Driver Logic * * * * */

    /* Enumerated type representing all the different menu options. */
    enum MenuOption {
        LargestInvalidOption,
        TestDoctorsWithoutOrders,
        TestDisasterPreparation,
        TestOptimalDisasterPreparation,
        TestDNADetectiveDirect,
        TestDNADetectiveFile,
        TestSimpleElections,
        TestWinningTheElection,
        ShowOffWinningTheElection,
        Quit,
        NumOptions
    };

    /* Invokes the proper testing code for a given selection. */
    void processMenuSelection(MenuOption option) {
        switch (option) {
        case TestDoctorsWithoutOrders:
            testCanPatientsBeSeen();
            break;
        case TestDisasterPreparation:
            testCanBeMadeDisasterReady();
            break;
        case TestOptimalDisasterPreparation:
            testOptimalDisasterPrep();
            break;
        case TestDNADetectiveDirect:
            testApproximatelyMatchDirect();
            break;
        case TestDNADetectiveFile:
            testApproximatelyMatchFile();
            break;
        case TestSimpleElections:
            testMinPopularVoteToWin(true);
            break;
        case TestWinningTheElection:
            testMinPopularVoteToWin(false);
            break;
        case ShowOffWinningTheElection:
            visualizeHistoricalElections();
            break;
        case Quit:
            cout << "Have a nice day!" << endl;
            exit(0);
        default:
            error("Unknown menu option?");
        }
    }

    /* Displays the menu of what's available. */
    void printMenu() {
        cout << "Enter your selection: " << endl;
        cout << "  " << TestDoctorsWithoutOrders         << ": Test Doctors Without Orders" << endl;
        cout << "  " << TestDisasterPreparation          << ": Test Disaster Preparation" << endl;
        cout << "  " << TestOptimalDisasterPreparation   << ": Find Minimum Coverage for a Disaster" << endl;
        cout << "  " << TestDNADetectiveDirect           << ": Test DNA Detective on Direct Input" << endl;
        cout << "  " << TestDNADetectiveFile             << ": Test DNA Detective on a Test File" << endl;
        cout << "  " << TestSimpleElections              << ": Test Winning the Election on Simplified Test Cases" << endl;
        cout << "  " << TestWinningTheElection           << ": Test Winning the Election on Historical Data" << endl;
        cout << "  " << ShowOffWinningTheElection        << ": View Historical Election Patterns" << endl;
        cout << "  " << Quit                             << ": Quit" << endl;
    }

    /* Prompts the user for a menu selection, reprompting as necessary. */
    MenuOption getMenuSelection() {
        while (true) {
            MenuOption option = MenuOption(getInteger("Enter your choice: "));
            if (option > LargestInvalidOption && option < NumOptions) {
                return option;
            }
            cerr << "Sorry, that choice was invalid." << endl;
        }
    }
}

int main() {
    while (true) {
        printMenu();
        processMenuSelection(getMenuSelection());
    }
    return 0; // Unreachable; silences warnings
}
