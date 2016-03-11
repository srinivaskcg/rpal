#include <iostream>
#include "Lexer.h"
#include "Token.h"
#include <string>
#include <set>
#include <cstdlib>
using namespace std;

Lexer::Lexer(string inputString) {
	this->inputString = inputString;
	this->size = inputString.size();
	this->presentVal = 0;
	this->currPtr = 0;
}

Token Lexer::getNextToken(){
	Token t = tokens[currPtr++];
	return t;
}

void Lexer::tokenizeStr()
{
	while(presentVal < size){

		Token token;
		
		char ch = inputString.at(presentVal++);
		//cout << ch << endl;

		if(isspace(ch) || ch == '\t' || ch == '\n'){
			continue;
		}else if(isalpha(ch)){
			token = tokenizeIdentifier(ch);
		}else if(isdigit(ch)){
			token = tokenizeInteger(ch);
		}else if (anOperator(ch)){
			token = tokenizeOperator(ch);
		}else if(ch == '\''){
			token = tokenizeString(ch);			
		}else if(ch == '(' || ch == ')' || ch == ';' || ch == ','){
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
			if(presentVal != size){
				ch = inputString.at(presentVal++);
				if(!isalpha(ch) && !isdigit(ch) && ch != '_'){
					presentVal--;
					break;
				}else{
					t.value +=ch;
				}
			}else{
				break;
			}
		}
		if(!aKeyword(t.value)){
			t.type = "IDENTIFIER";
		}else{
			t.type = "KEYWORD";
		}
	return t;
}

Token Lexer::tokenizeInteger(char ch){

	Token t;
	t.value +=ch;
	while(true){
		if(presentVal == size){
			break;
		}else{
			ch = inputString.at(presentVal++);
			if(!isdigit(ch)){
				presentVal--;
				break;
			}else{
				t.value +=ch;
			}
		}
	}
	t.type = "INT";
	return t;
}

Token Lexer::tokenizeString(char ch){

	Token t;
	t.value += ch;
	while(true){
		ch = inputString.at(presentVal++);
		if(ch == '\\'){
			char nextCh = inputString.at(presentVal++);
			if(nextCh =='t' || nextCh == 'n' || nextCh=='\\' || nextCh=='\''){
				t.value += ch;
				t.value += nextCh;
			}else{
				throw "Problem with creating <STRING> token";
			}
		}else if(ch == '\''){
			t.value += ch;
			t.type = "STRING";			
			break;
		}else if(isalpha(ch) || isdigit(ch) || anOperator(ch) || ch==')' || ch=='(' || ch==';' || ch==','
				|| ch == ' '){
			t.value += ch;
		}
	}
	return t;
}


Token Lexer::tokenizeOperator(char ch){
	
	Token t;

	if(ch == '/' && inputString.at(presentVal++) == '/'){
		while(true){
			ch = inputString.at(presentVal++);
			if(ch == '\n'){
				presentVal--;
				break;
			}else if(isalpha(ch) || isdigit(ch) || anOperator(ch) || ch == ' ' || ch=='\t'
					|| ch=='\'' || ch == '(' || ch==')' || ch==';' || ch==',' || ch=='\\'){
				continue;
			}
		}
		tokenizeStr();
	}else{
		if(ch == '/')
			presentVal--;
		t.value +=ch;
		while(true){
			if(presentVal == size){
				break;
			}else{
				ch = inputString.at(presentVal++);
				if(!anOperator(ch)){
					presentVal--;
					break;
				}else{
					t.value +=ch;
				}
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
	Token t = tokens[currPtr];
	return t;
}

bool Lexer::anOperator(char ch) {
  return operator_set.find(ch) != operator_set.end();
}

bool Lexer::aKeyword(string st) {
  return keyword_set.find(st) != keyword_set.end();
}

void Lexer::operator=(const Lexer& laObj){
	this->inputString = laObj.inputString;
	this->size = laObj.size;
	this->presentVal = laObj.presentVal;
}
