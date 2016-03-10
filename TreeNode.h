#ifndef TREENODE_H_
#define TREENODE_H_

#include "Token.h"

class TreeNode {
public:
	TreeNode();
	virtual ~TreeNode();
	Token value;
	TreeNode* right;
	TreeNode* left;
	TreeNode* next;
};

#endif /* TREENODE_H_ */

