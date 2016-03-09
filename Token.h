/*
 * Token.h
 *
 *  Created on: Oct 14, 2011
 *      Author: Saurabh Saxena
 *      // UFID        : 21817195
 */
#include <string>

#ifndef TOKEN_H_
#define TOKEN_H_

class Token {
public:
	Token();
	Token(std::string value, std::string type);
	virtual ~Token();
	std::string value;
	std::string type;

};

#endif /* TOKEN_H_ */
