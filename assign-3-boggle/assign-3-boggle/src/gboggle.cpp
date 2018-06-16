/*
 * File: gboggle.cpp
 * -----------------
 * Implements the graphics support functions for Boggle program.
 */

/*
 * General implementation notes
 * ----------------------------
 * This file implements the graphics support functions for the Boggle
 * program.  It makes straightforward use of the extended graphics
 * library to draw the cubes, board, letters, scoreboard, etc. The
 * implementations of the individual functions are fairly simple
 * and many of the helper functions do not require individual comments.
 * For descriptions of the behavior of the exported functions,
 * please see the interface file.
 */

#include <string>
#include "error.h"
#include "gboggle.h"
#include "gtypes.h"
#include "gwindow.h"
#include "gobjects.h"
#include "strlib.h"
using namespace std;

/*
 * Constants
 * ---------
 * To configure fixed spacing, fonts, etc.
 */

const int MAX_DIMENSION = 5;

const double EPSILON = 0.0001;         /* constant for antialiasing          */
const double BOARD_BORDER = 8;         /* width of border around the cubes   */
const double INDENT = 10;              /* indent at vertical edges of window */
const double LABEL_HEIGHT = 40;        /* space at top of window for labels  */
const double LABEL_DY = -5;            /* adjustment for label text          */
const double WORD_DY = 2;              /* adjustment for word list           */
const double HUMAN_PERCENTAGE = 0.38;  /* percentage of space for word lists */
                                       /* to apportion to human              */
const string CUBE_FONT = "Helvetica";  /* Font for letters on cube face      */
const double FONT_PERCENTAGE = 0.6;    /* percentage of cube used for letter */
const string SCORE_FONT = "Helvetica"; /* Font for scoreboard names & scores */
const int SCORE_FONT_SIZE = 18;
const int WORD_FONT_SIZE = 10;
const string WORD_FONT = "Geneva";

const string BOARD_COLOR = "#FFEE66";
const string DIE_COLOR = "#E6E6E6";
const string LETTER_COLOR = "#023BD6";
const string WORD_COLOR = "#02805D";
const string LABEL_COLOR = "#028040";

/*
 * A static struct is used to gather the state that is local to this module.
 * Mostly this is geometry calculated to fit the cubes, word lists,
 * and labels neatly within the current size of the graphics window.
 * In addition, the module tracks the count of words listed for each player
 * (to place additional words in the correct location in the displayed word
 * list) and the scores for each player (which must be saved and erased
 * before updating). There is also a 2-d array of the letters currently
 * showing on the cubes, to enable drawing them inverted for the
 * highlighting function.
 */

// We'd like to use GRectangle instead, but it doesn't support setter methods.
struct RectStruct { double x, y, w, h; };

static struct {
   int numWords[2];                    /* words displayed for each player    */
   int scores[2];                      /* scores for each player             */
   RectStruct scoreBox[2];             /* rectangle enscribing the label +   */
                                       /* score for each player              */
   GLabel *scoreLabels[2];             /* labels for player scores           */
   double wordColumnWidth;             /* width of word list columns         */
   double cubeSize;                    /* size of cube                       */
   double fontSize;                    /* font size used for labeling cubes  */
   RectStruct board;                   /* rectangle enscribed the cubes      */
   int numRows, numCols;               /* dimensions of cube layout on board */
   char letters[MAX_DIMENSION][MAX_DIMENSION];
} gState;

/* Global pointer to the graphics window */

static GWindow *gwp;

/*
 * These are the prototypes for functions private to this module.  These
 * helper functions are used to implement the functions which are exported
 * from this module (see .h for prototypes of the exported functions)
 */

static GLabel *constructLabel(string str, string font, string color);
static void fillBox(double x, double y, double width, double height, string color);
static void drawRoundedRect(double x, double y, double width, double height, double r, string color);
static void fillRoundedRect(double x, double y, double width, double height, double r, string color);
static void drawAndFillRoundedRect(double x, double y, double width, double height, double r, string color);
static void drawCenteredChar(double centerX, double centerY, char ch);
static void drawCube(int row, int col, char ch, bool invert);
static double cubeX(int col);
static double cubeY(int row);
static void drawPlayerLabel(Player player, string name);
static void drawEmptyCubes();
static void drawOneScore(Player playerNum, int value);
static void addToScoreForPlayer(int pointsToAdd, Player playerNum);
static void calculateGeometry(int numRows, int numCols);

void initGBoggle(GWindow& gw) {
   gwp = &gw;
   gwp->setRepaintImmediately(true);
}

/*
 * Function: drawBoard
 * -------------------
 * Exported function to draw a new board.  Sets up named colors, draws the
 * yellow border, draws the empty cubes, the labels on the scoreboard, and
 * resets scores to zero and words lists to empty.
 */
void drawBoard(int numRows, int numCols) {
   if (numRows < 0 || numRows > MAX_DIMENSION
                   || numCols < 0 || numCols > MAX_DIMENSION) {
      error("drawBoard called with invalid dimensions.");
   }
   gwp->clear();
   gwp->setWindowTitle("Welcome to Boggle!");
   calculateGeometry(numRows, numCols);

   // Draws a filled rect underneath cubes in the oh-so-familiar yellow color
   fillBox(gState.board.x, gState.board.y, gState.board.w, gState.board.h, BOARD_COLOR);
   drawEmptyCubes();
   drawPlayerLabel(HUMAN, "Me");
   drawPlayerLabel(COMPUTER, "Computer");
}

/*
 * Function: labelCube
 * -------------------
 * Exported function used to draw letters on cubes.  Set the 2-d array
 * at this position to hold the letter and then draw the cube in the
 * unhighlighted version with the letter added.
 */

void labelCube(int row, int col, char letter) {
   if (row < 0 || row >= gState.numRows || col < 0 || col >= gState.numCols) {
      error("labelCube called with invalid row, col arguments.");
   }
   gState.letters[row][col] = letter;
   drawCube(row, col, letter, false);
}

/*
 * Function: highlightCube
 * -----------------------
 * Exported function used to turn off and on highlight for cubes.  This
 * function is intended to highlight a cube which is currently being used
 * to form a word.
 */

void highlightCube(int row, int col, bool flag) {
   if (row < 0 || row >= gState.numRows || col < 0 || col >= gState.numCols) {
      error("labelCube called with invalid row, col arguments.");
   }
   drawCube(row, col, gState.letters[row][col], flag);
}

/*
 * Function: drawEmptyCubes
 * ------------------------
 * Draws all of the cubes in position.  The cubes are drawn blank,
 * the client will fill in the letters by calling the exported function
 * function labelCube.
 */
static void drawEmptyCubes() {
   for (int row = 0; row < gState.numRows; row++)
      for (int col = 0; col < gState.numCols; col++)
          drawCube(row, col, ' ', false);   // erase all cubes, start with blank chars
}

/*
 * Function: drawCube
 * ------------------
 * Draws one cube in its position, including the letter centered in the
 * middle of the cube.  If invert flag is true, reverse background and
 * letter colors.
 */

static void drawCube(int row, int col, char ch, bool invert) {
   drawAndFillRoundedRect(cubeX(col), cubeY(row),
                          gState.cubeSize, gState.cubeSize,
                          gState.cubeSize/5.0,
                          invert ? LETTER_COLOR : DIE_COLOR);
   gwp->setColor(invert ? DIE_COLOR : LETTER_COLOR);
   drawCenteredChar(cubeX(col) + gState.cubeSize/2.0,
                    cubeY(row) + gState.cubeSize/2.0, ch);
}

/*
 * Function: constructLabel
 * ------------------------
 * Dynamically allocates and initializes a new GLabel around the
 * provided text with the specifed color and font.
 */

static GLabel *constructLabel(string str, string font, string color) {
    GLabel *label = new GLabel(str);
    label->setFont(font);
    label->setColor(color);
    return label;
}

/*
 * Function: drawPlayerLabel
 * -------------------------
 * Labels player word list with specified name and draws a line
 * underneath the label.  It also resets the player's score and
 * word list count back to zero.
 */

static void drawPlayerLabel(Player player, string name) {
   gwp->drawLine(gState.scoreBox[player].x,
                 gState.scoreBox[player].y,
                 gState.scoreBox[player].x + gState.scoreBox[player].w,
                 gState.scoreBox[player].y);
   GLabel *playerLabel = constructLabel(name, SCORE_FONT + "-" + integerToString(SCORE_FONT_SIZE), LABEL_COLOR);
   gwp->add(playerLabel, gState.scoreBox[player].x, gState.scoreBox[player].y + LABEL_DY);
   gState.scores[player] = gState.numWords[player] = 0;
}

/*
 * Function: drawOneScore
 * ----------------------
 * Draws the specified score for the player according to player enum.
 */

static void drawOneScore(Player playerNum, int value) {
   gState.scoreLabels[playerNum] =
           constructLabel(integerToString(value), SCORE_FONT + "-" + integerToString(SCORE_FONT_SIZE), LABEL_COLOR);
   gwp->add(gState.scoreLabels[playerNum],
            gState.scoreBox[playerNum].x + gState.scoreBox[playerNum].w - gState.scoreLabels[playerNum]->getWidth(),
            gState.scoreBox[playerNum].y + LABEL_DY);
}

/*
 * Function: eraseOldScore
 * -----------------------
 * I used to do this with SetEraseMode, but that left cruft behind, so instead
 * paint an opaque white box over the old score.
 */

static void eraseOldScore(Player playerNum) {
   gwp->remove(gState.scoreLabels[playerNum]);
}

/*
 * Function: addToScoreForPlayer
 * -----------------------------
 * Updates a player's score by the specified number of points.
 * Note that we must first erase the old score before drawing
 * the new one (otherwise we scribble & create chaos).  The
 * scores are kept in an array that is private to this module.
 */

static void addToScoreForPlayer(int pointsToAdd, Player playerNum) {
   eraseOldScore(playerNum);
   gState.scores[playerNum] += pointsToAdd;
   drawOneScore(playerNum, gState.scores[playerNum]);
}

/*
 * Function: recordWordForPlayer
 * -----------------------------
 * Exported function to add a word to a player's list and update
 * their score accordingly.  It keeps track of the number of
 * words recorded for each player and position each successive word
 * in rows and columms from left to right, top to bottom.  It
 * also updates the scoreboards with point additions.
 */
void recordWordForPlayer(const string& word, Player player) {
   if (player != HUMAN && player != COMPUTER) {
      error("recordWordForPlayer called with invalid player argument.");
   }
   string normalized = toLowerCase(word);
   GLabel *label = constructLabel(normalized, WORD_FONT + "-" + integerToString(WORD_FONT_SIZE), WORD_COLOR);
   int numWordsInRow = int(gState.scoreBox[player].w/gState.wordColumnWidth);
   int row = gState.numWords[player] / numWordsInRow;
   int col = gState.numWords[player] % numWordsInRow;
   gwp->add(label,
            gState.scoreBox[player].x + col * gState.wordColumnWidth,
            gState.scoreBox[player].y + (row + 1) * WORD_FONT_SIZE + WORD_DY);
   gState.numWords[player]++;
   addToScoreForPlayer(normalized.length() - 3, player);
   if (col == numWordsInRow - 1) gwp->repaint();
}

/**
 * Function: repaint
 * -----------------
 * Forces a refresh of the graphics window.
 */
void repaint() {
    gwp->repaint();
}

/*
 * Handy utility functions to return coordinates of the lower-left
 * corner of the cube at position (row, col).  The cubes are numbered
 * from top to bottom, left to right starting from zero. Therefore,
 * the upper left corner is (0,0), lower right is (numRows-1,numCols-1)
 */

static double cubeX(int col) {
   return gState.board.x + BOARD_BORDER/2.0 + gState.cubeSize*col + EPSILON;
}

static double cubeY(int row) {
   return gState.board.y + BOARD_BORDER/2.0 + gState.cubeSize*row + EPSILON;
}

/* Function: calculateGeometry
 * ---------------------------
 * This internal helper does all the messy math to work out how to divide
 * up the space within the current graphics window to neatly fit the board,
 * the cubes, and the word lists.
 */

static void calculateGeometry(int numRows, int numCols) {
   double boardSize = min(gwp->getWidth()/3, gwp->getHeight() - LABEL_HEIGHT);
   gState.cubeSize = min((boardSize-BOARD_BORDER)/numRows,
                         (boardSize-BOARD_BORDER)/numCols);
   gState.board.w = gState.board.h = boardSize;
   gState.board.y = LABEL_HEIGHT;
   double leftover = gwp->getWidth() - gState.board.w - 2*INDENT;
   gState.scoreBox[HUMAN].x = INDENT;
   gState.scoreBox[HUMAN].y = gState.scoreBox[COMPUTER].y = gState.board.y;
   gState.scoreBox[HUMAN].h = gState.scoreBox[COMPUTER].h = LABEL_HEIGHT;
   gState.scoreBox[HUMAN].w = leftover*HUMAN_PERCENTAGE;
   gState.board.x = gState.scoreBox[HUMAN].x + gState.scoreBox[HUMAN].w;
   gState.scoreBox[COMPUTER].x = gState.board.x + gState.board.w + INDENT;
   gState.scoreBox[COMPUTER].w = gwp->getWidth()
                               - gState.scoreBox[COMPUTER].x - INDENT;
   gState.scoreLabels[HUMAN] = NULL;
   gState.scoreLabels[COMPUTER] = NULL;
   gState.numRows = numRows;
   gState.numCols = numCols;
   GLabel wordLabel("mmmmm");
   wordLabel.setFont(WORD_FONT + "-" + integerToString(WORD_FONT_SIZE));
   gState.wordColumnWidth = wordLabel.getWidth();
   for (gState.fontSize = 8; gState.fontSize < 48; gState.fontSize++) {
      GLabel label("M"); // the widest letter :)
      label.setFont(CUBE_FONT + "-" + integerToString(int(gState.fontSize)));
      if (label.getWidth() > FONT_PERCENTAGE*gState.cubeSize) break;
   }
}

/*
 * Function: drawCenteredChar
 * --------------------------
 * Used to draw the letters in the center of the cube.
 * Note that this function centers the char both vertically and
 * horizontally around the point specified.
 */

static void drawCenteredChar(double xc, double cy, char ch) {
   string s(1, ch);
   GLabel *label = constructLabel(s, CUBE_FONT + "-" + integerToString(int(gState.fontSize)), "Black");
   gwp->add(label, xc - label->getWidth()/2, cy + 0.4 * gState.fontSize);
}

/*
 * Shape-drawing functions
 * -----------------------
 * These routines use graphics library to draw rects, fill them in,
 * draw rounded corner rects and fill them in, too.
 *
 */

static void fillBox(double x, double y, double width, double height, string color) {
   gwp->setColor(color);
   gwp->fillRect(x, y, width, height);
}

static void fillRoundedRect(double x, double y, double width, double height, double r, string color) {
   gwp->setColor(color);
   gwp->fillRect(x+r, y, width - 2 * r, height);
   gwp->fillRect(x, y+r, width, height - 2 * r);

   GArc *corners[] = {new GArc(x, y, 2 * r, 2 * r, 90, 90),
                      new GArc(x, y + height - 2 * r, 2 * r, 2 * r, 180, 90),
                      new GArc(x + width - 2 * r, y + height - 2 * r, 2 * r, 2 * r, 270, 90),
                      new GArc(x + width - 2 * r, y, 2 * r, 2 * r, 0, 90)};

   for (size_t i = 0; i < 4; i++) {
       corners[i]->setFilled(true);
       corners[i]->setFillColor(color);
       corners[i]->setColor(color);
       gwp->add(corners[i]);
   }
}

static void drawRoundedRect(double x, double y, double width, double height, double r, string color) {
   gwp->setColor(color);
   gwp->drawLine(x + r, y, x + width - r, y);
   gwp->drawLine(x + r, y + height, x + width - r, y + height);
   gwp->drawLine(x, y + r, x, y + height - r);
   gwp->drawLine(x + width, y + r, x + width, y + height - r);

   GArc *corners[] = {new GArc(x, y, 2 * r, 2 * r, 90, 90),
                      new GArc(x, y + height - 2 * r, 2 * r, 2 * r, 180, 90),
                      new GArc(x + width - 2 * r, y + height - 2 * r, 2 * r, 2 * r, 270, 90),
                      new GArc(x + width - 2 * r, y, 2 * r, 2 * r, 0, 90)};

   for (size_t i = 0; i < 4; i++) {
       corners[i]->setColor(color);
       gwp->add(corners[i]);
   }
}

static void drawAndFillRoundedRect(double x, double y, double width, double height, double r, string color) {
   fillRoundedRect(x, y, width, height, r, color);
   drawRoundedRect(x, y, width, height, r, "Black");
}
