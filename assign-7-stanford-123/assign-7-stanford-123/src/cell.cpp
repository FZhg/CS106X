#include "cell.h"
#include <string>
using namespace std;

Cell::Cell(const string& cellname){

    lctn.col = cellname[0];
    lctn.row = stoi(cellname.substr(1));
    strData = "";
    numData = 0;
}

Cell::~Cell(){}

void Cell::addDepended(Cell* cell){
    depended.put(cell->getCellName(), cell);
}

void Cell::rmDepended(Cell* cell){
    depended.remove(cell->getCellName());
}

void Cell::addDepending(Cell *cell){
    depending.put(cell->getCellName(), cell);
}

void Cell::rmDepending(Cell *cell){
    depending.remove(cell->getCellName());
}

string Cell::toString() const{
    string rslt;
    rslt += command;
    rslt += "\n Cells that " + getCellName() + " directly depends on:"
            + depending.keys().toString() + '\n';
    rslt += "Cells that directly depend on " + getCellName() +":" +
            depended.keys().toString() + '\n';
    return rslt;
}

string Cell::getCellName() const{
   return  lctn.col + integerToString(lctn.row);
}

double Cell::getNumData() const{
    return  numData;
}

string Cell::getStrData() const{
    return strData;
}

void Cell::setNumData(double newData){
    numData=newData;
    strData = doubleToString(numData);
}

void Cell::setStrData(string newData){
    numData = 0;
    strData = newData;
}

void Cell::setCommand(string& newCommand){
    command = newCommand;
}

void Cell::copyCell(Cell& secCell){
    numData = secCell.numData;
    strData = secCell.strData;
}
