/*
 * scc.h
 *
 *  Created on: May 25, 2017
 *      Author: koldar
 */

#ifndef SCC_H_
#define SCC_H_

#include "predsuccgraph.h"
#include "hashtable.h"
#include "heap.h"

/**
 * Represents the possible return value of the function ::edge_traverser
 */
typedef enum {
	/**
	 * Indicates that the edge analyzed has to be ignored DFS-wise.
	 * This indicates that the DFS used by tarjan **won't** follow this edge by traversing it
	 */
	ET_TOIGNORE,
	/**
	 * Indicates that the edge analyzed has to be followed DFS-wise
	 *
	 * This is exactly opposite as ::ET_TOIGNORE
	 */
	ET_TOANALYZE,
	/**
	 * Indicates that we have to completely stop tarjan algorithm.
	 *
	 * Be sure to know what you're doing because this **potentally** put the scc status to an inconsistent state
	 */
	ET_STOP
} et_outcome;


/**
 * Represents a function telling you whether an edge encoutered by Tarjan
 * can be traversed during the DFS or not
 */
typedef et_outcome (*edge_traverser)(Edge*);

typedef Node scc;
typedef heap node_heap;
typedef struct scc_graph scc_graph;

/**
 * Creates a graph containing the sccs of the given \c graph
 *
 * @param[in] graph the graph we want to compute the sccs from
 * @param[in] traverser a function telling us if we need to traverse an edge or not
 * @param[in] trackInterSCCEdfges if true, we will create a ::scc_graph containing edges between 2 different sccs as well. Otherwise we will create only the sccs
 * @param[in] included contains an hashtable indexed by the nodes ids. If a node id is inside the hastable, the node will be considered during tarjan algorithm, otherwise the software will
 * 				ignoring it_ in this way you can compute SCCs on a subgraph of the given \c graph. The parameter can be NULL: if so, the whole graph will be analyzed
 * @return an instance of the ::scc_graph
 */
scc_graph* initStronglyConnectedComponentsGraph(const PredSuccGraph* graph, edge_traverser traverser, bool trackInterSCCEdges, const bool_ht* included);

void destroyStronglyConnectedComponentsGraph(const scc_graph* sccGraph);

/**
 * A possible ::edge_traverser implementation accepting every single edge
 *
 */
et_outcome edge_traverser_alwaysAccept(Edge* e);

PredSuccGraph* getStrongConnectedComponentGraph(const scc_graph* sccGraph);

/**
 * @param[in] scc the scc involved
 * @return the number of nodes inside a given \c scc
 */
int getNumberOfNodesInsideStrongConnectedComponent(const scc* s);

/**
 * Check whether or not a node inside the original graph with id \c nid is stored in the scc indexed with \c sccId
 *
 * @param[in] sccGraph the scc graph involved
 * @param[in] nid the id of the node inside the original graph
 * @param[in] sccId the id of the scc we want to test
 * @return
 * 	\li true if \c nid is inside the scc \c sccId;
 * 	\li false otherwise;
 */
bool isNodeInsideStrongConnectedComponent(const scc_graph* sccGraph, Node* n, scc* s);

/**
 * like ::isNodeInsideSCC but accepts ids of the nodes and scc involved
 */
bool isNodeInsideStrongConnectedComponentById(const scc_graph* sccGraph, NodeId nid, NodeId sccId);

scc* getStrongConnectedComponentOfNode(const scc_graph* sccGraph, Node* n);

scc* getStrongConnectedComponentOfNodeById(const scc_graph* sccGraph, NodeId id);

scc* getStrongConnectedComponentOfEdgeById(const scc_graph* sccGraph, NodeId source, NodeId sink);

scc* getStrongConnectedComponentOfEdge(const scc_graph* sccGraph, Edge* e);

/**
 * @param[in] s scc to analyze
 * @return the node whose id is the minimum inside \c s
 */
Node* getNodeWithMinimumIdInStrongConnectedComponent(const scc* s);

/**
 *
 * \note
 * Do not alter the output heao, otherwise unknown bhevaiours may happen
 *
 *
 * @param[in] s the scc to analyze
 * @return an heap containing all the nodes inside a given scc
 */
node_heap* getNodesInsideStrongConnectedComponent(const scc* s);

#endif /* SCC_H_ */
