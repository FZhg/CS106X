#ifndef Elections_Included
#define Elections_Included

#include "vector.h"
#include "RecursionToTheRescue.h"

/**
 * Type representing an elections test case.
 */
struct ElectionTest {
    Vector<State> allStates;  // All the states that voted
    int minPopularVoteNeeded; // The popular vote needed
    int year;                 // What year it is
};

/**
 * Given an election year, returns a Vector<State> containing all the
 * election data from that year. If the shouldSimplify flag is set, the
 * data set is first reduced down to ten states.
 *
 * @param year The year in question. Valid years are multiples of four between 1828 - 2016.
 * @param shouldSimplify Whether to use the simplified version of the data rather than the full.
 * @return A list of all the states from the election in that year.
 * @throws ErrorException If an error occurs or the year is invalid.
 */
ElectionTest electionDataFrom(int year, bool shouldSimplify);

/**
 * Shows the outcome of a given election in the visualization window.
 * Because not all states have been around since the inception of the
 * Union (and some didn't participate in 1864), this function needs to
 * know both the list of all the states and which states were won.
 *
 * @param allStates        A list of all states that participated in the election.
 * @param statesWon        A list of the states that were won.
 * @param year             The year of the election.
 * @param totalPopularVote The total number of popular votes cast.
 * @param wonPopularVote   The total number of popular votes won.
 */
void visualizeElection(const Vector<State>& allStates,
                       const Vector<State>& statesWon,
                       int year, int totalPopularVote, int wonPopularVote);

#endif
