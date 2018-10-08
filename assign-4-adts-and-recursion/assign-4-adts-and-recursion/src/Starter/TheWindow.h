#ifndef TheWindow_Included
#define TheWindow_Included

#include "gwindow.h"
#include <string>

/* Background color. */
static const std::string kBackgroundColor = "#000000";

/* Size of the display window. */
static const double DISPLAY_WIDTH = 962;
static const double DISPLAY_HEIGHT = 700;

/**
 * Returns the window used for graphics in this assignment, creating one
 * if necessary.
 *
 * @return The graphics window.
 */
GWindow& theWindow();

/**
 * Clears the display, resetting it to its default color.
 */
void clearDisplay();

#endif
