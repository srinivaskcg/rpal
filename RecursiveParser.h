#ifndef RECURSIVEPARSER_H_
#define RECURSIVEPARSER_H_

#include <string>
#include "Token.h"
#include "TreeNode.h"
#include "LexicalAnalyzer.h"
#include <stack>
#include <cstdlib>

using namespace std;

class Parser
{
	private:
		static string ID;
		static string STR;
		static string INT;
		static string KEY;
		static string OPT;
		Lexer la;
		bool moreTokens;
		stack<TreeNode *> trees;
		Token nextToken;
		void addRightChild(TreeNode* t);
		void formattedPrint(Token t,std::string dots);
		void preOrder(TreeNode* t, std::string dots);
		void readToken(Token token);
		void treeBuilder(Token token, int popNodesCnt);
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
		~Parser();
		void printTree();
		void parse();
};

#endif /*End of Parser*/
