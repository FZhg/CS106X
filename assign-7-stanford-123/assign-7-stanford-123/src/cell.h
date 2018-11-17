#ifndef CELL_H
#define CELL_H
#include <string>
#include "ssutil.h"
#include "hashmap.h"

/**
 * Class: cell
 * -------------
 * Each cell has a numeric data field and string data field.
 * When the cell content is number,  both of the fileds will the same meaningly.
 * When the cell content is string, the string data field is the string the content
 * , but the numeric field will be 0;
 */
class Cell
{
public:
    /**
     * Constructor & Destructor: Cell
     * Usage: usually used by subclasses and compilers
     * -----------------------------------------------
     * Preconditions: cellname must be valid and empty;
     */
    Cell(const std::string& cellname);
    ~Cell();

    /**
     * Method: getData
     * Usage: double data = cell.getData();
     * ------------------------------------
     * Return 0 if it is a StringCell, the true data if a DoubleCell.
     */
    double getNumData() const;

    /**
     * Method: feedContent
     * Usage: usually used the SSView object to update the Table
     * ----------------------------------------------------------
     * return a string that represent the data
     *
     */
    std::string getStrData() const;

    /**
     * Method: setNumData
     * Usage: model.setNumData(newData);
     * ---------------------------------
     * Set the numData to the expression object evaluating result.
     * Preconditions: only called by other clients when the cell
     *  is to accept numberic content.
     */
    void setNumData(double newData);

    /**
     * Method: setStrData
     * Usage: model.setStrData(newData);
     * ----------------------------------
     * Set the strData field to the TextStringExp objexct evaluating result.
     * Preconditions: only called by the other clients
     * when the cell is to accept string content.
     */
    void setStrData(std::string newData);

    /**
     * Method: addDepended
     * Usage: cell.addDepended(Cell* cell);
     * -------------------------------------
     * Add a cell point to the depended map container.
     */
    void addDepended(Cell* cell);

    /**
     * Method: rmDepended
     * Usage: cell.rmDepended(cell);
     * -----------------------------------
     * Remove a cell point to the depended map container.
     */
    void rmDepended(Cell* cell);

    /**
     * Method: addDepending
     * Usage: cell.addDepending(Cell* cell);
     * -------------------------------------
     * Add a cell point to the depending map container.
     */
    void addDepending(Cell* cell);

    /**
     *Method: rmDepending(Cell* cell);
     * Usage: cell.rmDepending(cell);
     * -------------------------------
     * remove a cell point to the depending map container.
     */
    void rmDepending(Cell* cell);

    /**
     * Method: getCellName
     * Usage string cellName = cell.getCellName()
     * ------------------------------------------
     * return the cellname according to the cell location in the table.
     */
    std::string getCellName() const;

    /**
     * Method； toString
     * Usage: std::cout << cell.toString << std::endl;
     * ------------------------------------------------
     * return a string containing the latest command for this cell,
     * the depending the dependent relations for this cell.
     */
    std::string toString() const;

    /**
     * Method: setCommand
     * Usage： Cell.setCommand(newCommand);
     * ------------------------------------
     * Change the command of the cell to the new command.
     */
    void setCommand(std::string& newCommand);

    /**
     * Method: copyCell
     * Usage: cell.copyCell(secCell);
     * ------------------------------
     * Change the exact everything of the secCell the  the current cell object.
     */
    void copyCell(Cell& secCell);



private:
    location lctn;
    std::string command;
    std::string strData;
    double numData;
    HashMap<std::string, Cell*> depended;
    HashMap<std::string, Cell*> depending;
};
#endif // CELL_H
