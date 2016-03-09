/*
 * LexicalAnalyzer.cpp
 *
 *  Created on: Oct 14, 2011
 *      Author: saurabh Saurabh
 *      // UFID        : 21817195
 */
#include <iostream>
#include "LexicalAnalyzer.h"
#include "Token.h"
#include <string>
#include <cstdlib>

using namespace std;


LexicalAnalyzer::LexicalAnalyzer(std::string codeString) {
	this->codeString = codeString;
	this->size = codeString.size();
	this->readPtr = 0;
}

LexicalAnalyzer::~LexicalAnalyzer() {

}

Token LexicalAnalyzer::getNextToken()
{
	Token token;
	//If next character is a space, ignore
	//If next character is a letter, go to case: identifier
	//If next character is a digit, go to Integer
	//If next character is a operator go to operator
	//If next character is a //, go to comment section
	//Handle punctuation characters
	//Handle String if next character start with ''
	while(true){
		//cout << "Intial ptr value "<< readPtr;
		if(readPtr == size)
			throw "No more tokens";
		char ch = codeString.at(readPtr++);
		if(isspace(ch) or ch == '\t' or ch == '\n'){
			continue;
		}else if(isalpha(ch)){
			token.value +=ch;
			while(true){
				if(readPtr != size){
					ch = codeString.at(readPtr++);
					if(isalpha(ch) or isdigit(ch) or ch == '_'){
						token.value +=ch;
					}else{
						readPtr--;
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
		}else if(isdigit(ch)){
			token.value +=ch;
			while(true){
				if(readPtr != size){
					ch = codeString.at(readPtr++);
					if(isdigit(ch)){
						token.value +=ch;
					}else{
						readPtr--;
						break;
					}
				}else{
					break;
				}
			}
			token.type = "INTEGER";
		}else if (isoperator(ch)){
			if(ch == '/' && codeString.at(readPtr++) == '/'){
				while(true){
					ch = codeString.at(readPtr++);
					if(ch == '\n'){
						readPtr--;
						break;
					}else if(isalpha(ch) or isdigit(ch) or isoperator(ch) or isspace(ch) or ch=='\t'
							or ch=='\'' or ch == '(' or ch==')' or ch==';' or ch==',' or ch=='\\'){
						continue;
					}
				}
				continue;
			}else{
				if(ch == '/')
					readPtr--;
				token.value +=ch;
				while(true){
					if(readPtr != size){
						ch = codeString.at(readPtr++);
						if(isoperator(ch)){
							token.value +=ch;
						}else{
							readPtr--;
							break;
						}
					}else{
						break;
					}
				}
				token.type = "OPERATOR";
			}
		}else if(ch == '\''){
			token.value += ch;
			while(true){
				ch = codeString.at(readPtr++);
				if(ch == '\\'){
					char ch1 = codeString.at(readPtr++);
					if(ch1 =='t' or ch1 == 'n' or ch1=='\\' or ch1=='\''){
						token.value += ch;
						token.value += ch1;
					}else{
						throw "Problem with creating <STRING> token";
					}
				}else if(ch == '\''){
					token.value +=ch;
					token.type ="STRING";
					return token;
				} else if(isalpha(ch) or isdigit(ch) or isoperator(ch) or ch==')' or ch=='(' or ch==';' or ch==','
						or isspace(ch)){
					token.value +=ch;
				}
			}
		}else if(ch == '(' or ch == ')' or ch == ';' or ch == ','){
			token.type = ch;
			token.value = ch;
		}
		return token;
	}

}

Token LexicalAnalyzer::peekNextToken(){
	if(readPtr == size)
		throw "No more tokens";
	Token t = getNextToken();
	int count = t.value.size();
	while(count-- !=0){
		readPtr--;
	}
	return t;
}


bool LexicalAnalyzer::isoperator(char ch){
	char operators[] = {'+','-','*','<','>','&','.','@','/',':','=','-','|','$','!','#','%','^','_','[',']','{','}','"','`','?'};
	int i;
	for(i=0;i<26;i++){
		if(ch==operators[i])
			return true;
	}
	return false;
}


bool LexicalAnalyzer::isKeyword(std::string tokenValue){
	std::string keywords[] = {"let","where","aug","or","not","gr","ge","ls","le","eq","ne","true","false","nil","dummy","in","fn"
						,"within","and","rec","list"};
	int i;
	for(i=0;i<21;i++){
		if(tokenValue == keywords[i]){
			return true;
		}
	}
	return false;
}

void LexicalAnalyzer::operator=(const LexicalAnalyzer& laObj){
	this->codeString = laObj.codeString;
	this->size = laObj.size;
	this->readPtr = laObj.readPtr;
}

LexicalAnalyzer::LexicalAnalyzer(){

}
