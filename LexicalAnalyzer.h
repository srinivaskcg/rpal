
#include <string>
#include <vector>
#include "Token.h"

using namespace std;

#ifndef LEXICALANALYZER_H_
#define LEXICALANALYZER_H_

class LexicalAnalyzer {
	private:
		string codeString;
		int size;
		int readPtr;
		bool isoperator(char ch);
		bool isKeyword(string tokenValue);

	public:
		LexicalAnalyzer(string codeString);
		LexicalAnalyzer();
		void operator=(const LexicalAnalyzer& laObj);
		virtual ~LexicalAnalyzer();
		Token getNextToken();
		void tokenizeStr();
		Token peekNextToken();
		vector<Token> tokens;
		int currPtr;
};

#endif /* LEXICALANALYZER_H_ */
