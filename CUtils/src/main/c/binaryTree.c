/*
 * binaryTree.c
 *
 *  Created on: Jun 6, 2017
 *      Author: koldar
 */

#include <stdlib.h>
#include <string.h>
#include "binaryTree.h"
#include "log.h"
#include "macros.h"

typedef struct binary_node {
	/**
	 * pointer to the parent binary_node.
	 * BST root has this field set to NULL
	 */
	struct binary_node* parent;
	/**
	 * pointer to the left child of the node.
	 * Left child sub tree contains all the nodes whose payload is less than the current one
	 */
	struct binary_node* left;
	/**
	 * pointer to the left child of the node.
	 * Right child sub tree contains all the nodes whose payload is greater than the current one
	 */
	struct binary_node* right;
	/**
	 * the payload of the node.
	 * Can be whatever you want
	 */
	void* payload;
} binary_node;

typedef struct binary_tree {
	/**
	 * points to the root of the binary tree. Can be NULL
	 */
	struct binary_node* root;
	/**
	 * The function used to check if a node payload is bigger or smaller than another node's one
	 */
	orderer o;
	/**
	 * Function used to destroy a node payload
	 */
	destructor d;
	/**
	 * number of elements inside this tree
	 */
	int size;
};

static binary_node* initBinaryNode(binary_node* parent, binary_node* left, binary_node* right, void* payload);
static bool addItemInBinaryNode(binary_node* binaryNode, void* payload, binary_tree* tree);
static bool containsItemInBinaryNode(const binary_node* node, void* payload, orderer o);
static binary_node* findMinInBinaryNode(const binary_node* node, orderer o, binary_node* minTmp);
static bool removeItemInBinaryNode(binary_node* node, binary_node** parentBranch, void* payload, orderer o);
static void destroyBinaryNode(binary_node* binaryNode, bool withElements, destructor d);
static void* findItemInBinaryNode(const binary_node* n, finder f);

binary_tree* initBinaryTree(orderer o, destructor d) {
	binary_tree* retVal = malloc(sizeof(binary_tree));
	if (retVal == NULL) {
		ERROR_MALLOC();
	}

	retVal->size = 0;
	retVal->root = NULL;
	retVal->o = o;
	retVal->d = d;
	return retVal;
}


void destroyBinaryTree(binary_tree* binaryTree) {
	if (binaryTree->root != NULL) {
		destroyBinaryNode(binaryTree->root, false, binaryTree->d);
	}
	free(binaryTree);
}

void destroyBinaryTreeWithElements(binary_tree* binaryTree) {
	if (binaryTree->root != NULL) {
		destroyBinaryNode(binaryTree->root, true, binaryTree->d);
	}
	free(binaryTree);
}

bool addItemInBinaryTree(binary_tree* binarytree, void* payload) {
	if (binarytree->size == 0) {
		binarytree->root = initBinaryNode(NULL, NULL, NULL, payload);
		binarytree->size += 1;
		return true;
	}

	bool retVal = addItemInBinaryNode(binarytree->root, payload, binarytree);
	if (retVal) {
		binarytree->size += 1;
	}
	return retVal;
}

bool containsItemInBinaryTree(const binary_tree* binaryTree, void* payload) {
	return containsItemInBinaryNode(binaryTree->root, payload, binaryTree->o);
}

void* findItemInBinaryTree(const binary_tree* binaryTree, finder f) {
	return findItemInBinaryNode(binaryTree->root, f);
}

int getSizeOfBinaryTree(const binary_tree* binaryTree) {
	return binaryTree->size;
}

bool removeItemInBinaryTree(binary_tree* binaryTree, void* payload) {
	bool retVal = removeItemInBinaryNode(binaryTree->root, &binaryTree->root, payload, binaryTree->o);
	if (retVal) {
		binaryTree->size--;
	}
	return retVal;

}

void clearBinaryTree(binary_tree* binarytree) {
	destroyBinaryNode(binarytree->root, false, NULL);
	binarytree->size = 0;
	binarytree->root = NULL;
}

binary_node* _getRootOfBinaryTree(const binary_tree* binaryTree) {
	return binaryTree->root;
}

binary_node* _getLeftOfBinaryNode(const binary_node* binaryNode) {
	return binaryNode->left;
}

binary_node* _getRightOfBinaryNode(const binary_node* binaryNode) {
	return binaryNode->right;
}

void* _getPayloadOfBinaryNode(const binary_node* binaryNode) {
	return binaryNode->payload;
}

binary_node* _getParentOfBinaryNode(const binary_node* binaryNode) {
	return binaryNode->parent;
}

static void* findItemInBinaryNode(const binary_node* n, finder f) {
	if (n == NULL) {
		return NULL;
	}

	if (f(n->payload) == true) {
		return n->payload;
	}

	void* retVal;

	retVal = findItemInBinaryNode(n->left, f);
	if (retVal != NULL) {
		return retVal;
	}
	retVal = findItemInBinaryNode(n->right, f);
	if (retVal != NULL) {
		return retVal;
	}

	return NULL;
}

static bool removeItemInBinaryNode(binary_node* node, binary_node** parentBranch, void* payload, orderer o) {
	if (node == NULL) {
		return false;
	}

	register int r = o(payload, node->payload);
	if (r < 0) {
		return removeItemInBinaryNode(node->left, &node->left, payload, o);
	}
	if (r > 0) {
		return removeItemInBinaryNode(node->right, &node->right, payload, o);
	}

	//we have found the element

	if ((node->left == NULL) && (node->right == NULL)) {
		//no children: we can remove this node without any hassle
		*parentBranch = NULL;
		free(node);
		return true;
	}
	if (node->left == NULL) {
		//the node has only the right child
		*parentBranch = node->right;
		free(node);
		return true;
	}
	if (node->right == NULL) {
		//the node has only the left child
		*parentBranch = node->left;
		free(node);
		return true;
	}
	//the node has both children

	//find the minimum value just after the one inside "node"
	binary_node* minNode = findMinInBinaryNode(node->right, o, node->right);
	info("min node under %p is %p", node->right->payload, minNode->payload);
	//we remove from the tree the min node
	if (minNode == node->right) {
		node->right = NULL;
	} else {
		minNode->parent->left = NULL;
	}
	//we change the payload
	node->payload = minNode->payload;
	//we need to remove from memory the minNode binary_node
	free(minNode);
	return true;
}

/**
 * find the node with the minimum value inside the binary sub tree of \c node
 *
 * @param[in] node the node generating the subtree we want to look into
 * @param[in] o a function used to ocmpare node payloads
 * @param[in] minTmp the minimum we ahve found up until now
 * @param[in] minTmpParentNodeBranch a pointer to the branch we have followed (from the parent) to reach the minimum \c minTmp
 * @param[out] minNodeParentBranch reference to a place where we can store the branch we have followed from the parent to reach the minimum we have found. Can be NULL
 * @return
 * 	the node containing the minimum value inside subtree generated by the node \c node
 */
static binary_node* findMinInBinaryNode(const binary_node* node, orderer o, binary_node* minTmp) {
	if (node == NULL) {
		return minTmp;
	}
	if (o(node->payload, minTmp->payload) < 0) {
		//new minimum found
		minTmp = node;
	}
	//we're sure right branch can't offer any more minimum
	return findMinInBinaryNode(node->left, o, minTmp);
}

static bool containsItemInBinaryNode(const binary_node* node, void* payload, orderer o) {
	if (node == NULL) {
		return false;
	}

	int r = o(payload, node->payload);
	if (r == 0) {
		return true;
	}
	return containsItemInBinaryNode(r < 0 ? node->left : node->right, payload, o);

}

/**
 * Release from memory everything related to a binary node
 *
 * The function will release from memory the whole subtree generated by this ::binary_node
 *
 * @param[in] binaryNode the node to clean up
 * @param[in] withElements if true we will remove from memory the paylaod as well
 * @param[in] used only if \c withElements is set to true. Otherwise it is ignored
 */
static void destroyBinaryNode(binary_node* binaryNode, bool withElements, destructor d) {
	if (binaryNode->left != NULL) {
		destroyBinaryNode(binaryNode->left, withElements, d);
	}
	if (binaryNode->right != NULL) {
		destroyBinaryNode(binaryNode->right, withElements, d);
	}
	if (withElements) {
		d(binaryNode->payload);
	}
	free(binaryNode);
}

static bool addItemInBinaryNode(binary_node* binaryNode, void* payload, binary_tree* tree) {
	if (tree->o(payload, binaryNode->payload) < 0) {
		if (binaryNode->left == NULL) {
			binaryNode->left = initBinaryNode(binaryNode, NULL, NULL, payload);
			return true;
		} else {
			return addItemInBinaryNode(binaryNode->left, payload, tree);
		}
	} else {
		if (binaryNode->right == NULL) {
			binaryNode->right = initBinaryNode(binaryNode, NULL, NULL, payload);
			return true;
		} else {
			return addItemInBinaryNode(binaryNode->right, payload, tree);
		}
	}
}

static binary_node* initBinaryNode(binary_node* parent, binary_node* left, binary_node* right, void* payload) {
	binary_node* n = malloc(sizeof(binary_node));
	if (n == NULL) {
		ERROR_MALLOC();
	}
	n->payload = payload;
	n->parent = parent;
	n->left = left;
	n->right = right;

	return n;
}

