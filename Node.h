#ifndef NODE_H_
#define NODE_H_

#include "Token.h"

class Node {
public:
	Node();
	virtual ~Node();
	Token value;
	Node* right;
	Node* left;
};

#endif // NODE_H_

