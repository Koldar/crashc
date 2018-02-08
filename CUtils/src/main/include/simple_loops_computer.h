/*
 * simple_loops_computer.h
 *
 *  Created on: Jul 25, 2017
 *      Author: koldar
 */

#ifndef SIMPLE_LOOPS_COMPUTER_H_
#define SIMPLE_LOOPS_COMPUTER_H_

#include "predsuccgraph.h"
#include "scc.h"

/**
 * a list of nodes ::Node
 */
typedef list node_list;
/**
 * a loop is a node list
 */
typedef node_list loop;
/**
 * a loop list is a list containing node_lists as elements
 */
typedef list loop_list;

typedef struct johnson_support johnson_support;


johnson_support* initJohnsonSupport();

void destroyJohnsonSupport(johnson_support* support);

loop_list* computeSimpleLoopsInGraph(johnson_support* support, const PredSuccGraph* g, edge_traverser t);

#endif /* SIMPLE_LOOPS_COMPUTER_H_ */
