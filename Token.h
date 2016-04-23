/*
 * Token.h
 *
 *  Created on: Oct 14, 2011
 *      Author: Saurabh Saxena
 *      // UFID        : 21817195
 */
#include <string>
#include <vector>

#ifndef TOKEN_H_
#define TOKEN_H_

class Token {
public:
	Token();
	Token(std::string value, std::string type); //Simple token constructor
	Token(std::string type,std::string lamdaParam, int lamdaNum);// Constructor for lambda closure
	Token(std::string type, int envNum ); // Constructor for environment
	Token(std::string type, int betaIfDeltaNum, int betaElseDeltaNum);
	virtual ~Token();
	std::string value;
	std::string type;
	int lambdaNum;
	std::string lambdaParam;
	int envNum;
	int betaIfDeltaNum;
	int betaElseDeltaNum;
	int tauCount;
	bool isTuple;
	std::vector<Token> tuple;
	void construct();
	int lambdaEnv;
};

#endif /* TOKEN_H_ */
