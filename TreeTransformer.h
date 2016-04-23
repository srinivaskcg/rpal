/*
 * TreeTransformer.h
 *
 *  Created on: Dec 6, 2011
 *      Author: saurabh
 */

#ifndef TREETRANSFORMER_H_
#define TREETRANSFORMER_H_

#include "TreeNode.h"

class TreeTransformer {
private:
	TreeNode* createNewNode(TreeNode* t);
public:
	TreeTransformer();
	virtual ~TreeTransformer();
	TreeNode* transformTree(TreeNode* root);
	TreeNode* transformLET(TreeNode* root);
	TreeNode* transformWHERE(TreeNode* root);
	TreeNode* transformFCN_FORM(TreeNode* root);
	TreeNode* transformLAMDA(TreeNode* root);
	TreeNode* transformWITHIN(TreeNode* root);
	TreeNode* transformAT(TreeNode* root);
	TreeNode* transformAND(TreeNode* root);
	TreeNode* transformREC(TreeNode* root);
	Token lambda;
	Token gamma;
};


#endif /* TREETRANSFORMER_H_ */
