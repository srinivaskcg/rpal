/*
 * LexicalAnalyzer.h
 *
 *  Created on: Oct 14, 2011
 *      Author: Saurabh Saxena
 *      // UFID        : 21817195
 */
#include <string>
#include "Token.h"

#ifndef LEXICALANALYZER_H_
#define LEXICALANALYZER_H_

class LexicalAnalyzer {
	private:
		std::string codeString;
		int size;
		int readPtr;
		bool isoperator(char ch);
		bool isKeyword(std::string tokenValue);

	public:
		LexicalAnalyzer(std::string codeString);
		LexicalAnalyzer();
		void operator=(const LexicalAnalyzer& laObj);
		virtual ~LexicalAnalyzer();
		Token getNextToken();
		Token peekNextToken();
};

#endif /* LEXICALANALYZER_H_ */
