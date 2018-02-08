/*
 * \file redBlackTree.h
 *
 * The module allows you to use red black trees
 *
 * For further information about the red black trees see the following references:
 *
 * \li Cormen, introduction to algorithms, 3rd edition, Chapter 13;
 *
 * <h3>Why use them?</h3>
 *
 * Red black trees are an enhanced vesion of BST: they allow to maintain a balanced tree even with lots of insertion and removal:
 * this lead to a cost of insertion/search/removal of just \f$ O(n log_2 n) \f$ averagely. Viceversa, BSTs suffer from potential tree
 * unbalacing: when a BST has lots of insertion and removal, it may become a list, thereby creating a cost of insertion/lookup/deletion of \f$ O(n)\f$.
 *
 * **So use red black trees when your tree has lots of insertion/deletion**
 *
 * <h3>Implementation details</h3>
 * The only thing to remember is that while Cormen says that there exist a NIL sentinel that is black, here we implicitly say that the NULL value
 * is by default black.
 *
 *  Created on: Jun 9, 2017
 *      Author: koldar
 */

#ifndef REDBLACKTREE_H_
#define REDBLACKTREE_H_

#include <stdbool.h>
#include "typedefs.h"


typedef struct rb_tree rb_tree;

/**
 * Creates a new red black tree in the memory
 *
 * @param[in] d the function to call when you want to remove from the memory an item in the tree
 * @param[in] o a function used to compare different items inside the tree
 * @return the instance of the red black tree
 */
rb_tree* initRedBlackTree(destructor d, orderer o);

/**
 * Destroy the tree
 *
 * \note
 * All the items in the tree **won't be released** from the memory at all!
 * If you want to release them as well, use ::destroyRedBlackTreeWithElements
 *
 * @param[in] tree the tree to free
 */
void destroyRedBlackTree(rb_tree* tree);

/**
 * like ::destroyRedBlackTree except that all items in the tree will be release from the memory as well
 *
 * @param[in] tree the tree to free
 */
void destroyRedBlackTreeWithElements(rb_tree* tree);

/**
 * Adds an item inside the tree
 *
 * @param[inout] tree the tree to alter
 * @param[in] paylaod the item to add in the tree
 */
bool addItemInRedBlackTree(rb_tree* tree, void* payload);

/**
 * @param[in] tree the tree to analyze
 * @return the number of items inside the tree
 */
int getSizeOfRedBlackTree(const rb_tree* tree);

/**
 * @param[in] tree the tree to analyze
 * @return
 *  \li true if the tree has no elements inside it;
 *  \li false otherwise
 */
bool isRedBlackTreeEmpty(const rb_tree* tree);

/**
 * @param[in] tree the tree to analyze
 * @param[in] payload the item to look for
 * @return
 * 	\li true if \c tree has the item \c payload;
 * 	\li false otherwise
 */
bool containsItemInRedBlackTree(const rb_tree* tree, void* payload);

/**
 * @param[in] tree the tree to analyze
 * @return
 * 	\li the minimum item inside the \c tree;
 * 	\li NULL if the \c tree is empty
 */
void* getMinimumInRedBlackTree(const rb_tree* tree);

/**
 * @param[in] tree the tree to analyze
 * @return
 * 	\li the maximum item inside the \c tree;
 * 	\li NULL if the \c tree is empty
 */
void* getMaximumInRedBlackTree(const rb_tree* tree);

/**
 * Removes an element from the \c tree
 *
 * @param[in] tree the tree to alter
 * @param[in] payload the item to look for
 * @return
 * 	\li true if the element has been found and has been removed in the tree;
 * 	\li false if we couldn't find the \c paylaod in the tree
 */
bool removeItemInRedBlackTree(rb_tree* tree, void* payload);

/**
 * like ::removeItemInRedBlackTreeWithElement but we release the \c payload from the memory as well
 *
 * @param[in] tree the tree to alter
 * @param[in] payload the item to look for
 * @return
 * 	\li true if the element has been found and has been removed in the tree;
 * 	\li false if we couldn't find the \c paylaod in the tree
 */
bool removeItemInRedBlackTreeWithElement(rb_tree* tree, void* payload);


#endif /* REDBLACKTREE_H_ */
