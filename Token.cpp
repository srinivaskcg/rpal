/*
 * Token.cpp
 *
 *  Created on: Oct 14, 2011
 *      Author: Saurabh Saxena
 *      // UFID        : 21817195
 */

#include "Token.h"

Token::Token() {
	// TODO Auto-generated constructor stub

}

Token::Token(std::string value, std::string type){
	this->value = value;
	this->type = type;
}

Token::~Token() {
	// TODO Auto-generated destructor stub
}
