
#include "TreeNode.h"

TreeNode::TreeNode() {
	right = NULL;
	left = NULL;
}

TreeNode::~TreeNode() {
	if(right != NULL)
		delete right;
	if(left != NULL)
		delete left;
}
