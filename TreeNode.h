/*
 * TreeNode.h
 *
 *  Created on: Oct 16, 2011
 *      Author: Saurabh Saxena
 *      // UFID        : 21817195
 */

#ifndef TREENODE_H_
#define TREENODE_H_

#include "Token.h"

class TreeNode {
public:
	TreeNode();
	TreeNode(Token token);
	virtual ~TreeNode();
	Token value;
	TreeNode* right;
	TreeNode* left;
	TreeNode* next;
};

#endif /* TREENODE_H_ */

