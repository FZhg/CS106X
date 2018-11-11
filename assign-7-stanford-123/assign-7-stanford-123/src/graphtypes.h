/**
 * File: graphtypes.h
 * ------------------
 * This file defines low-level data structures that represent graphs.
 * You are free to modify this header file to help accomodate the needs to
 * your Stanford 1-2-3 application.
 */

#pragma once
#include <string>
#include "gtypes.h"
#include "map.h"
#include "set.h"

/**
 * Type: node
 * ----------
 * This type represents an individual node and consists of the
 * name of the node and the set of arcs from this node.  We
 * exclude x and y coordinates from this version, since your
 * graphs won't be visualized on screen as traditional graphs.
 */

struct node {
   std::string name;
   Set<struct arc *> arcs;
};

/**
 * Type: arc
 * ---------
 * This type represents an individual arc and consists of pointers
 * to the endpoints.  This particular version excludes the cost, since
 * the dependency graphs you'll be building won't require it.
 */

struct arc {
   node *start;
   node *finish;
};

/**
 * Type: graph
 * -----------
 * This type represents a graph and consists of a set of nodes, a set of
 * arcs, and a map that creates an association between names and nodes.
 */
struct graph {
   Set<node *> nodes;
   Set<arc *> arcs;
   Map<std::string, node *> index;
};
