/**
 * File: maze-types.h
 * ------------------
 * Provides the definitions of the cell and wall types used in the 
 * code base.  They're defined here in a separate file so that they
 * can shared by the system.
 */

#ifndef _maze_types_
#define _maze_types_

/**
 * Defines a positive integer coordinate in 2D space, where we
 * (arbitrarily) impose the requirement that (0, 0)
 * is above and/or to the left of all others.
 */
struct cell {
	int row;
	int col;
};

inline bool operator<(const cell& one, const cell& two) {
    return one.row < two.row || (one.row == two.row && one.col < two.col);
}
    
/**
 * A wall within a maze is described by the two cells
 * it separates.  It's the responsibility of the programmer
 * to ensure that the two cells are really next to one another,
 * and that the two cells are sensibly ordered.
 */
struct wall {
	cell one;
	cell two;
};

inline bool operator<(const wall& one, const wall& two) {
    if (one.one < two.one) return true;
    if (two.one < one.one) return false;
    return one.two < two.two;
} // yes, this is somewhat arbitrary, but it can be arbitrary as long as it's consistent
    
#endif
