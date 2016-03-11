
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
		bool moreTokens;
		stack<Node *> trees;
		Token nextToken;

		bool anOperator(char ch);
		bool aKeyword(string tokenValue);
		bool aComment(char ch);
		bool aPunctuation(char ch);
		void makeRightNode(Node* t);
		void formattedPrint(Token t,std::string dots);
		void preOrdTraversal(Node* t, std::string dots);
		void readToken(Token token);
		void treeBuilder(string tokenVal, int popNodesCnt);
		
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

		vector<Token> tokens;
		int currPtr;

		Token getNextToken();
		Token peekNextToken();
		
		void tokenizeStr();
		
		Token tokenizeIdentifier(char ch);
		Token tokenizeInteger(char ch);
		Token tokenizeOperator(char ch);
		Token tokenizeString(char ch);
		Token tokenizePunctuation(char ch);

		void printTree();
		void parse();
};

#endif // LEXPARSER_H_ 
