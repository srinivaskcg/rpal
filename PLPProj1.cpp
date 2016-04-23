
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

#include "LexicalAnalyzer.h"
#include "RecursiveParser.h"
#include "TreeTransformer.h"
#include "TreeNode.h"
#include "CSEMachine.h"

using namespace std;

void preOrder(TreeNode* t, std::string dots);
void formattedPrint(Token t,std::string dots);

string openFile(char* fileName){
	
	string file_content;
	string next_line;
	ifstream input_file;

	input_file.open(fileName);

	if(input_file.fail())
		cerr << "Error: Problem opening input file";

	if(input_file.is_open()){
		while(input_file.good()){
			getline(input_file,next_line);
			file_content +=  next_line + "\n";
		}
	}
	input_file.close();
	return file_content;
}

int main(int argc,char *argv[]) {

	int i;
	char* file_name;
	bool ast_switch = false;
	bool l_switch = false;

	if(argc == 4){
		if(((string) argv[1]).compare("-l") == 0)
			l_switch = true;
		if(((string) argv[2]).compare("-ast") == 0)
			ast_switch = true;
		file_name = argv[3];
	}
	else if(argc == 3){
		if(((string) argv[1]).compare("-ast") == 0)
			ast_switch = true;
		file_name = argv[2];
	}

	string code_string = openFile(file_name);

	if(code_string.size() == 0)
		return 0;
	
	if(l_switch){
		cout << code_string << endl;
	}
	
	if(ast_switch){
		LexicalAnalyzer lex(code_string);
		lex.parse();

		TreeNode* root = lex.getTree();
		preOrder(root, "");

		TreeTransformer transformer;
		TreeNode* transformedRoot = transformer.transformTree(root);

		CSEMachine machine(transformedRoot);
		machine.evaluateTree();
	}else{
		cout << "How to run: p1 -ast location" << endl;
	}
	return 0;
}

void preOrder(TreeNode* t, std::string dots){
	formattedPrint(t->value,dots);
	string dots1 = "." + dots;
	if(t->left != NULL)
		preOrder(t->left,dots1);
	if(t->right != NULL)
		preOrder(t->right,dots);
}

void formattedPrint(Token t,std::string dots){
	if(t.type == "IDENTIFIER"){
		cout <<dots<<"<ID:"<< t.value<<'>'<<endl;
	}else if(t.type == "INTEGER"){
		cout <<dots<<"<INT:"<< t.value<<'>'<<endl;
	}else if(t.type == "STRING"){
		cout << dots<<"<STR:"<<t.value<<'>'<<endl;
	}else if(t.value == "true" or t.value == "false" or t.value == "nil" or t.value== "dummy"){
		cout << dots<<'<'<<t.value<<'>'<<endl;
	}else{
		cout <<dots<<t.value<<endl;
	}
}

