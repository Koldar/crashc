/*
 * \file topologicalOrder.h
 *
 * Represents a module containing function implementing the topological order
 *
 * For further information, see <a href="https://en.wikipedia.org/wiki/Topological_sorting">topological order</a>
 *
 *  Created on: May 31, 2017
 *      Author: koldar
 */

#ifndef TOPOLOGICALORDER_H_
#define TOPOLOGICALORDER_H_

#include "predsuccgraph.h"
#include "node.h"
#include "list.h"

/**
 * List all the possible implementation this module can offer of topological sorts
 */
typedef enum {
	///**static** algorithm described <a href="https://en.wikipedia.org/wiki/Topological_sorting#Depth-first_search">here</a>
	TO_CORMEN
} to_impl;

/**
 * Compute the static topological order of a DAG
 *
 * \note
 * The behaviour of this function is undefined is \c graph is not a DAG!
 *
 * @param[in] implementation a number representing the implementation you want to use to compute the static topological order
 */
void doStaticTopologicalOrderWith(to_impl implementation, PredSuccGraph* graph, NodeList* output);



#endif /* TOPOLOGICALORDER_H_ */
