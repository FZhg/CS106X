/**
 * File: ssmodel.h
 * ---------------
 * This file defines the interface for SSModel class used in the
 * Stanford 1-2-3 spreadsheet program.
 */
#pragma once
#include <fstream>
#include <string>
#include "tokenscanner.h"
#include "ssview.h"
#include "cell.h"
#include "vector.h"
#include "hashmap.h"

/**
 * This is a "forward reference" which informs the compiler there is a
 * class named Expression that we'll eventually get around to seeing the
 * full class interface for, but for now, we just need to know such a class
 * exists.
 */

class Expression;

/**

  //TO-DO: user input tolerance, impelement a buffer, if not mistake then make changes
  //       to current model.
* Class: SSModel
 * --------------
 * This is the starting interface for the spreadsheet data model class.  The 
 * public member functions listed here are used by other parts of the given code 
 * (i.e. controller/view) and thus need to be retained as is and implemented correctly. 
 * However the rest of the class (both public & private) is yours to design.
 */

class SSModel {
public:
		
/**
 * Constructor: SSModel
 * Usage: SSModel model(10, 20, vp);
 * --------------------------------
 * The constructor initializes a new empty data model of the given size 
 * which is to be displayed using the given SSView object. Whenever data 
 * in the model changes, this view is notified to re-display the
 * affected cells.
 */
    
    SSModel(int numRows, int numCols, SSView *ssview);

/**
 * Destructor: ~SSModel
 * Usage: delete mp;
 * -----------------
 * The destructor deallocates any storage for this model.
 */
    
	~SSModel();
		
/**
 * Member function: nameIsValid
 * Usage: if (model.nameIsValid(name))...
 * ------------------------------------------
 * This member function returns true if name is a valid string
 * name of a cell in this model, false otherwise. The string
 * must be in the proper format (column letter followed by row number)
 * and refer to a cell location within bounds for this model.
 */
    bool nameIsValid(const std::string& name) const;
	 
 /**
  * Member function: setCellFromScanner
  * Usage: model.setCellFromScanner("A1", scanner);
  * -----------------------------------------------
  * This member function reads an expression from the scanner and
  * stores it as the the contents for the named cell.  If there is
  * any problem with setting the cell's value (the expression is
  * malformed, contains a circular reference, etc.) error is called
  * and the cell's contents are unchanged.  If the contents were
  * successfully updated, the new cell is displayed in the view
  * and its dependent cells are updated as well.
  */
	
    void setCellFromScanner(std::string& cellname, TokenScanner& scanner);

/**
 * Member function: printCellInformation
 * Usage: model.printCellInformation("A1");
 * ----------------------------------------
 * This member function prints the cell information for the named
 * cell to cout.  Cell information includes its current contents
 * and dependencies (and other information you choose to include).
 */
    
    void printCellInformation(const std::string& cellname) const;

/**
 * Member functions: writeToStream, readFromStream
 * Usage: model.writeToStream(outfile);
 *        model.readFromStream(infile);
 * --------------------------------
 * These member functions read/write model contents
 * to/from a stream.  The stream is assumed to be valid and open.
 * The file format consists of cell values, one per line, like this:
 *
 *      A1 = 3
 *      A2 = 4 * (A1 + 8)
 *      A3 = "a string"
 *
 * error is called if there is any trouble reading/writing
 * the file.
 */

    void writeToStream(std::ostream &outfile) const;

    /**
     * Method: clear
     * Usage: model.clear();
     * ---------------------
     * Clear the current model.
     */
    void clear();

    /**
     * Method: isEmptyCell
     * Usage: model.isEmptyCell(cellname);
     * -----------------------------------
     * Return true if the cell is empty.
     */
    bool isNonEmptyCell(std::string& cellname) const;

    /**
     * Method: getValue
     * Usage: model.getValue(cellname);
     * ---------------------------------
     * Return the value of the Cell responding to the cellname;
     */
    double getNumValue(const std::string& cellname) const;

    /**
     * Method setNumValue
     * Usage: model.setNumValue(cellname, newNumData);
     * -----------------------------------------------
     * Preconditions:
     * only called by clients when the cell  is to accept a
     * numeric content;
     * cellname must be valid and the cell must not be empty.
     *
     */
    void setNumValue(std::string& cellname, double newNumData);

    /**
     * Method: getStrData(std:;string& cellname)
     * Usage:  getStrData(cellname);
     * ----------------------------
     * Return the string content of the cell.
     * Usually used by the SSview part.
     */
    std::string getStrData(std::string& cellname) const;

    /**
     * Method: setStrData
     * Usage: Model.setStrData(cellname, newStrData);
     * ----------------------------------------------
     * Preconditions:
     * Only Called by the clients when the cell is to accept
     *  a string content;
     * Cellname must be valid and the cell must not be empty.
     */
    void setStrData(std::string& cellname, std::string& newStrData);

    /**
     * Method： addCommand
     * Usage： model.addCommand(newCommand);
     * ------------------------------------
     * Add the current command to the model for possible output.
     */
    void addCommand(std::string newCommand);

    /**
     * Method: rmLastCommand()
     * Usagae: modle.rmLastCommand();
     * --------------------------------
     * Used by the controler to rm the incorrect command.
     */
    void rmLastCommand();

    /**
     * Method: getCell
     * Usage: Cell* cell = model.getCell(cellname);
     * ---------------------------------------------
     * Preconditions： cellname must be valid
     * return the cell pointer
     * if the cell is empty creat empty cell pointer.
     */
    Cell* getCell(const std::string& cellname);

    /**
     * Method: rmCell
     * Usage: model.rmCell(cellname);
     * ------------------------------
     * rm the cell and delete the heap memory.
     */
    void rmCell(std::string& cellname);

    /**
     * Method: show
     * Usage: model.show()
     * ----------------------
     * Show the nonEmptyCells to the current SSview Object.
     */
    void show();

	
private:
    int numRows;
    int numCols;
    HashMap<std::string, Cell*> nonEmptyCells;
    Vector<std::string> commands;
    SSView* view;
};

