/**
 * File: ssmodel.cpp
 * ------------------
 * This file will contain the implementation of the SSModel class (once you
 * have finished it!
 */
 
#include "ssmodel.h"
#include "exp.h"
#include "parser.h"
#include <cctype>
#include "cell.h"
#include <string>
#include "error.h"
using namespace std;

SSModel::SSModel(int nRows, int nCols, SSView *ssview) {
    numRows = nRows;
    numCols = nCols;
    view = ssview;
}

SSModel::~SSModel() {}

/*
 * Implementation Notes: nameIsValid
 * ---------------------------------
 * Row is a upperclass letter
 * Row and col must be in the right range.
 * Preconditions:cellname must be all lowercase
 */
bool SSModel::nameIsValid(const string& cellname) const {
    if(!isalpha(cellname[0])){
        return false;
    } else if((cellname[0] - 'a' + 1) > numCols){
        return false; // check for row range
    }
    string rowIndexString = cellname.substr(1);
    for(int i = 0; i < rowIndexString.size(); i ++){
        if(!isdigit(rowIndexString[i])) return false;
    }
    int rowIndex = stoi(rowIndexString);
    if(rowIndex > numRows - 1) return false; //outlimit our col range
    return true;

}

void SSModel::setCellFromScanner(string& cellname, TokenScanner& scanner) {
    Cell* newCell = new Cell(cellname);
    try{
        Expression* rhs = parseExp(scanner);
        /* the textstring expression */
        if(rhs->getType() == TEXTSTRING){
            newCell->setStrData(rhs->toString());
        } else if(rhs->getType() == IDENTIFIER){
            string name = rhs->toString();
            if(!nameIsValid(name)){
                error("Invalid cell name " + name);
            }
            //add depending for the newCell
            newCell->addDepending(this->getCell(name));
            //add depended for identifier
            getCell(name)->addDepended(newCell);
            newCell->copyCell(*this->getCell(name));
        } else{
            rhs->eval(*this, newCell);
        }
        rmCell(cellname);
        newCell->setCommand(commands[commands.size()-1]);
        nonEmptyCells.add(cellname, newCell);
        show();
    } catch(ErrorException ex){
        cout << ex.getMessage() << endl;
    }
}

void SSModel::printCellInformation(const string& cellname) const {
    if(!nonEmptyCells.containsKey(cellname)){
        error("The cell " + cellname + " is empty in the model.");
    } else{
        cout << nonEmptyCells.get(cellname)->toString() << endl;
    }
}

void SSModel::writeToStream(ostream& outfile) const {
    for(auto const& command : commands){
        outfile << command.substr(2); //get rid of "set"
    }
}

bool SSModel::isNonEmptyCell(string& cellname) const{
    return nonEmptyCells.containsKey(cellname);
}

double SSModel::getNumValue(const string &cellname) const{
    if(nonEmptyCells.containsKey(cellname)){
        return nonEmptyCells.get(cellname)->getNumData();
    } else{
        return 0; // reduduncy! since ident-eval create a empty cell.
    }
}

void SSModel::setNumValue(string &cellname, double newNumData){
    nonEmptyCells.get(cellname)->setNumData(newNumData);
}

string SSModel::getStrData(string &cellname) const{
    return nonEmptyCells.get(cellname)->getStrData();
}

void SSModel::setStrData(string &cellname, string &newStrData){
    nonEmptyCells.get(cellname)->setStrData(newStrData);
}

void SSModel::addCommand(string newCommand){
    commands.add(newCommand);
}

void SSModel::rmLastCommand(){
    commands.remove(commands.size() - 1);
}

Cell* SSModel::getCell(const std::string& cellname){
    if(nonEmptyCells.containsKey(cellname)){
        return nonEmptyCells.get(cellname);
    } else{
        Cell* newCell = new Cell(cellname);
        nonEmptyCells.put(cellname, newCell);
        return newCell;
    }
}

void SSModel::rmCell(string &cellname){
    if(nonEmptyCells.containsKey(cellname)){
        nonEmptyCells.get(cellname)->~Cell();
        nonEmptyCells.remove(cellname);
    }
}
void SSModel::clear(){
    for(auto const& cell : nonEmptyCells.values()){
        cell->~Cell();
    }
    nonEmptyCells.clear();
    commands.clear();
    view->displayEmptySpreadsheet();
}

void SSModel::show(){
    for(const auto cell:nonEmptyCells.values()){
        view->displayCell(cell);
    }
}
