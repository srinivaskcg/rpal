/*
 * TreeTransformer.cpp
 *
 *  Created on: Dec 6, 2011
 *      Author: saurabh
 */

#include "TreeTransformer.h"
#include <iostream>
#include <queue>
#include <stack>


using namespace std;


void printNodes(TreeNode* node);

TreeTransformer::TreeTransformer() {
	// TODO Auto-generated constructor stub
	this->gamma = Token("gamma","gamma");
	this->lambda = Token("lambda","lambda");
}

TreeTransformer::~TreeTransformer() {
	// TODO Auto-generated destructor stub
}

TreeNode* TreeTransformer::transformTree(TreeNode* root){
	TreeNode* topRoot = root;
	TreeNode* nextChild = topRoot->left;
	bool firstChild = true;
	while(nextChild != NULL){
		TreeNode* transformedChild = transformTree(nextChild);
		//cout << "Root: "<<root->value.value<<" Child: "<<transformedChild->value.value<<endl;
		if(firstChild == true){
			topRoot->left = transformedChild;
			topRoot = topRoot->left;
			firstChild = false;
		}else{
			topRoot->right = transformedChild;
			topRoot = topRoot->right;
		}
		nextChild = nextChild->right;
	}
	Token rootToken = root->value;
	if(rootToken.value=="let"){
		root = transformLET(root);
	}else if(rootToken.value =="where"){
		root = transformWHERE(root);
	}else if(rootToken.value == "function_form"){
		root = transformFCN_FORM(root);
	}else if(rootToken.value == "within"){
		root = transformWITHIN(root);
	}else if(rootToken.value == "and"){
		root = transformAND(root);
	}else if(rootToken.value == "@"){
		root = transformAT(root);
	}else if(rootToken.value == "lambda"){
		root = transformLAMDA(root);
	}else if(rootToken.value == "rec"){
		root = transformREC(root);
	}
	return root;
}



TreeNode* TreeTransformer::transformLET(TreeNode* letNode){
	TreeNode* lambdaNode = new TreeNode(this->lambda);
	TreeNode* gammaNode = new TreeNode(this->gamma);
	gammaNode->left = lambdaNode;
	lambdaNode->right = letNode->left->left->right;
	lambdaNode->left = letNode->left->left;
	lambdaNode->left->right = letNode->left->right;
	return gammaNode;
}


TreeNode* TreeTransformer::transformWHERE(TreeNode* whereNode){
	TreeNode* lambdaNode = new TreeNode(this->lambda);
	TreeNode* gammaNode = new TreeNode(this->gamma);
	gammaNode->left = lambdaNode;
	gammaNode->left->right = whereNode->left->right->left->right;
	gammaNode->left->left = whereNode->left->right->left;
	gammaNode->left->left->right = whereNode->left;
	gammaNode->left->left->right->right = NULL;
	return gammaNode;
}


TreeNode* TreeTransformer::transformFCN_FORM(TreeNode* fcnNode){
	Token equalToken("=","=");
	TreeNode* equalNode = new TreeNode(equalToken);
	int count = 0;
	stack<TreeNode*> stk;
	stk.push(fcnNode->left); //Pushing name of the function
	TreeNode* childNode = fcnNode->left->right; //First Variable
	while(childNode != NULL){
		count++;
		stk.push(childNode);
		childNode = childNode->right;
	}
	count = count-1;
	//cout << "Count: "<< count<<endl;
	while(count != 0){
		TreeNode* firstNode = stk.top();
		stk.pop();
		TreeNode* secondNode = stk.top();
		stk.pop();
		//cout << firstNode->value.value << " "<<secondNode->value.value<<endl;
		secondNode->right = firstNode;
		TreeNode* lambdaNode = new TreeNode(this->lambda);
		lambdaNode->left = secondNode;
		stk.push(lambdaNode);
		count--;
	}
	TreeNode* firstNode = stk.top();
	stk.pop();
	TreeNode* secondNode = stk.top();
	stk.pop();
	//cout << firstNode->value.value << " "<<secondNode->value.value<<endl;
	secondNode->right = firstNode;
	equalNode->left = secondNode;
	return equalNode;
}

TreeNode* TreeTransformer::transformWITHIN(TreeNode* withinNode){
	Token equalToken("=","=");
	TreeNode* equalNode = new TreeNode(equalToken);
	TreeNode* gammaNode = new TreeNode(this->gamma);
	TreeNode* lambdaNode = new TreeNode(this->lambda);
	gammaNode->left = lambdaNode;
	lambdaNode->right = withinNode->left->left->right;//E1 node
	lambdaNode->left = withinNode->left->left; // X1 node
	lambdaNode->left->right = withinNode->left->right->left->right; // E2 node#
	equalNode->left = withinNode->left->right->left; // X2 node
	equalNode->left->right = gammaNode;

	//cout<<"withinNode->left->right->left->right "<<withinNode->left->right->left->right->value.value<<endl;
	//cout<<"withinNode->left->right->left "<<withinNode->left->right->left->value.value<<endl;
	//cout<<"withinNode->left->right "<<withinNode->left->right->value.value<<endl;
	//cout<<"Printing within node"<<endl;
	//printNodes(equalNode);
	return equalNode;
}


TreeNode* TreeTransformer::transformAND(TreeNode* andNode){
	Token equalToken("=","=");
	TreeNode* equalNode = new TreeNode(equalToken);
	Token commaToken(",",",");
	TreeNode* commaNode = new TreeNode(commaToken);
	Token tauToken("tau","tau");
	TreeNode* tauNode = new TreeNode();
	equalNode->left = commaNode;
	commaNode->right = tauNode;
	TreeNode* andChild = andNode->left; //First child
	queue<TreeNode*> equalQueue;
	int tauCount = 0;
	while(andChild != NULL){
		equalQueue.push(andChild);
		andChild = andChild->right;
		tauCount++;
	}
	tauToken.tauCount = tauCount;
	tauNode->value = tauToken;
	TreeNode* currParam = NULL;
	TreeNode* currValue = NULL;
	while(!equalQueue.empty()){
		TreeNode* eqNode = equalQueue.front();
		equalQueue.pop();
		TreeNode* param = eqNode->left;
		TreeNode* value = eqNode->left->right;
		param->right = NULL;
		if(currParam == NULL){
			currParam = param;
			currValue = value;
			commaNode->left = currParam;
			tauNode->left = currValue;
		}else{
			currParam->right = param;
			currValue->right = value;
			currParam = param;
			currValue = value;
		}
	}
	return equalNode;
}

TreeNode* TreeTransformer::transformAT(TreeNode* root){
	TreeNode* gammaNode1 = new TreeNode(this->gamma);
	TreeNode* gammaNode2 = new TreeNode(this->gamma);
	gammaNode1->left = gammaNode2;
	TreeNode* e2 = root->left->right->right;
	TreeNode* n = root->left->right;
	n->right = NULL;
	TreeNode* e1  =root->left;
	e1->right = NULL;
	gammaNode2->left = n;
	n->right = e1;
	gammaNode2->right = e2;
	return gammaNode1;
}

TreeNode* TreeTransformer::transformLAMDA(TreeNode* root){
	//Returning if only one variable is there
	if(root->left->right->right == NULL)
		return root;
	stack<TreeNode*> s;
	TreeNode* childNode = root->left;
	while(childNode != NULL){
		s.push(childNode);
		TreeNode* nextChild = childNode->right;
		childNode->right = NULL;
		childNode = nextChild;
	}
	while(s.size() != 1){
		TreeNode* rightChild = s.top();
		s.pop();
		TreeNode* leftChild = s.top();
		s.pop();
		TreeNode* lambdaNode = new TreeNode(this->lambda);
		lambdaNode->left = leftChild;
		leftChild->right = rightChild;
		s.push(lambdaNode);
	}
	TreeNode* newst = s.top();
	s.pop();
	return newst;
}

TreeNode* TreeTransformer::transformREC(TreeNode* root){
	Token equalToken("=","=");
	TreeNode* equalNode = new TreeNode(equalToken);
	TreeNode* gammaNode = new TreeNode(this->gamma);
	TreeNode* lambdaNode = new TreeNode(this->lambda);
	Token ystarToken("YSTAR","YSTAR");
	TreeNode* ystarNode = new TreeNode(ystarToken);
	TreeNode* e = root->left->left->right;
	TreeNode* x1 = root->left->left;
	x1->right = NULL;
	TreeNode* x2 = new TreeNode(x1->value);
	equalNode->left = x1;
	x1->right = gammaNode;
	gammaNode->left = ystarNode;
	ystarNode->right= lambdaNode;
	lambdaNode->left = x2;
	x2->right = e;
	return equalNode;


}

TreeNode* TreeTransformer::createNewNode(TreeNode* t){
	return new TreeNode(t->value);
}

void printNodes(TreeNode* node){
	cout<<node->value.value<<endl;
	if(node->left !=NULL)
		printNodes(node->left);
	if(node->right !=NULL)
		printNodes(node->right);
}

