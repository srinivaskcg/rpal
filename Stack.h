/*
 * Stack.h
 *
 *  Created on: Oct 16, 2011
 *      Author: saurabh
 */

#ifndef STACK_H_
#define STACK_H_

#include "TreeNode.h"

class Stack {
private:
	TreeNode* top;
	int size;
public:
	Stack();
	virtual ~Stack();
	TreeNode* pop();
	void push(TreeNode* treeNode);
	void addRightChild(TreeNode* treeNode);
	int getSize();
};

#endif /* STACK_H_ */
