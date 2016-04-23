/*
 * LexicalAnalyzer.h
 *
 *  Created on: Oct 14, 2011
 *      Author: Saurabh Saxena
 *      // UFID        : 21817195
 */

#include <string>
#include <vector>
#include <unordered_set>
#include <stack>
#include <cstdlib>
#include "Token.h"
 #include "TreeNode.h"

#ifndef LEXICALANALYZER_H_
#define LEXICALANALYZER_H_

 using namespace std;

class LexicalAnalyzer {

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
		int currPtr;
		vector<Token> tokens;
		bool tokensLeft;

		bool moreTokens;
		stack<TreeNode*> stk;
		Token nextToken;
		void formattedPrint(Token t,string dots);
		void preOrder(TreeNode* t, string dots);
		void read(Token token);
		void buildTree(Token token, int numOfNodes);
		void parseE();
		void parseEw();
		void parseT();
		void parseTa();
		void parseTc();
		void parseB();
		void parseBt();
		void parseBs();
		void parseBp();
		void parseA();
		void parseAt();
		void parseAf();
		void parseAp();
		void parseR();
		void parseRn();
		void parseD();
		void parseDa();
		void parseDr();
		void parseDb();
		void parseVb();
		void parseV1();
		void parseBpHelper(Token nextToken, string tokenValue);
		void parseRHelper(Token t, string value);

	public:
		LexicalAnalyzer(string codeString);
		LexicalAnalyzer();
		//void operator=(const LexicalAnalyzer& laObj);
		virtual ~LexicalAnalyzer();
		bool anOperator(char c);
		bool aKeyword(string tokenValue);
		bool aComment(char c);
		bool aPunctuation(char c);

		void tokenizeStr();
		Token tokenizeIdentifier(char ch);
		Token tokenizeInteger(char ch);
		Token tokenizeOperator(char ch);
		Token tokenizeString(char ch);
		Token tokenizePunctuation(char ch);
		void readToken(Token token);
		Token getNextToken();
		Token peekNextToken();

		void parse();
		TreeNode* getTree();

		static string ID;
	static string STR;
	static string INT;
	static string KEY;
	static string OPT;

		void printTree();
		void makeRightNode(TreeNode* t);
};

#endif /* LEXICALANALYZER_H_ */
