/*
 * \file binaryTree.h
 *
 * Implements a binary tree. Useful for set implementations
 *
 * Binary Search Trees (BST) allows you to search, insert and delete in worst case O(n). However,
 * averagely, they allow you to search, insert and delete in \f$ O( n log_2 n ) \f$ time.
 *
 * Some references:
 *
 * \li <a href="http://web.cs.wpi.edu/~cs2102/common/notes-d13/bsts-and-avls.html">link1</a>;
 * \li <a href="http://www.algolist.net/Data_structures/Binary_search_tree/Removal">link2</a>;
 *
 *  Created on: Jun 6, 2017
 *      Author: koldar
 */

#ifndef BINARYTREE_H_
#define BINARYTREE_H_

#include <stdlib.h>
#include <stdbool.h>
#include "typedefs.h"
#include "stringBuilder.h"
#include "dynamic_array.h"

/**
 * Represents the entry point for the whole binary tree
 */
typedef struct binary_tree binary_tree;

typedef struct binary_node binary_node;

binary_tree* initBinaryTree(orderer o, destructor d);

void destroyBinaryTree(binary_tree* binaryTree);

void destroyBinaryTreeWithElements(binary_tree* binaryTree);

/**
 * Adds an item inside the given binary tree
 *
 * This operation is \f$ O( n log_2 n) \f$ long
 *
 * @param[in] binaerytree the tree involved
 * @param[in] payload the data you want to add inside the tree
 * @return
 * 	\li true if the data has been added inside the tree;
 * 	\li false otherwise;
 */
bool addItemInBinaryTree(binary_tree* binarytree, void* payload);

/**
 * Check the membership of a data inside the binary tree
 *
 * this operation takes up to \f$ O(n log_2 n) \f$
 *
 * @param[in] binaryTree the tree involved
 * @param[in] paylaod the data you're looking for
 * @return
 * 	\li true if the payload is inside the tree;
 * 	\li false otherwise;
 */
bool containsItemInBinaryTree(const binary_tree* binaryTree, void* payload);

/**
 * Find an element satisfying the given criterion in the BST
 *
 * @param[in] binaryTree the tree to analyze
 * @param[in] f the criterion to use
 * @return
 * 	\li a non NULL value representing the first value found inside the tree satisfying the criterion
 * 	\li NULL if not elements within the tree satisfied the criterion;
 */
void* findItemInBinaryTree(const binary_tree* binaryTree, finder f);

/**
 * @param[in] binaryTree the tree involved
 * @return the number of elements inside this binary tree
 */
int getSizeOfBinaryTree(const binary_tree* binaryTree);

/**
 * Removes an item from the binary search tree
 *
 * @param[in] binaryTree the tree involved
 * @param[in] payload the payload we want to look for
 * @return
 * 	\li true if the elements was found and removed;
 * 	\li false otherwise;
 */
bool removeItemInBinaryTree(binary_tree* binaryTree, void* payload);

/**
 * clear every elements inside the binary search tree (without releasing the memory)
 *
 * @param[in] binarytree the tree to clear
 */
void clearBinaryTree(binary_tree* binarytree);

binary_node* _getRootOfBinaryTree(const binary_tree* binaryTree);

binary_node* _getLeftOfBinaryNode(const binary_node* binaryNode);

binary_node* _getRightOfBinaryNode(const binary_node* binaryNode);

void* _getPayloadOfBinaryNode(const binary_node* binaryNode);

#define ITERATE_THROUGH_BST(_bst, _payload, type) \
	binary_tree* UV(bst) = (_bst); \
	binary_node* UV(tmp) = _getRootOfBinaryTree(UV(bst)); \
	binary_node* UV(tmp2) = NULL; \
	bool UV(outerLoop) = getSizeOfBinaryTree(UV(bst)) != 0; \
	int UV(visitedIndex) = 0; \
	dynamic_1D_array* UV(visited) = NULL; \
	if (UV(outerLoop)) \
		for ( \
			type _payload = NULL \
			; \
			UV(outerLoop) ? (UV(visited) = initDynamicArray(binary_node*, getSizeOfBinaryTree(UV(bst))), true) : (destroyDynamicArray(UV(visited)), false) \
			; \
			UV(outerLoop) = false \
		) \
			for ( \
					setDynamicArrayCellToVariableValue(UV(visited), UV(visitedIndex)++, binary_node*, UV(tmp)) \
					; \
					(_payload =  UV(tmp) != NULL ? _getPayloadOfBinaryNode(UV(tmp)) : NULL, UV(tmp) != NULL) \
					; \
					UV(visitedIndex)--, \
					UV(tmp2) = _getLeftOfBinaryNode(UV(tmp)), \
					setDynamicArrayCellToVariableValue(UV(visited), UV(visitedIndex), binary_node*, UV(tmp2)), \
					UV(visitedIndex) += getDynamicArrayCellValue(UV(visited), UV(visitedIndex), binary_node*) != NULL ? 1 : 0, \
					UV(tmp2) = _getRightOfBinaryNode(UV(tmp)), \
					setDynamicArrayCellToVariableValue(UV(visited), UV(visitedIndex), binary_node*, UV(tmp2)), \
					UV(visitedIndex) += getDynamicArrayCellValue(UV(visited), UV(visitedIndex), binary_node*) != NULL ? 1 : 0, \
					UV(tmp) = UV(visitedIndex) > 0 ? getDynamicArrayCellValue(UV(visited), UV(visitedIndex) - 1, binary_node*) : NULL \
				) \

#endif /* BINARYTREE_H_ */
