/*
 * CSEMachine.h
 *
 *  Created on: Dec 9, 2011
 *      Author: saurabh
 */

#ifndef CSEMACHINE_H_
#define CSEMACHINE_H_

#include <map>
#include <stack>
#include "Token.h"
#include "TreeNode.h"
#include <list>
#include <vector>
#include <queue>
#include <sstream>
#include <utility>

using namespace std;
typedef map<string,Token> envMap;
typedef pair<int,string> keyPair;

class CSEMachine {
public:
	CSEMachine();
	CSEMachine(TreeNode* input);
	virtual ~CSEMachine();
	void evaluateTree();
private:
	map<int, vector<Token> > deltaMap;
	ostringstream oss;
	int deltaCounter;
	int currDeltaNum;
	int envCounter;
	queue<TreeNode*> pendingDeltaQueue;
	TreeNode* inputTree;
	//map<int, envMap> paramMap;
	map<keyPair,Token> paramMap;
	void createControlStructures(TreeNode* root);
	void preOrderTraversal(TreeNode* root, vector<Token> &currentDelta);
	void processCurrentToken(Token &currToken, stack<Token> &controlStack, stack<Token> &executionStack);
	Token applyOperator(Token firstToken, Token secondToken, Token currToken);
	string intToString(int intValue);
	vector<string> split(string inputString, char delimiter);
	bool notFunction(string value);
	bool isParamter(Token currToken);
	map<int,int> envMap;
	stack<int> envStack;
	int currEnv;
	bool printCalled;
	string unescape(const string& s);
	void printTuple(Token t);
};


#endif /* CSEMACHINE_H_ */
