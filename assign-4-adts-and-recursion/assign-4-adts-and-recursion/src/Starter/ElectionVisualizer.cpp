#include "gwindow.h"
#include "tokenscanner.h"
#include "gobjects.h"
#include "vector.h"
#include "map.h"
#include "gevents.h"
#include "RecursionToTheRescue.h"
#include "TheWindow.h"
#include "Common.h"
#include <fstream>
#include <memory> // For unique_ptr
#include <string>
#include <iostream>
using namespace std;

/* Everything in here is private to this file. */
namespace {
    /* Master list of all shapes of US states, cached because loading them
     * is really slow.
     *
     * We use GCompound because the GWindow type does not actually have
     * a function that you can use to draw polygons.
     */
    Map<string, GCompound*> theStateShapes;

    /* Colors to use to draw the states. */
    const string kWinFillColor    = "#FFCF60";
    const string kWinBorderColor  = "#806030";
    const string kLoseFillColor   = "#404040";
    const string kLoseBorderColor = "#202020";

    /* Font information. */
    const string kFontColor       = "#FFCF60";
    const string kFont            = "Monospace-BOLD-20";

    /* Text rendering information */
    const double kTextX           = DISPLAY_WIDTH / 2.0;
    const double kYearY           = 50;
    const double kTotalPopVoteY   = 80;
    const double kWonPopVoteY     = 110;

    /* Name of the file containing the state shapes. */
    const string kStateShapeFile  = "Elections/Visualizer/us-borders.txt";

    /* The viewpoint coordinates - the minimum and maximum longitude
     * and latitude.
     */
    const double MIN_LONGITUDE = -180;
    const double MAX_LONGITUDE = -65;

    const double MIN_LATITUDE = +17;
    const double MAX_LATITUDE = +72;

    /* Draws a shape with the specified fill and border colors. */
    void drawState(GCompound* shape,
                   const string& borderColor,
                   const string& fillColor) {
        /* GCompound is not itself a GFillable, but its components
         * are all GPolygons.
         */
        for (int i = 0; i < shape->getElementCount(); i++) {
            /* The dynamic_cast here isn't necessary, but if we decide
             * to do things like add in the number of electoral votes
             * or state names we may need to do some switching logic
             * here. The safety doesn't hurt, either.
             */
            GPolygon* polygon = dynamic_cast<GPolygon*>(shape->getElement(i));
            if (!polygon) error("Expected a GPolygon underlying type.");

            polygon->setColor(borderColor);
            polygon->setFillColor(fillColor);
        }
        theWindow().draw(shape);
    }

    void showStates(const Map<string, bool>& electionMap) {
        clearDisplay();

        for (const string& state: electionMap) {
            /* Make sure we actually have this state in our records. */
            if (!theStateShapes.containsKey(state)) {
                error("State not found: " + state);
            }

            drawState(theStateShapes[state],
                      electionMap[state]? kWinBorderColor : kLoseBorderColor,
                      electionMap[state]? kWinFillColor   : kLoseFillColor);
        }
    }

    /* Given all the states in an election and which ones you'd
     * win to carry the election, builds up an election map that
     * associates each state with whether it's won or lost. Note
     * that not all 50 states (plus DC!) are necessarily going
     * to be here, because they might not exist yet!
     */
    Map<string, bool> electionMapFor(const Vector<State>& allStates,
                                            const Vector<State>& statesWon) {
        /* Two passes: first one defaults everything to false so that
         * it exists; second pass marks the states we won.
         */
        Map<string, bool> result;
        for (const auto& state: allStates) {
            result[state.name] = false;
        }

        for (const auto& state: statesWon) {
            result[state.name] = true;
        }

        return result;
    }

    /**
     * Given a raw longitude, returns the screen x coordinate where
     * it should be displayed.
     *
     * @param longitude The longitude in question.
     * @return Where it maps to as an x coordinate.
     */
    double longitudeToX(double longitude) {
        return DISPLAY_WIDTH * (longitude - MIN_LONGITUDE) / (MAX_LONGITUDE - MIN_LONGITUDE);
    }

    /**
     * Given a raw latitude, returns the screen y coordinate where
     * it should be displayed.
     *
     * @param latitude The latitude in question.
     * @return Where it maps to as a y coordinate.
     */
    double latitudeToY(double latitude) {
        return DISPLAY_HEIGHT * (1.0 - (latitude - MIN_LATITUDE) / (MAX_LATITUDE - MIN_LATITUDE));
    }

    /**
     * Loads a single polygon from the input stream. The polygon format
     * is
     *
     *    Line 1:    Ignore this line
     *    Lines 2-n: Longitude Latitude
     *    Line n+1:  Blank
     */
    GPolygon* loadSingleShapeFrom(istream& input) {
        GPolygon* result = new GPolygon();
        result->setFilled(true);

        /* Skip the first line, since it's in the middle of the region. */
        {
            string throwaway;
            getline(input, throwaway);
        }

        /* Parse the remaining lines until we get to a blank line. */
        for (string line; getline(input, line) && line != ""; ) {
            auto pieces = stringSplit(line, " ");
            double x = longitudeToX(stringToReal(pieces[0]));
            double y =  latitudeToY(stringToReal(pieces[1]));

            result->addVertex(x, y);
        }
        return result;
    }

    /* Loads the state shapes from the data file, storing them into
     * the global variable theStateShapes. This function is
     * idempotent.
     */
    void loadStateShapes() {
        if (!theStateShapes.isEmpty()) return;
        cout << "Just a moment... loading shape files... " << endl;

        ifstream input(kStateShapeFile);
        if (!input) error("Unable to load state shape file.");

        for (string stateName, numShapes;
             getline(input, stateName) && getline(input, numShapes); ){
            /* Read a GCompound from the file consisting of all the
             * pieces of the state.
             */
            GCompound* compound = new GCompound();
            for (int i = 0; i < stringToInteger(numShapes); i++) {
                compound->add(loadSingleShapeFrom(input));
            }
            theStateShapes[stateName] = compound;
        }
    }

    void showStats(int year, int totalPopularVote, int wonPopularVote) {
        /* We have to use a GLabel to render text. Just make one and reuse it. */
        unique_ptr<GLabel> label(new GLabel());
        label->setColor(kFontColor);
        label->setFont(kFont);

        /* Render the year */
        label->setText("Year: " + to_string(year));
        theWindow().draw(label.get(), kTextX, kYearY);

        /* ... and the total popular vote. */
        label->setText("Popular Votes Cast: " + addCommasTo(totalPopularVote));
        theWindow().draw(label.get(), kTextX, kTotalPopVoteY);

        /* ... and the votes you'd need. */
        label->setText("Minimum to Win:     " + addCommasTo(wonPopularVote));
        theWindow().draw(label.get(), kTextX, kWonPopVoteY);
    }


    /* Sets up the election graphics system. This function is
     * idempotent.
     */
    void initializeElectionGraphics() {
        loadStateShapes();
    }
}

/* * * * Exported Functions * * * */

void visualizeElection(const Vector<State>& allStates,
                       const Vector<State>& statesWon,
                       int year, int totalPopularVote, int wonPopularVote) {
    initializeElectionGraphics();
    showStates(electionMapFor(allStates, statesWon));
    showStats(year, totalPopularVote, wonPopularVote);

    /* Show our updates. */
    theWindow().repaint();
}
