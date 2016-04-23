/*
 * Token.cpp
 *
 *  Created on: Oct 14, 2011
 *      Author: Saurabh Saxena
 *      // UFID        : 21817195
 */

#include "Token.h"
#include <vector>

using namespace std;

Token::Token() {
	// TODO Auto-generated constructor stub
	isTuple = false;
	construct();
}

Token::Token(std::string value, std::string type){
	this->value = value;
	this->type = type;
	isTuple = false;
	construct();
}

Token::Token(std::string type, int envNum){
	this->type = type;
	this->envNum = envNum;
	isTuple = false;
	construct();
}

void Token::construct(){
	this->tuple = vector<Token>();
}

Token::Token(std::string type,std::string lambdaParam, int lambdaNum){
	this->type = type;
	this->lambdaParam = lambdaParam;
	this->lambdaNum = lambdaNum;
	isTuple = false;
	construct();
}

Token::Token(std::string type, int betaIfDeltaNum, int betaElseDeltaNum){
	this->type = type;
	this->betaIfDeltaNum = betaIfDeltaNum;
	this->betaElseDeltaNum = betaElseDeltaNum;
	isTuple = false;
	construct();
}

Token::~Token() {
	// TODO Auto-generated destructor stub
}
