#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "Lexer.h"
#include "Parser.h"

using namespace std;

string openFile(char* fileName){
	
	string file_content;
	string next_line;
	ifstream input_file;

	input_file.open(fileName);

	//cout << fileName << endl;
	
	if(input_file.fail())
		cerr << "Error: Problem opening input file";

	if(input_file.is_open()){
		while(input_file.good()){
			getline(input_file,next_line);
			file_content +=  next_line + "\n";
		}
	}
	//cout << file_content << endl;
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

	if(ast_switch){
		string code_string = openFile(file_name);
		if(code_string.size() == 0)
			return 0;

		if(l_switch){
			cout << code_string << endl;
		}

		Lexer la(code_string);
		Parser parser(la);
		parser.parse();
	}else{
		cout << "How to run: p1 -ast location" << endl;
	}
	return 0;
}