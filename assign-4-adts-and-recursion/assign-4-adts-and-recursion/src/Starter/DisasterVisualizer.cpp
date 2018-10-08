#include "Disasters.h"
#include "TheWindow.h"
#include "gobjects.h"
#include <memory>
#include <climits>
#include <algorithm>
using namespace std;

/* Everything in here is private to this file. */
namespace {
    /* Colors to use to draw the network. */
    const string kDarkFillColor    = "#404040";
    const string kDarkBorderColor  = "#202020";
    const string kDarkRoadColor    = "#808080";
    const string kDarkFontColor    = "#a0a0a0";

    const string kLightFillColor   = "#FFCF60";
    const string kLightBorderColor = "#806030";
    const string kLightRoadColor   = "#FFFFFF";
    const string kLightFontColor   = "#000000";

    /* Radius of a city */
    const double kCityRadius = 25;

    /* Buffer space around the window. */
    const double kBufferSpace = 60;

    /* Lower bound on the width or height of the data range, used for
     * collinear points.
     */
    const double kMinDataSeparation = 1e-5;

    /* Max length of a string in a label. */
    const string::size_type kMaxLength = 3;

    /* Geometry information for drawing the network. */
    struct Geometry {
        /* Range of X and Y values in the data set, used for
         * scaling everything.
         */
        double minDataX, minDataY, maxDataX, maxDataY;

        /* Range of X and Y values to use when drawing everything. */
        double minDrawX, minDrawY, maxDrawX, maxDrawY;
    };

    /* Given a data set, fills in the min and max X and Y values
     * encountered in that set.
     */
    void computeDataBounds(const DisasterTest& network, Geometry& geo) {
        geo.minDataX = geo.minDataY = numeric_limits<double>::infinity();
        geo.maxDataX = geo.maxDataY = -numeric_limits<double>::infinity();

        for (const string& cityName: network.cityLocations) {
            geo.minDataX = min(geo.minDataX, network.cityLocations[cityName].getX());
            geo.minDataY = min(geo.minDataY, network.cityLocations[cityName].getY());

            geo.maxDataX = max(geo.maxDataX, network.cityLocations[cityName].getX());
            geo.maxDataY = max(geo.maxDataY, network.cityLocations[cityName].getY());
        }

        /* And now edge cases! Ensure that the difference between these amounts is
         * nonzero and not terribly small.
         */
        geo.maxDataX = max(geo.maxDataX, geo.minDataX + kMinDataSeparation);
        geo.maxDataY = max(geo.maxDataY, geo.minDataY + kMinDataSeparation);
    }

    /* Once we have the data bounds, we can compute the graphics bounds,
     * which will try to take maximum advantage of the width and height
     * that we have available to us.
     */
    void computeGraphicsBounds(const DisasterTest& network,
                               Geometry& geo) {
        /* Get the aspect ratio of the window. */
        double winWidth  = theWindow().getCanvasWidth()  - 2 * kBufferSpace;
        double winHeight = theWindow().getCanvasHeight() - 2 * kBufferSpace;
        double winAspect = winWidth / winHeight;

        /* Get the aspect ratio of the data set. */
        double dataAspect = (geo.maxDataX - geo.minDataX) / (geo.maxDataY - geo.minDataY);

        double dataWidth, dataHeight;

        /* If the data aspect ratio exceeds the window aspect ratio,
         * the limiting factor in the display is going to be the
         * width. Therefore, we'll use that to determine our effective
         * width and height.
         */
        if (dataAspect >= winAspect) {
            dataWidth = winWidth;
            dataHeight = dataWidth / dataAspect;
        } else {
            dataHeight = winHeight;
            dataWidth = dataAspect * dataHeight;
        }

        /* Now, go center that in the window. */
        geo.minDrawX = (winWidth  -  dataWidth) / 2.0 + kBufferSpace;
        geo.minDrawY = (winHeight - dataHeight) / 2.0 + kBufferSpace;

        geo.maxDrawX = geo.minDrawX + dataWidth;
        geo.maxDrawY = geo.minDrawY + dataHeight;
    }

    /* Given the road network, determines its geometry. */
    Geometry geometryFor(const DisasterTest& network) {
        Geometry result;
        computeDataBounds(network, result);
        computeGraphicsBounds(network, result);
        return result;
    }

    /* Converts a coordinate in logical space into a coordinate in
     * physical space.
     */
    GPoint logicalToPhysical(const GPoint& pt, const Geometry& geo) {
        double x = ((pt.getX() - geo.minDataX) / (geo.maxDataX - geo.minDataX)) * (geo.maxDrawX - geo.minDrawX) + geo.minDrawX;
        double y = ((pt.getY() - geo.minDataY) / (geo.maxDataY - geo.minDataY)) * (geo.maxDrawY - geo.minDrawY) + geo.minDrawY;

        return { x, y };
    }

    /* Draws all the roads in the network, highlighting ones that
     * are adjacent to lit cities.
     */
    void drawRoads(const Geometry& geo,
                   const DisasterTest& network,
                   const Set<string>& selected) {
        for (const string& source: network.network) {
            for (const string& dest: network.network[source]) {
                /* Selected roads draw in the bright color; deselected
                 * roads draw in a the dark color.
                 */
                theWindow().setColor((selected.contains(source) || selected.contains(dest))? kLightRoadColor : kDarkRoadColor);

                /* Draw the line, remembering that the coordinates are in
                 * logical rather than physical space.
                 */
                theWindow().drawLine(logicalToPhysical(network.cityLocations[source], geo),
                                     logicalToPhysical(network.cityLocations[dest], geo));
            }
        }
    }

    /* Draws all the cities, highlighting the ones that are in the
     * selected set.
     */
    void drawCities(const Geometry& geo,
                    const DisasterTest& network,
                    const Set<string>& selected) {
        for (const string& city: network.network) {
            /* Figure out the center of the city on the screen. */
            auto center = logicalToPhysical(network.cityLocations[city], geo);

            /* There's no way to draw a filled circle with a boundary as one call. */
            theWindow().setColor(selected.contains(city)? kLightBorderColor : kDarkBorderColor);
            theWindow().fillOval(center.getX() - kCityRadius,
                                 center.getY() - kCityRadius,
                                 2 * kCityRadius,
                                 2 * kCityRadius);

            theWindow().setColor(selected.contains(city)? kLightFillColor : kDarkFillColor);
            theWindow().drawOval(center.getX() - kCityRadius,
                                 center.getY() - kCityRadius,
                                 2 * kCityRadius,
                                 2 * kCityRadius);
        }
    }

    /* Returns a shortened name for the given city name. We use the first
     * three letters of the name if it's a single word and otherwise use
     * its initials.
     */
    string shorthandFor(const string& name) {
        auto components = stringSplit(name, " ");
        if (components.size() == 0) {
            error("It shouldn't be possible for there to be no components of the city name.");
            return "";
        } else if (components.size() == 1) {
            if (components[0].length() < kMaxLength) return components[0];
            else return components[0].substr(0, 3);
        } else {
            /* Use initials. */
            string result;
            for (size_t i = 0; result.length() < kMaxLength && i < components.size(); i++) {
                /* Skip empty components, which might exist if there are consecutive spaces in
                 * the name
                 */
                if (!components[i].empty()) {
                    result += components[i][0];
                }
            }
            return result;
        }
    }

    /* Draws the names of all the cities into the display window,
     * using abbreviations as appropriate.
     */
    void drawCityLabels(const Geometry& geo,
                        const DisasterTest& network,
                        const Set<string>& selected) {
        /* We need to either make a bunch of temporary labels or one
         * persistent one. Here's using the latter approach.
         */
        unique_ptr<GLabel> label(new GLabel());
        label->setFont("Monospace-12");

        for (const string& city: network.network) {
            /* Figure out the center of the city on the screen. */
            auto center = logicalToPhysical(network.cityLocations[city], geo);

            /* Set the label text and color. */
            label->setLabel(shorthandFor(city));
            label->setColor(selected.contains(city)? kLightFontColor : kDarkFontColor);

            theWindow().draw(label.get(),
                             center.getX() - label->getWidth() / 2.0,
                             center.getY() + label->getFontAscent() / 2.0);
        }
    }
}

void visualizeNetwork(const DisasterTest& network,
                      const Set<string>& selected) {
    clearDisplay();

    /* There's a weird edge case where if there are no cities,
     * the window geometry can't be calculated properly. Therefore,
     * we're going skip all this logic if there's nothing to draw.
     */
    if (!network.network.isEmpty()) {
        Geometry geo = geometryFor(network);

        /* Draw the roads under the cities to avoid weird graphics
         * artifacts.
         */
        drawRoads(geo, network, selected);
        drawCities(geo, network, selected);
        drawCityLabels(geo, network, selected);
    }
    theWindow().repaint();
}
