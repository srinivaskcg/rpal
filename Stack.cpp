/*
 * Stack.cpp
 *
 *  Created on: Oct 16, 2011
 *      Author: saurabh Saxena
 *      // UFID        : 21817195
 */

#include "Stack.h"
#include <iostream>

Stack::Stack() {
	top = NULL;
	size = 0;
}

Stack::~Stack() {
	while(top != NULL){
		TreeNode* curr = top;
		top = top->next;
		delete curr;
	}
}

TreeNode* Stack::pop(){
	if(top == NULL)
		return NULL;
	TreeNode* curr = top;
	top = top->next;
	size--;
	return curr;
}


void Stack::push(TreeNode* treeNode){
	treeNode->next = top;
	size++;
	top = treeNode;
	//std::cout<<"Inside push "<< top<<" "<< top->next<<" "<<std::endl;
}

void Stack::addRightChild(TreeNode* treeNode){
	//std::cout<< "Top element value "<<top<<std::endl;
	//std::cout<<treeNode->value.value<<std::endl;
	top->right = treeNode;
	//std::cout<<top->value.value<<std::endl;
}


int Stack::getSize(){
	return this->size;
}
