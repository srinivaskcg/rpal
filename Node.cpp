//
//  Node.cpp
//
//  Created by Srinivas Gubbala on 3/4/16.
//  UFID - 2131 7376

#include "Node.h"

Node::Node() {
	right = NULL;
	left = NULL;
}

Node::~Node() {
	if(right != NULL)
		delete right;
	if(left != NULL)
		delete left;
}
