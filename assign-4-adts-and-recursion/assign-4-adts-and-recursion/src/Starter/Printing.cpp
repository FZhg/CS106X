#include "RecursionToTheRescue.h"
#include "Common.h"
#include <sstream>
#include <climits>
using namespace std;

/* Prints a human-readable version of a Doctor struct. */
ostream& operator<< (ostream& out, const Doctor& doc) {
    ostringstream result;
    result << "[ Doctor " << doc.name << " ("
           << pluralize(doc.hoursFree, "hour") << " free) ]";
    return out << result.str();
}

/* Prints a human-readable version of a Patient struct. */
ostream& operator<< (ostream& out, const Patient& patient) {
    ostringstream result;
    result << "[ Patient " << patient.name << " ("
           << pluralize(patient.hoursNeeded, "hour") << " needed) ]";
    return out << result.str();
}

/* Prints a human-readable version of a state. */
ostream& operator<< (ostream& out, const State& state) {
    ostringstream result;
    result << "[ " << state.name << ": "
           << pluralize(state.popularVotes, "popular vote") << ", "
           << pluralize(state.electoralVotes, "electoral vote")
           << " ]";
    return out << result.str();
}

/* Prints a human-readable version of a MinInfo structure. */
ostream& operator<< (ostream& out, const MinInfo& info) {
    ostringstream result;

    /* Assume students are using INT_MAX as a sentinel. */
    if (info.popularVotesNeeded == INT_MAX) {
        result << "[ impossible ]";
    } else {
        result << "[ " << pluralize(info.popularVotesNeeded, " vote") << " needed, carrying " << info.statesUsed << " ]";
    }

    return out << result.str();
}
