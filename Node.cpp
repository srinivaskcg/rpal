
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
