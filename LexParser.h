//
//  LexParser.h
//
//  Created by Srinivas Gubbala on 3/5/16.
//  UFID - 2131 7376


#ifndef LEXPARSER_H_
#define LEXPARSER_H_

#include <string>
#include <vector>
#include <unordered_set>
#include <stack>
#include <cstdlib>

#include "Token.h"
#include "Node.h"
#include "LexParser.h"

using namespace std;

class LexParser {

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
		Token nt;
		bool tokensLeft;
		stack<Node *> trees;

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

		void makeRightNode(Node* t);
		void preOrdTraversal(Node* t, std::string dots);
		void treeBuilder(string tokenVal, int popNodesCnt);
		void printTree();
		void formattedPrint(Token t,std::string dots);
		
		void E();
		void Ew();

		void T();
		void Ta();
		void Tc();
		
		void B();
		void Bt();
		void Bs();
		void Bp();
		
		void A();
		void At();
		void Af();
		void Ap();
		
		void R();
		void Rn();
		
		void D();
		void Da();
		void Dr();
		void Db();
		
		void Vb();
		void Vl();

	public:
		LexParser(string inputString);
		LexParser(){}

		void parse();
};

#endif // LEXPARSER_H_ 
