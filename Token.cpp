//
//  Token.cpp
//
//  Created by Srinivas Gubbala on 3/4/16.

#include "Token.h"

using namespace std;

Token::Token(string value, string type){
	this->value = value;
	this->type = type;
}

Token::Token() {}

Token::~Token() {}
