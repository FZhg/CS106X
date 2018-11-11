/**
 * File: ssutil.h
 * --------------
 * This file defines a set of small utilities for the Stanford 1-2-3 program.
 * You are free to use, modify, or ignore this code as you see fit.
 */
 
#pragma once
#include "vector.h"

/**
 * Type: location
 * --------------
 * Location structure to identify a cell by its col (a char)
 * and row (an int).
 */

struct location {
	char col;
	int row;
} ;
	
/**
 * Type: range
 * -----------
 * This struct can be used to identify a range. It represents start/stop
 * cells as locations. An alternative would be to store string 
 * cellnames for the start and stop cell, and if you'd rather, feel free
 * to change the definition to better fit your needs.
 */

struct range {
	location startCell, stopCell;
} ;
		
/**
 * Function: stringToLocation
 * Usage: if (stringToLocation("A10", loc))....
 * --------------------------------------------
 * This function converts a cell name into a location passed
 * by reference.  Name is expected to be in format of column letter
 * (either lower or uppercase), followed by row number. 
 * Returns true if found char followed by number and loc was assigned, 
 * otherwise returns false and contents of loc are unchanged.
 */

bool stringToLocation(const std::string& name, location& loc);

/**
 * Function: locationToString
 * Usage: name = locationToString(loc);
 * ------------------------------------
 * Helper function to convert a location to string form
 * consisting of a column letter followed by row number, e.g. "A7"
 */

std::string locationToString(const location& loc);

/**
 * Functions: average, sum, product, max, min, median, stdev
 * Usage: avg =  average(values);
 * ------------------------------
 * Implementation of built-in functions supported in cell formulas. Each
 * takes a vector of double values and returns the computed result.
 */

double average(const Vector<double> & values);
double sum(const Vector<double> & values);
double product(const Vector<double> & values);
double max(const Vector<double> & values);
double min(const Vector<double> & values);
double median(const Vector<double> & values);
double stdev(const Vector<double> & values);

