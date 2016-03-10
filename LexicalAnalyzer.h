
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
};

#endif /* LEXICALANALYZER_H_ */
