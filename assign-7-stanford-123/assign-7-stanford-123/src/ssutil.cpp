/*
 * File: util.cpp
 * --------------
 * The util.cpp files provides a few utility functions for use in Stanford 1-2-3.
 * You are free to use these as is, discard them, or modify them as you like.
 */

#include "ssutil.h"
#include <cctype>
#include <cmath>
#include <sstream>
#include <algorithm>
using namespace std;

bool stringToLocation(const string& name, location& loc) {
	stringstream sstr;
	sstr << name;	// insert name into stream, then parse it back out
	char ch;
	int num;
	sstr >> ch >> num;	// expect char followed by integer
	if (!isalpha(ch) || sstr.fail() || !sstr.eof()) return false;
	loc.col = toupper(ch);
	loc.row = num;
	return true;
}

string locationToString(const location& loc) {
	return string(1, toupper(loc.col)) + integerToString(loc.row);
}

double min(const Vector<double>& values) {
	double min = values[0];
	for (int i = 1; i < values.size(); i++) 
		if (values[i] < min) 
            min = values[i];
	return min;
}

double max(const Vector<double>& values) {
	double max = values[0];
	for (int i = 1; i < values.size(); i++) 
		if (values[i] > max) 
            max = values[i];
	return max;
}

double sum(const Vector<double>& values) {
	double sum = 0;
	for (int i = 0; i < values.size(); i++) 
        sum += values[i];
	return sum;
}

double product(const Vector<double>& values) {
	double prod = 1;
	for (int i = 0; i < values.size(); i++) 
		prod *= values[i];
	return prod;
}

/* This function should be accessible by both name "mean" and "average" */
double average(const Vector<double>& values) {
	return sum(values)/values.size();
}

double median(const Vector<double>& values) {
    Vector<double> clone = values;
	sort(clone.begin(), clone.end());
	int n = clone.size();
	if (n % 2 == 0) 
		return (clone[n/2] + clone[n/2 - 1])/2;
	else 
		return clone[n/2];
}

double stdev(const Vector<double>& values) {
	double sum = 0, sumsquares = 0;
	for (int i = 0; i < values.size(); i++) {
		sum += values[i];
		sumsquares += values[i] * values[i];
	}
	return sqrt((values.size() * sumsquares - sum*sum)/(values.size() * values.size()));
}

