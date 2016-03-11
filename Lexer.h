
#include <string>
#include <vector>
#include <unordered_set>
#include "Token.h"

using namespace std;

#ifndef LEXER_H_
#define LEXER_H_

class Lexer {

	unordered_set<char> operator_set {
								    '+', '-', '*', '<', '>', '&', '.', '@', '/', ':', '=', '~', '|', '$',
								    '!', '#', '%', '^', '_', '[', ']', '{', '}', '"', '`', '?'
								  };

	unordered_set<string> keyword_set {
									"let","in","fn","where","aug","or","not","gr","ge","ls","le","eq","ne",
									"true","false","nil","dummy","within","and","rec","list"
								  };

	private:
		string inputString;
		int size;
		int presentVal;
		bool anOperator(char ch);
		bool aKeyword(string tokenValue);

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
