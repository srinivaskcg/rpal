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
		LexicalAnalyzer la;
		bool moreTokens;
		stack<TreeNode *> trees;
		Token nextToken;
		void addRightChild(TreeNode* t);
		void formattedPrint(Token t,std::string dots);
		void preOrder(TreeNode* t, std::string dots);
		void read(Token token);
		void buildTree(Token token, int numOfNodes);
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
		void BpHelper(Token nextToken, string tokenValue);
		void RHelper(Token t, string value);
	public:
		Parser(LexicalAnalyzer la);
		~Parser();
		void printTree();
		void parse();
};




#endif /*End of Recursive Parser*/
