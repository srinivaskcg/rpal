
#include <string>
#include <vector>
#include "Token.h"

using namespace std;

#ifndef LEXICALANALYZER_H_
#define LEXICALANALYZER_H_

class Lexer {
	private:
		string codeString;
		int size;
		int readPtr;
		bool isoperator(char ch);
		bool isKeyword(string tokenValue);

	public:
		Lexer(string codeString);
		Lexer();
		void operator=(const Lexer& laObj);
		virtual ~Lexer();
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

#endif /* LEXICALANALYZER_H_ */
