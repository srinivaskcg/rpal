/*
 * CSEMachine.cpp
 *
 *  Created on: Dec 9, 2011
 *      Author: saurabh
 */

#include "CSEMachine.h"
 #include "LexicalAnalyzer.h"
#include "TreeNode.h"
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <sstream>
#include <iostream>
#include <stack>
#include <cmath>
#include <utility>

typedef pair<int,string> key_pair;

CSEMachine::CSEMachine() {
	// TODO Auto-generated constructor stub

}

CSEMachine::~CSEMachine() {
	// TODO Auto-generated destructor stub
}


CSEMachine::CSEMachine(TreeNode* input){
	this->inputTree = input;
	this->deltaCounter = 0;
	this->currDeltaNum = 0;
	this->envCounter = 0;
	this->envStack.push(0);
	this->currEnv = 0;
	this->envMap = map<int,int>();
	this->printCalled = false;
}

void CSEMachine::evaluateTree(){
	createControlStructures(this->inputTree);
	Token envToken("env",envCounter);
	stack<Token> controlStack;
	stack<Token> executionStack;
	controlStack.push(envToken);
	//Making parent of env 0 , -1
	envMap[0] = -1;

	vector<Token> delta0 = deltaMap[0];
	for(int i=0;i<delta0.size();i++){
		controlStack.push(delta0[i]);
	}
	executionStack.push(envToken);
	int whileCount = 0;
	while(controlStack.size() != 1){
		Token currToken = controlStack.top();
		controlStack.pop();
		processCurrentToken(currToken,controlStack,executionStack);
		//cout<<"Inside while: "<<controlStack.size()<< endl;
		if(whileCount > 5000){
			break;
		}
		whileCount++;
	}
	if(printCalled == false)
		cout<<endl;
	//cout<<endl;
	//cout<<"Execution result: "<<executionStack.top().value<<endl;
}

void CSEMachine::processCurrentToken(Token &currToken,stack<Token> &controlStack, stack<Token> &executionStack){
	//cout<<"Control stack top: "<<currToken.type <<" Exe top: "<<executionStack.top().type<< endl;
	//cout<<"Control stack top: "<<currToken.value <<" Exe top: "<<executionStack.top().value<< endl;
	if(currToken.type == LexicalAnalyzer::OPT){
		Token firstToken = executionStack.top();
		executionStack.pop();
		Token secondToken = executionStack.top();
		executionStack.pop();
		Token resultToken = applyOperator(firstToken, secondToken, currToken);
		executionStack.push(resultToken);
	}else if(currToken.type == "neg"){
		Token firstToken = executionStack.top();
		executionStack.pop();
		int paramVal = atoi(firstToken.value.c_str());
		paramVal = -paramVal;
		Token newToken(intToString(paramVal), LexicalAnalyzer::INT);
		executionStack.push(newToken);
	}else if(currToken.type =="not"){
		Token firstToken = executionStack.top();
		executionStack.pop();
		if(firstToken.value == "true"){
			executionStack.push(Token("false","false"));
		}else{
			executionStack.push(Token("true","true"));
		}
	//}else if(currToken.type == LexicalAnalyzer::ID && isParamter(currToken)){
	}else if(currToken.type == LexicalAnalyzer::ID && isParamter(currToken)){
		string varName = currToken.value;
		int temp = currEnv;
		//cout <<  "Current env "<<temp<< endl;
		pair<int,string> keyPair(temp,varName);
		map<key_pair,Token>::iterator it = paramMap.find(keyPair);
		while(paramMap.end() == it && temp>=0){
			temp = envMap[temp];
			keyPair.first = temp;
			//cout << "Temp: "<<temp;
			it = paramMap.find(keyPair);
		}
		if(paramMap.end() != it){
			Token paramValToken = it->second;
			//paramMap.erase(it);
			executionStack.push(paramValToken);
		}
	}else if(currToken.type == "gamma"){
		Token topExeToken = executionStack.top();
		executionStack.pop();
		if(topExeToken.type == "lambdaClosure"){
			Token env("env",++envCounter);
			//cout<< "Parent: "<< topExeToken.lambdaEnv << " Env: "<< envCounter;
			envMap[envCounter] = topExeToken.lambdaEnv;
			envStack.push(envCounter);
			currEnv = envCounter;
			if(topExeToken.isTuple == false){
				string paramName = topExeToken.lambdaParam;
				Token paramToken = executionStack.top();
				executionStack.pop();
				//int paramValue = atoi(paramToken.value.c_str());
				pair<int,string> keyPair(envCounter,paramName);
				paramMap[keyPair] = paramToken;
				//cout<< "Inside if"<<paramToken.type<<" Param name "<<paramName<< " Environment "<<envCounter<<endl;
			}else{
				//cout << "Inside else"<<endl;
				string tuple = topExeToken.lambdaParam;
				vector<string> params = split(tuple,',');
				Token valueTuple = executionStack.top();
				//cout << "Value tuple: "<< valueTuple.type<< valueTuple.value <<endl;
				executionStack.pop();
				vector<Token> tupleVector = valueTuple.tuple;
				//cout<< "Num of parameters "<< params.size() << " Num of values "<<tupleVector.size();
				for(unsigned int i=0;i<params.size();i++){
					if(params[i] != ""){
						pair<int,string> keyPair(envCounter,params[i].c_str());
						paramMap[keyPair] = tupleVector[i];
					}
					//cout<< "Inside for loop "<<endl;
				}
				//cout<< "Outside for"<<endl;
				////cout << "Escaping else"<<paramMap["x"]<<paramMap["y"]<<endl;
			}
			controlStack.push(env);
			executionStack.push(env);
			int lambdaNum = topExeToken.lambdaNum;
			vector<Token> delta = deltaMap[lambdaNum];
			for(int i=0;i<delta.size();i++){
				controlStack.push(delta[i]);
			}
		}else if(topExeToken.type == "YSTAR"){
			Token nextToken = executionStack.top();
			//cout << "Inside Ystar "<< nextToken.type<<endl;
			executionStack.pop();
			nextToken.type ="eta";
			executionStack.push(nextToken);
		}else if(topExeToken.type == "eta"){
			Token lambdaToken = topExeToken;
			lambdaToken.type = "lambdaClosure";
			executionStack.push(topExeToken);
			executionStack.push(lambdaToken);
			Token gammaToken("gamma","gamma");
			controlStack.push(gammaToken);
			controlStack.push(gammaToken);
		}else if(topExeToken.value == "Stern" || topExeToken.value == "stern"){
			Token stringToken = executionStack.top();
			executionStack.pop();
			string tokenValue = stringToken.value;
			tokenValue = tokenValue.substr(2,tokenValue.size()-3);
			tokenValue = "'"+tokenValue+"'";
			stringToken.value = tokenValue;
			executionStack.push(stringToken);
		}else if(topExeToken.value == "Stem" || topExeToken.value == "stem"){
			Token stringToken = executionStack.top();
			executionStack.pop();
			string tokenValue = stringToken.value;
			tokenValue = tokenValue.substr(1,1);
			tokenValue = "'"+tokenValue+"'";
			stringToken.value = tokenValue;
			executionStack.push(stringToken);
		}else if(topExeToken.value == "Conc" || topExeToken.value == "conc"){
			Token firstToken = executionStack.top();
			executionStack.pop();
			Token secondToken = executionStack.top();
			executionStack.pop();
			//cout<< "Inside Concat 1 "<<firstToken.value << " 2 "<<secondToken.value<<endl;
			string concatValue = firstToken.value.substr(1,firstToken.value.size()-2)+secondToken.value.substr(1,secondToken.value.size()-2);
			concatValue = "'"+concatValue+"'";
			//cout <<"Concat value "<<concatValue<<endl;
			Token newToken(concatValue,LexicalAnalyzer::STR);
			executionStack.push(newToken);
			//Removing extra gamma
			controlStack.pop();
		}else if(topExeToken.value == "ItoS" || topExeToken.value == "itos"){
			Token firstToken = executionStack.top();
			executionStack.pop();
			firstToken.type = LexicalAnalyzer::STR;
			firstToken.value = "'"+firstToken.value+"'";
			executionStack.push(firstToken);
			//Removing extra gamma
			//scontrolStack.pop();
		}else if(topExeToken.value == "Print" || topExeToken.value == "print"){
			printCalled = true;
			//cout << "Inside print" << endl;
			Token t = executionStack.top();
			executionStack.pop();
			if(t.isTuple == false){
				if(t.type== LexicalAnalyzer::STR){
					string tempStr =unescape(t.value.substr(1,t.value.size()-2));
					cout << tempStr;
					if(tempStr[tempStr.size()-1] == '\n')
						cout<<endl;
					//cout << t.value.substr(1,t.value.size()-2);
				}else if(t.type == "lambdaClosure"){
					cout <<"[lambda closure: "<<t.lambdaParam<<": "<<t.lambdaNum<<"]";
				}else{
					//cout<<t.value<<endl;
					cout<<t.value;
				}
				Token dummyToken("dummy","dummy");
				executionStack.push(dummyToken);
			}else{
				vector<Token> tupleVector = t.tuple;
				for(int i=0;i<tupleVector.size();i++){
					if(i==0){
						cout<<"(";
					}else{
						cout<<", ";
					}
					if(tupleVector[i].type == LexicalAnalyzer::STR){
						cout<< unescape(tupleVector[i].value.substr(1,tupleVector[i].value.size()-2));
					}else if(tupleVector[i].isTuple == true ){
						cout<<"Inside else if"<<endl;
						vector<Token> innerTuple = tupleVector[i].tuple;
						cout << "Size" << innerTuple.size()<<endl;
						if(innerTuple.size() == 1){
							if(innerTuple[0].type == LexicalAnalyzer::STR)
								cout<< unescape(innerTuple[0].value.substr(1,innerTuple[0].value.size()-2));
						}
					}else{
						cout << tupleVector[i].value;
					}
					if(i==tupleVector.size() -1){
						cout<<")";
					}
				}
			}
			//cout<< endl;
		}else if(topExeToken.value == "Isinteger"){
			Token t = executionStack.top();
			executionStack.pop();
			executionStack.push(t.type==LexicalAnalyzer::INT ? Token("true","true"):Token("false","false"));
		}else if(topExeToken.value == "Istruthvalue"){
			Token t = executionStack.top();
			executionStack.pop();
			executionStack.push(t.value=="true" || t.value=="false" ? Token("true","true"):Token("false","false"));
		}else if(topExeToken.value == "Isstring"){
			Token t = executionStack.top();
			executionStack.pop();
			executionStack.push(t.type==LexicalAnalyzer::STR ? Token("true","true"):Token("false","false"));
		}else if(topExeToken.value == "Istuple"){
			//cout<<"Inside is tuple"<<endl;
			Token t = executionStack.top();
			executionStack.pop();
			executionStack.push(t.isTuple==true ? Token("true","true"):Token("false","false"));
		}else if(topExeToken.value == "Isdummy"){
			Token t = executionStack.top();
			executionStack.pop();
			executionStack.push(t.value=="dummy" ? Token("true","true"):Token("false","false"));
		}else if(topExeToken.value == "Isfunction"){
			Token t = executionStack.top();
			executionStack.pop();
			executionStack.push(t.type=="lambdaClosure" ? Token("true","true"):Token("false","false"));
		}else if(topExeToken.value == "Order"){
			//cout<<"Inside Order "<<endl;
			Token t = executionStack.top();
			executionStack.pop();
			executionStack.push(Token(intToString(t.tuple.size()),LexicalAnalyzer::INT));
		}else if(topExeToken.value == "Null"){
			//cout<<"Inside Null "<<endl;
			Token t = executionStack.top();
			executionStack.pop();
			executionStack.push((t.value == "nil") ? Token("true","true"):Token("false","false"));
		}else if(topExeToken.isTuple == true){
			Token t = executionStack.top();
			executionStack.pop();
			if(t.type == LexicalAnalyzer::INT){
				int indx = atoi(t.value.c_str());
				indx -=1;
				executionStack.push(topExeToken.tuple[indx]);
			}
		}
	}else if(currToken.type =="env"){
		Token topToken = executionStack.top();
		executionStack.pop();
		executionStack.pop();
		executionStack.push(topToken);
		envStack.pop();
		currEnv = envStack.top();
	}else if(currToken.type == "beta"){
		Token topToken = executionStack.top();
		executionStack.pop();
		vector<Token> delta;
		if(topToken.value == "true"){
			delta = deltaMap[currToken.betaIfDeltaNum];
		}else{
			delta = deltaMap[currToken.betaElseDeltaNum];
		}
		for(unsigned int i=0;i<delta.size();i++){
			controlStack.push(delta[i]);
		}
	}else if(currToken.value == "tau"){
		int tauCount = currToken.tauCount;
		//cout << "Tau count "<< tauCount<< endl;
		string tuple="(";
		vector<Token> tupleVector;
		for(int i=0;i<tauCount;i++){
			Token t = executionStack.top();
			tupleVector.push_back(t);
			executionStack.pop();
			if(i == tauCount -1)
				tuple += t.value;
			else
				tuple += t.value +", ";
		}
		tuple +=")";
		//cout<< "Tuple value: " <<tuple<<endl;
		Token newToken(tuple,"tuple");
		newToken.tuple = tupleVector;
		newToken.isTuple = true;
		executionStack.push(newToken);
	}else if(currToken.value == "nil"){
		currToken.isTuple = true;
		executionStack.push(currToken);
	}else if(currToken.value == "aug"){
		//cout <<"Inside aug "<<endl;
		Token tuple = executionStack.top();
		executionStack.pop();
		Token toAdd = executionStack.top();
		executionStack.pop();
		if(tuple.value == "nil"){
			//Token newToken("("+toAdd.value+")","tuple");
			Token newToken(toAdd.value,"tuple");
			newToken.isTuple = true;
			newToken.tuple = vector<Token>();
			newToken.tuple.push_back(toAdd);
			executionStack.push(newToken);
		}else{
			tuple.tuple.push_back(toAdd);
			executionStack.push(tuple);
		}
	}else if(currToken.type == "lambdaClosure"){
		//cout<< "Inside lambdaclosure env set"<<endl;
		currToken.lambdaEnv = currEnv;
		executionStack.push(currToken);
	}else{
		executionStack.push(currToken);
	}
}


Token CSEMachine::applyOperator(Token firstToken, Token secondToken, Token currToken){
	string tokenVal = currToken.value;
	//cout <<"Operator: "<< currToken.value<< endl;
	if(firstToken.type == LexicalAnalyzer::INT){
		int firstVal = atoi(firstToken.value.c_str());
		int secondVal = atoi(secondToken.value.c_str());
		//cout<< firstVal << " "<< secondVal<< endl;
		int resultVal = 0;
		if(tokenVal == "*"){
			resultVal = firstVal*secondVal;
			return Token(intToString(resultVal),firstToken.type);
		}else if(tokenVal == "+"){
			resultVal = firstVal+secondVal;
			return Token(intToString(resultVal),firstToken.type);
		}else if(tokenVal == "-"){
			resultVal = firstVal-secondVal;
			return Token(intToString(resultVal),firstToken.type);
		}else if(tokenVal == "/"){
			resultVal = firstVal/secondVal;
			return Token(intToString(resultVal),firstToken.type);
		}else if(tokenVal == "**"){
			resultVal = pow(firstVal,secondVal);
			return Token(intToString(resultVal),firstToken.type);
		}else if(tokenVal == "gr"){
			return (firstVal > secondVal ? Token("true","true"):Token("false","false"));
		}else if(tokenVal == "ls"){
			//cout << "Inside less than" <<endl;
			return (firstVal < secondVal ? Token("true","true"):Token("false","false"));
		}else if(tokenVal == "ge"){
			return (firstVal >= secondVal ? Token("true","true"):Token("false","false"));
		}else if(tokenVal == "le"){
			return (firstVal <= secondVal ? Token("true","true"):Token("false","false"));
		}else if(tokenVal == "eq"){
			return (firstVal == secondVal ? Token("true","true"):Token("false","false"));
		}else if(tokenVal == "ne"){
			return (firstVal != secondVal ? Token("true","true"):Token("false","false"));
		}
	}else if(firstToken.type == LexicalAnalyzer::STR){ // String operators
		if(tokenVal == "eq"){
			return (firstToken.value == secondToken.value ? Token("true","true"):Token("false","false"));
		}else if(tokenVal == "ne"){
			return (firstToken.value != secondToken.value ? Token("true","true"):Token("false","false"));
		}
	}else if(firstToken.type == "true" || firstToken.type == "false"){ // Boolean operators
		//cout<< "Inside boolean field"<<endl;
		if(tokenVal == "or"){
			//cout << "Inside or "<<firstToken.type << " "<<secondToken.type<<endl;
			return ((firstToken.type=="true" || secondToken.type=="true")? Token("true","true"): Token("false","false"));
		}else if(tokenVal == "&"){
			return ((firstToken.type=="true" && secondToken.type=="true")? Token("true","true"): Token("false","false"));
		}else if(tokenVal == "eq"){
			return ((firstToken.type==secondToken.type)? Token("true","true"): Token("false","false"));
		}else if(tokenVal == "ne"){
			return ((firstToken.type != secondToken.type)? Token("true","true"): Token("false","false"));
		}
	}
	return Token("","");
}
void CSEMachine::createControlStructures(TreeNode* root){
	//Read the next preorder node from tree, how about saving all the nodes in a queue in one go.
	//Check if it is lambda or not
	//if it is lambda create a lambda closure and put it in stack, insert the next next preorder node in a queue.
	//if not then put the token in current delta.
	//if no more preorder elements start a new tree with elements present in the stack, insert the delta in the stack with the
	//Appropriate number.
	pendingDeltaQueue.push(root);
	while(!pendingDeltaQueue.empty()){
		vector<Token> currentDelta;
		TreeNode* currStartNode = pendingDeltaQueue.front();
		pendingDeltaQueue.pop();
		preOrderTraversal(currStartNode, currentDelta);
		deltaMap[currDeltaNum++] = currentDelta;
	}
	for(int j=0;j<deltaMap.size();j++){
		//cout <<"Size for map "<<j<<" "<<deltaMap[j].size()<<endl;
		for(int i=0;i<deltaMap[j].size();i++){
				//cout << deltaMap[j][i].type<<",";
			}
		//cout << endl;
	}

}


void CSEMachine::preOrderTraversal(TreeNode* root, vector<Token> &currentDelta){
	if(root->value.type == "lambda"){
		if(root->left->value.value != ","){
			Token lambdaClosure("lambdaClosure",root->left->value.value, ++deltaCounter);
			currentDelta.push_back(lambdaClosure);
		}else{
			TreeNode* commaChild = root->left->left;
			string tuple;
			while(commaChild != NULL){
				tuple += commaChild->value.value + ",";
				commaChild = commaChild->right;
			}
			Token lambdaClosure("lambdaClosure",tuple, ++deltaCounter);
			lambdaClosure.isTuple = true;
			currentDelta.push_back(lambdaClosure);
		}
		//cout<<currentDelta.size()<<endl;
		pendingDeltaQueue.push(root->left->right);
		if(root->right !=NULL)
					preOrderTraversal(root->right,currentDelta);
	}else if(root->value.value == "->"){
		Token betaToken("beta",deltaCounter+1,deltaCounter+2);
		currentDelta.push_back(betaToken);
		pendingDeltaQueue.push(root->left->right);
		pendingDeltaQueue.push(root->left->right->right);

		root->left->right->right = NULL;
		root->left->right = NULL; //Not sure about this
		deltaCounter +=2;
		if(root->left != NULL)
				preOrderTraversal(root->left,currentDelta);
		if(root->right !=NULL)
				preOrderTraversal(root->right,currentDelta);
	}else{
		currentDelta.push_back(root->value);
		if(root->left != NULL)
				preOrderTraversal(root->left,currentDelta);
		if(root->right !=NULL)
					preOrderTraversal(root->right,currentDelta);
	}

}


string CSEMachine::intToString(int intValue){
	ostringstream oss;
	oss<<intValue;
	return oss.str();
}

vector<string> CSEMachine::split(string inputString, char delimiter){
	vector<string> result;
	string::iterator it;
	string temp;
	for(it = inputString.begin();it < inputString.end();it++){
		if(*it == delimiter){
			result.push_back(temp);
			temp = string();
		}else{
			temp +=*it;
		}
	}
	return result;
}

bool CSEMachine::notFunction(string identifier){
	string functions[] = {"Stem","stem","Stern","stern","Print","print","Conc","conc","Istuple","Isinteger","Isfunction","Istruthvalue",
			"Isdummy","Order","Null"};
	for(int i=0;i<15;i++){
		if(identifier == functions[i])
			return false;
	}
	return true;
}

bool CSEMachine::isParamter(Token currToken){
	string varName = currToken.value;
	int temp = currEnv;
	pair<int,string> keyPair(temp,varName);
	map<key_pair,Token>::iterator it = paramMap.find(keyPair);
	while(paramMap.end() == it && temp>=0){
		temp = envMap[temp];
		keyPair.first = temp;
		//cout << "Temp: "<<temp;
		it = paramMap.find(keyPair);
	}
	if(paramMap.end() != it){
		return true;
	}else{
		return false;
	}
}


string CSEMachine::unescape(const string& s)
{
  string res;
  string::const_iterator it = s.begin();
  while (it != s.end())
  {
    char c = *it++;
    if (c == '\\' && it != s.end())
    {
      switch (*it++) {
      case '\\': c = '\\'; break;
      case 'n': c = '\n'; break;
      case 't': c = '\t'; break;
      // all other escapes
      default:
        // invalid escape sequence - skip it. alternatively you can copy it as is, throw an exception...
        continue;
      }
    }
    res += c;
  }

  return res;
}

void CSEMachine::printTuple(Token t){
	vector<Token> tupleVector = t.tuple;
	for(int i=0;i<tupleVector.size();i++){
		if(i==0){
			cout<<"(";
		}else{
			cout<<", ";
		}
		if(tupleVector[i].type == LexicalAnalyzer::STR){
			cout<< unescape(tupleVector[i].value.substr(1,tupleVector[i].value.size()-2));
		}else if(tupleVector[i].type == "tuple"){
			printTuple(tupleVector[i]);
		}else{
			cout << tupleVector[i].value;
		}
		if(i==tupleVector.size() -1){
			cout<<")";
		}
	}

}
