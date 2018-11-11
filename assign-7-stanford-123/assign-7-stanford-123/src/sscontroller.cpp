/**
 * File: sscontroller.cpp
 * ----------------------
 * This file implements the command-line controller for the Stanford 1-2-3 program.
 *
 * Fall 2007 Julie Zelenski (all initial work)
 * Autumn 2009 Jerry Cain (upgrade to use new libraries)
 * Autumn 2012 Jerry Cain (upgrade to use even newer libraries)
 * Autumn 2018 Jerry Cain (upgrade to use even *newer* libraries)
 */

#include <iostream>
#include <fstream>
#include <iomanip>

#include "simpio.h"
#include "map.h"
#include "tokenscanner.h"
#include "error.h"
#include "ssutil.h"
#include "ssmodel.h"
#include "ssview.h"
#include "console.h"
using namespace std;

/**
 * General implementation notes
 * ----------------------------
 * The controller module implements the command-line interface to the spreadsheet
 * program.  The interface can easily be extended with additional
 * commands by writing a handler function for the command and then adding 
 * an entry to the table.
 */

/**
 * Typedef: cmdFnT
 * ---------------
 * This typedef makes a shorthand name for the function prototype
 * so we can refer to it as cmdFnT instead of the long form 
 */

typedef void (*cmdFnT)(TokenScanner& scanner, SSModel& model);

/**
 * Function: helpAction, loadAction, saveAction, etc.
 * ---------------------------------------------------------
 * Each of these handles one specific command.  In most cases, it is
 * simply a matter of using the scanner to read the following arguments and
 * then acting on the command by sending the appropriate message to the model.
 */

static const int kLeftColumnWidth = 22;
static void helpAction(TokenScanner& /* scanner */, SSModel& /* model */) {
	cout << left << setw(kLeftColumnWidth) 
         << "help" << "Print this menu of commands" << endl;
	cout << left << setw(kLeftColumnWidth) 
         << "load <filename>" << "Read named file into spreadsheet" << endl;
	cout << left << setw(kLeftColumnWidth) 
         << "save <filename>" << "Save current spreadsheet to named file" << endl;
	cout << left << setw(kLeftColumnWidth) 
         << "set <cell> = <value>" 
         << "Set cell to value. Value can be \"string\" or formula" << endl;
	cout << left << setw(kLeftColumnWidth) 
         << "get <cell>" << "Print contents of cell" << endl;
	cout << left << setw(kLeftColumnWidth) 
         << "quit" << "Quit the program" << endl;
	cout << endl;
}

static void loadAction(TokenScanner& scanner, SSModel& model) {
	if (!scanner.hasMoreTokens()) 
        error("The load command requires a file name.");
	
    string filename;
	while (scanner.hasMoreTokens())
		filename += scanner.nextToken();
	ifstream infile(filename.c_str());
	if (infile.fail()) 
        error("Cannot open the file named \"" + filename + "\".");
	model.readFromStream(infile);
	cout << "Loaded file \"" << filename << "\"." << endl;
}

static void saveAction(TokenScanner& scanner, SSModel& model) {
	if (!scanner.hasMoreTokens()) 
        error("The save command requires a file name.");
    
	string filename;
	while (scanner.hasMoreTokens())
        filename += scanner.nextToken();
	ofstream out(filename.c_str());
	if (out.fail()) 
        error("Cannot open the file named \"" + filename + "\".");
	model.writeToStream(out);
	cout << "Saved file \"" << filename << "\"." << endl;
}

static void setAction(TokenScanner& scanner, SSModel& model) {
	if (!scanner.hasMoreTokens()) 
        error("The set command requires a cell name and a value.");
	string cellname = scanner.nextToken();
	if (!model.nameIsValid(cellname)) 
        error("Invalid cell name " + cellname);
	if (scanner.nextToken() != "=") 
        error("= expected.");
	model.setCellFromScanner(cellname, scanner);
}

static void getAction(TokenScanner& scanner, SSModel& model) {
	if (!scanner.hasMoreTokens()) 
        error("The get command requires a cell name.");
	string cellname = scanner.nextToken();
	if (!model.nameIsValid(cellname)) error("Invalid cell name " + cellname);
	model.printCellInformation(cellname);
}

static const double kHaltTimeBeforeWindowClose = 3000;
static void quitAction(TokenScanner& /* scanner */, SSModel& /* model */) {
    cout << "Thanks for using Stanford 1-2-3.  " 
         << "Closing window in " << kHaltTimeBeforeWindowClose/1000 << " "
         << "seconds...." << endl;
    pause(kHaltTimeBeforeWindowClose);
	exitGraphics();
}

/**
 * Function: setUpCommandTable
 * ---------------------------
 * Adds all commands into the table.
 */

static void setUpCommandTable(Map<string, cmdFnT>& table) {
    // store map entries using lowercase, always use lowercase to lookup
    table["help"] = helpAction;
    table["load"] = loadAction;
    table["save"] = saveAction;
    table["set"] = setAction;
    table["get"] = getAction;
    table["quit"] = quitAction;
}

/*********************************************************************************
 * DO NOT EDIT FUNCTIONS BELOW THIS COMMENT. WE WILL BE USING THEM FOR GRADING   *
 *********************************************************************************/

/**
 * Function: interpretCommands
 * ---------------------------
 * The loop in this function gets a command entered by the user and uses a
 * command dispatch table to determine how to act on the request. One of
 * the more unusual aspects is its use of a try-block around the attempt
 * to handle the command.  Many things can go wrong in processing the command
 * (malformed arguments, parsing problem, unable to open a file, etc) and the
 * underlying functions call error() to report those problems.  Usually this
 * would quit the entire program, which in this case is not desirable.
 * Instead, the try-block allows the call to error() to be "caught" and
 * control jumps to the exception handling statement here, which prints
 * the message and goes back into the loop to allow the user to try again.
 */

static void interpretCommands(Map<string, cmdFnT>& cmdTable) {
	SSView view;
	SSModel model(kNumRowsDisplayed, kNumColsDisplayed, &view);
	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.scanNumbers();
	scanner.scanStrings();
	
	cout << "Welcome to Stanford 1-2-3.  Type \"help\" for list of commands." 
         << endl << endl;
	while (true) {
		string command = getLine("Enter command: ");
		scanner.setInput(command);
        string cmdName = toLowerCase(trim(scanner.nextToken()));
		if (!cmdTable.containsKey(cmdName)) {
			cout << "Unrecognized command \"" + cmdName 
            << "\". Type \"help\" for list of commands." << endl;
		} else {
			try {
				cmdTable[cmdName](scanner, model);
			} catch (ErrorException ex) {
				cout << "Error in " << cmdName << " command: " 
                << ex.getMessage() << endl;
			} 
		}
	}
}

int main() {
	Map<string, cmdFnT> cmdTable;
	setUpCommandTable(cmdTable);
	interpretCommands(cmdTable);
	return 0;
}






