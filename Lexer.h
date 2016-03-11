
#include <string>
#include <vector>
#include "Token.h"

using namespace std;

#ifndef LEXER_H_
#define LEXER_H_

class Lexer {
	private:
		string inputString;
		int size;
		int presentVal;
		bool anOperator(char ch);
		bool isKeyword(string tokenValue);

	public:
		Lexer(string inputString);
		Lexer(){}
		void operator = (const Lexer& laObj);
		Token getNextToken();
		void tokenizeStr();
		Token peekNextToken();
		vector<Token> tokens;
		int currPtr;
		Token tokenizeIdentifier(char ch);
		Token tokenizeInteger(char ch);
		Token tokenizeOperator(char ch);
		Token tokenizeString(char ch);
		Token tokenizePunctuation(char ch);
};

#endif // LEXER_H_ 
