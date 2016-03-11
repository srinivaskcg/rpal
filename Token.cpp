#include "Token.h"

using namespace std;

Token::Token() {}

Token::Token(string value, string type){
	this->value = value;
	this->type = type;
}

Token::~Token() {}
