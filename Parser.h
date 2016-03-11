#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <stack>
#include <cstdlib>
#include "Token.h"
#include "Node.h"
#include "Lexer.h"

using namespace std;

class Parser
{
	private:
		Lexer la;
		bool moreTokens;
		stack<Node *> trees;
		Token nextToken;
		void addRightChild(Node* t);
		void formattedPrint(Token t,std::string dots);
		void preOrder(Node* t, std::string dots);
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
		Parser(Lexer la);
		void printTree();
		void parse();
};

#endif // PARSER_H_
