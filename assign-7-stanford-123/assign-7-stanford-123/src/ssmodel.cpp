/**
 * File: ssmodel.cpp
 * ------------------
 * This file will contain the implementation of the SSModel class (once you
 * have finished it!
 */
 
#include "ssmodel.h"
#include "exp.h"
using namespace std;

SSModel::SSModel(int nRows, int nCols, SSView *view) {}

SSModel::~SSModel() {}

bool SSModel::nameIsValid(const string& cellname) const {
	return false;
}

void SSModel::setCellFromScanner(const string& cellname, TokenScanner& scanner) {}

void SSModel::printCellInformation(const string& cellname) const {}

void SSModel::writeToStream(ostream& outfile) const {}

void SSModel::readFromStream(istream& infile) {}
