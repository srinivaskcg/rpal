#include <iostream>
#include "LexicalAnalyzer.h"
#include "Token.h"
#include <string>
#include <set>
#include <cstdlib>

using namespace std;

Lexer::Lexer(std::string codeString) {
	this->codeString = codeString;
	this->size = codeString.size();
	this->readPtr = 0;
	this->currPtr = 0;
}

Lexer::~Lexer() {

}

Token Lexer::getNextToken(){

	//cout << "Inside get next" << endl;
	//cout << currPtr << endl;

	Token t = tokens[currPtr++];
	//cout << t.value << "\t" << t.type << endl;
	return t;
}

void Lexer::tokenizeStr()
{

	//cout << "Inside Tokenizer" << endl;
	size = codeString.size();
	//cout << size << endl;
	//readPtr = 0;
	//If next character is a space, ignore
	//If next character is a letter, go to case: identifier
	//If next character is a digit, go to Integer
	//If next character is a operator go to operator
	//If next character is a //, go to comment section
	//Handle punctuation characters
	//Handle String if next character start with ''
	while(readPtr < size){
		//cout << "Intial ptr value "<< readPtr << endl;
		/*if(readPtr == size){
			cout << "No more tokens";
			break;
		}*/

		Token token;
		
		char ch = codeString.at(readPtr++);

		if(isspace(ch) or ch == '\t' or ch == '\n'){
			continue;
		}else if(isalpha(ch)){
			token = tokenizeIdentifier(ch);
		}else if(isdigit(ch)){
			token = tokenizeInteger(ch);
		}else if (isoperator(ch)){
			token = tokenizeOperator(ch);
		}else if(ch == '\''){
			token = tokenizeString(ch);			
		}else if(ch == '(' or ch == ')' or ch == ';' or ch == ','){
			token = tokenizePunctuation(ch);
		}
		tokens.push_back(token);
		//cout << token.type << "\t" << token.value << endl;
	}

}

Token Lexer::tokenizeIdentifier(char ch){

	Token t;
	t.value +=ch;
		while(true){
			if(readPtr != size){
				ch = codeString.at(readPtr++);
				if(isalpha(ch) || isdigit(ch) || ch == '_'){
					t.value +=ch;
				}else{
					readPtr--;
					break;
				}
			}else{
				break;
			}
		}
		if(isKeyword(t.value)){
			t.type = "KEYWORD";
		}else{
			t.type = "IDENTIFIER";
		}
	return t;
}

Token Lexer::tokenizeInteger(char ch){

	Token t;
	t.value +=ch;
	while(true){
		if(readPtr != size){
			ch = codeString.at(readPtr++);
			if(isdigit(ch)){
				t.value +=ch;
			}else{
				readPtr--;
				break;
			}
		}else{
			break;
		}
	}
	t.type = "INT";
	return t;
}

Token Lexer::tokenizeString(char ch){
	Token t;
	t.value += ch;
	while(true){
		ch = codeString.at(readPtr++);
		if(ch == '\\'){
			char ch1 = codeString.at(readPtr++);
			if(ch1 =='t' or ch1 == 'n' or ch1=='\\' or ch1=='\''){
				t.value += ch;
				t.value += ch1;
			}else{
				throw "Problem with creating <STRING> token";
			}
		}else if(ch == '\''){
			t.value +=ch;
			t.type ="STRING";
			//return token;
			
			break;
		} else if(isalpha(ch) or isdigit(ch) or isoperator(ch) or ch==')' or ch=='(' or ch==';' or ch==','
				or isspace(ch)){
			t.value +=ch;
		}
	}
	return t;
}


Token Lexer::tokenizeOperator(char ch){
	
	Token t;

	if(ch == '/' && codeString.at(readPtr++) == '/'){
	while(true){
		ch = codeString.at(readPtr++);
		if(ch == '\n'){
			readPtr--;
			break;
		}else if(isalpha(ch) || isdigit(ch) || isoperator(ch) || ch == ' ' || ch=='\t'
				|| ch=='\'' || ch == '(' || ch==')' || ch==';' || ch==',' || ch=='\\'){
			continue;
		}
	}
	tokenizeStr();
	}else{
		if(ch == '/')
			readPtr--;
		t.value +=ch;
		while(true){
			if(readPtr != size){
				ch = codeString.at(readPtr++);
				if(isoperator(ch)){
					t.value +=ch;
				}else{
					readPtr--;
					break;
				}
			}else{
				break;
			}
		}
		t.type = "OPERATOR";
	}
	return t;
}

Token Lexer::tokenizePunctuation(char ch){
	Token t;
	t.type = ch;
	t.value = ch;
	return t;
}

Token Lexer::peekNextToken(){
	//cout << "Inside peek" << "\t" << currPtr << "\t" << size << endl;

	//if(currPtr == size)
	//	throw "No more tokens";
	Token t = tokens[currPtr];
	/*int count = t.value.size();
	while(count-- !=0){
		readPtr--;
	}*/
	return t;
}


bool Lexer::isoperator(char ch){

	char char_set[] = {'+','-','*','<','>','&','.','@','/',':','=','-','|','$','!','#','%','^','_','[',']','{','}','"','`','?'};
	
	set<char> ops;   
	ops.insert(char_set, char_set+26);

	if(ops.count(ch) > 0)
		return true;

	return false;
}

bool Lexer::isKeyword(string tokenValue){
	
	string keyword_set[] = {"let","in","fn","where","aug","or","not","gr","ge","ls","le","eq","ne","true","false","nil","dummy",
							"within","and","rec","list"};

	set<string> keys;   
	keys.insert(keyword_set, keyword_set+21);

	if(keys.count(tokenValue) > 0)
		return true;

	return false;
}

void Lexer::operator=(const Lexer& laObj){
	this->codeString = laObj.codeString;
	this->size = laObj.size;
	this->readPtr = laObj.readPtr;
}

Lexer::Lexer(){

}
