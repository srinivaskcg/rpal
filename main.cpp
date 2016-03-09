#include <iostream>
#include <fstream>
#include <set>
#include <vector>

#include <string.h>
#include "header.h"

using namespace std;

string openFile(char* fileName){
	
	string fileContent;
	string nextLine;

	ifstream inputFile;

	inputFile.open(fileName);
	
	if(inputFile.fail())
		cerr << "Error: Problem opening input file";

	if(inputFile.is_open()){
		while(inputFile.good()){
			getline(inputFile,nextLine);
			fileContent +=  nextLine;
		}
	}
	return fileContent;
}

// Token Class

Token::Token(string value, string type){
    this->value = value;
    this->type = type;
}


//Lexical Analyzer class

bool Lexer::isOperator(char ch){

	char char_set[] = {'+','-','*','<','>','&','.','@','/',':','=','-','|','$','!','#','%','^','_','[',']','{','}','"','`','?'};
	
	set<char> ops;   
	ops.insert(char_set, char_set+26);

	if(ops.count(ch) > 0)
		return true;

	return false;
}

bool Lexer::isKeyword(std::string tokenValue){
	
	string keyword_set[] = {"let","in","fn","where","aug","or","not","gr","ge","ls","le","eq","ne","true","false","nil","dummy",
							"within","and","rec","list"};

	set<string> keys;   
	keys.insert(keyword_set, keyword_set+21);

	if(keys.count(tokenValue) > 0)
		return true;

	return false;
}

void Lexer::tokenizeString(string contentStr){

	int currPosition = 0;
	int size = contentStr.size();

	vector<Token> tokens;

	cout << "Intial currPosition value " << currPosition << endl;
	cout << "size" << size << endl;
	
	while(true){

		Token token;
		
		if(currPosition == size)
			cout << "No more tokens";
		
		char ch = contentStr.at(currPosition++);

		if(isspace(ch) or ch == '\t' or ch == '\n'){
			continue;
		}
		else if(isalpha(ch)){
			token.value +=ch;
			while(true){
				if(currPosition != size){
					ch = contentStr.at(currPosition++);
					if(isalpha(ch) or isdigit(ch) or ch == '_'){
						token.value +=ch;
					}else{
						currPosition--;
						break;
					}
				}else{
					break;
				}
			}
			if(isKeyword(token.value)){
				token.type = "KEYWORD";
			}else{
				token.type = "IDENTIFIER";
			}
		}
		else if(isdigit(ch)){
			token.value +=ch;
			while(true){
				if(currPosition != size){
					ch = contentStr.at(currPosition++);
					if(isdigit(ch)){
						token.value +=ch;
					}else{
						currPosition--;
						break;
					}
				}else{
					break;
				}
			}
			token.type = "INTEGER";
		}
		else if (isOperator(ch)){
			if(ch == '/' && contentStr.at(currPosition++) == '/'){
				while(true){
					ch = contentStr.at(currPosition++);
					if(ch == '\n'){
						currPosition--;
						break;
					}else if(isalpha(ch) or isdigit(ch) or isOperator(ch) or isspace(ch) or ch=='\t'
							or ch=='\'' or ch == '(' or ch==')' or ch==';' or ch==',' or ch=='\\'){
						continue;
					}
				}
				continue;
			}else{
				if(ch == '/')
					currPosition--;
				token.value +=ch;
				while(true){
					if(currPosition != size){
						ch = contentStr.at(currPosition++);
						if(isOperator(ch)){
							token.value +=ch;
						}else{
							currPosition--;
							break;
						}
					}else{
						break;
					}
				}
				token.type = "OPERATOR";
			}
		}
		else if(ch == '\''){
			token.value += ch;
			while(true){
				ch = contentStr.at(currPosition++);
				if(ch == '\\'){
					char ch1 = contentStr.at(currPosition++);
					if(ch1 =='t' or ch1 == 'n' or ch1=='\\' or ch1=='\''){
						token.value += ch;
						token.value += ch1;
					}else{
						throw "Problem with creating <STRING> token";
					}
				}else if(ch == '\''){
					token.value +=ch;
					token.type ="STRING";
					tokens.push_back(token);
					break;
				} else if(isalpha(ch) or isdigit(ch) or isOperator(ch) or ch==')' or ch=='(' or ch==';' or ch==','
						or isspace(ch)){
					token.value +=ch;
				}
			}
		}
		else if(ch == '(' or ch == ')' or ch == ';' or ch == ','){
			token.type = ch;
			token.value = ch;
		}
		tokens.push_back(token);
		cout << token.value <<endl;
		cout << token.type <<endl;
		cout << "currPosition " << currPosition << endl;
		cout << "-----------" <<endl;

	}
}

// Recursive Descent Parser class

Parser::Parser(string parseString){//, bool isAst, bool isListing){

//	if(isListing){
	cout << "Inside parser" << endl;
		lex.tokenizeString(parseString);
//	}

}

int main(int argc, char* argv[]) {

	cout << "Started !!!" << endl;

	char* input;

	//if(argc == 1){
		input = argv[1];	
		cout << input << endl;
	//}

	string codeString = openFile(input);
	cout << codeString;
	cout << codeString.size();

	Parser p(codeString);

}