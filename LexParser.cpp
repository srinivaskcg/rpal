//
//  LexParser.cpp
//
//  Created by Srinivas Gubbala on 3/5/16.
//  UFID - 2131 7376
//

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

#include "LexParser.h"
#include "Token.h"

using namespace std;

// Constructor 
LexParser::LexParser(string inputString) {
	this->inputString = inputString;
	this->size = inputString.size();
	this->presentVal = 0;
	this->currPtr = 0;
	this->tokensLeft = true;
}

// Main method for Lexical Analysis and Recursive Descent Parser
void LexParser::parse(){
	tokenizeStr();
	nt = getNextToken();
	E();
	printTree();
}

// Method to tokenize the string and generate tokens
void LexParser::tokenizeStr()
{
	while(presentVal < size){

		Token token;
		
		char c = inputString.at(presentVal++);
		//cout << c << endl;

		if(isspace(c) || c == '\t' || c == '\n'){
			continue;
		}else if(isalpha(c)){
			token = tokenizeIdentifier(c);
		}else if(isdigit(c)){
			token = tokenizeInteger(c);
		}else if (anOperator(c)){
			token = tokenizeOperator(c);
		}else if(c == '\''){
			token = tokenizeString(c);			
		}else if(c == '(' || c == ')' || c == ';' || c == ','){
			token = tokenizePunctuation(c);
		}
		tokens.push_back(token);
		//cout << token.type << "\t" << token.value << endl;
	}
}

// Rpal Phase structure grammar rules 

// ########################### Expressions ###########################

// E 	-> ’let’ D ’in’ E 
//		-> ’fn’  Vb+ ’.’ E
//		->  Ew;

void LexParser::E(){
	if(nt.value == "let"){
		Token letToken("let","KEYWORD");
		readToken(letToken);
		D();
		Token inToken("in","KEYWORD");
		readToken(inToken);
		E();
		treeBuilder("let",2);
	} else if(nt.value == "fn"){
		Token fnToken("fn","fn");
		readToken(fnToken);
		int n=1;
		while(nt.type == "IDENTIFIER" || nt.type == "("){
			Vb();
			n++;
		}
		Token dotToken(".","OPERATOR");
		readToken(dotToken);
		E();
		treeBuilder("lambda",n);
	} else{
		Ew();
	}
}

// Ew   -> T’where’ Dr  
//		-> T;

void LexParser::Ew(){
	T();
	if(nt.value == "where"){
		Token t("where","KEYWORD");
		readToken(t);
		Dr();
		treeBuilder("where",2);
	}
}

// ########################### Tuple Expressions ###########################

// T 	-> Ta(’,’ Ta )+ 
//		-> Ta ;

void LexParser::T(){
	Ta();
	if(nt.value == ","){
		int vars = 1;
		while(nt.value == ","){
			readToken(nt);
			Ta();
			vars++;
		}
		treeBuilder("tau",vars);
	}
}

// Ta   -> Ta ’aug’ Tc
//		-> Tc ;

void LexParser::Ta(){
	Tc();
	while(nt.value == "aug"){
		Token temp = nt;
		readToken(nt);
		Tc();
		treeBuilder("aug",2);
	}
}

// Tc   -> B’->’ Tc ’|’ Tc
//		-> B ;

void LexParser::Tc(){
	B();
	if(nt.value == "->"){
		readToken(nt);
		Tc();
		Token elseToken("|","OPERATOR");
		readToken(elseToken);
		Tc();
		treeBuilder("->",3);
	}
}

// ########################### Boolean Expressions ###########################

// B 	-> B ’or’ Bt
//		-> Bt ;

void LexParser::B(){
	Bt();
	while(nt.value == "or"){
		Token temp = nt;
		readToken(nt);
		Bt();
		treeBuilder("or",2);
	}
}

// Bt   -> Bt ’&’ Bs
//		-> Bs ;

void LexParser::Bt(){
	Bs();
	while(nt.value == "&"){
		Token temp = nt;
		readToken(nt);
		Bs();
		treeBuilder("&",2);
	}
}

// Bs   -> ’not’ Bp
//		-> Bp ;

void LexParser::Bs(){
	if(nt.value == "not"){
		Token temp = nt;
		readToken(nt);
		Bp();
		treeBuilder("not",1);
	}else{
		Bp();
	}
}

// Bp   -> A (’gr’ | ’>’ ) A
//		-> A (’ge’ | ’>=’) A  
//		-> A (’ls’ | ’<’ ) A 
//		-> A (’le’ | ’<=’) A
//		-> A ’eq’ A
//		-> A ’ne’ A
//		-> A ;

void LexParser::Bp(){
	A();
	if(nt.value=="gr" || nt.value == ">"){
		readToken(nt);
		A();
		treeBuilder("gr",2);
	}else if(nt.value == "ls" || nt.value == "<"){
		readToken(nt);
		A();
		treeBuilder("ls",2);
	}else if(nt.value =="ge" || nt.value == ">=" ){
		readToken(nt);
		A();
		treeBuilder("ge",2);
	}else if(nt.value =="le" || nt.value == "<=" ){
		readToken(nt);
		A();
		treeBuilder("le",2);
	}else if(nt.value =="eq" ){
		readToken(nt);
		A();
		treeBuilder("eq",2);
	}else if(nt.value =="ne"){
		readToken(nt);
		A();
		treeBuilder("ne",2);
	}
}

// ########################### Arithmetic Expressions ###########################

// At 	-> A’+’ At
//		-> A ’-’ At
//		->   ’+’ At
//		->   ’-’ At                                
//		-> At ;

void LexParser::A(){
	if(nt.value == "-"){
		Token negToken("-","OPERATOR");
		readToken(negToken);
		At();
		treeBuilder("neg",1);
	}else if(nt.value == "+"){
		Token posToken("+","OPERATOR");
		readToken(posToken);
		At();
	}else{
		At();
	}
	while(nt.value == "+" || nt.value == "-"){
		Token tmp = nt;
		readToken(nt);
		At();
		treeBuilder(tmp.value,2);
	}
}

// At   -> At ’*’ Af
//		-> At ’/’ Af
//		-> Af ;

void LexParser::At(){
	Af();
	while(nt.value == "*" || nt.value=="/"){
		Token temp = nt;
		readToken(nt);
		Af();
		treeBuilder(temp.value,2);
	}
}

// Af   -> Ap ’**’ Af
//		-> Ap ;

void LexParser::Af(){
	Ap();
	while(nt.value == "**"){
		Token temp = nt;
		readToken(nt);
		Af();
		treeBuilder(temp.value,2);
	}
}

// Ap   -> Ap ’@’ ’<IDENTIFIER>’ R 
//		-> R ;

void LexParser::Ap(){
	R();
	while(nt.value == "@"){
		Token temp = nt;
		readToken(nt); 
		if(nt.type != "IDENTIFIER")
			throw "Expected Identifier found in parseAp()";
		readToken(nt); 
		R();
		treeBuilder(temp.value,3);
	}
}

// ########################### Rators And Rands ###########################

// R 	->	R Rn
//		-> Rn ;

void LexParser::R(){
	Rn();
	while(nt.type == "IDENTIFIER" || nt.type == "STRING" || nt.type == "INT" ||
			nt.value == "true" || nt.value == "false" || nt.value == "nil" ||
			nt.value == "(" || nt.value == "dummy"){
		Rn();
		treeBuilder("gamma",2);
	}
}

// Rn   -> ’<IDENTIFIER>’
//		-> ’<INTEGER>’
//		-> ’<STRING>’
//		-> ’true’
//		-> ’false’
//		-> ’nil’
//		-> ’(’ E ’)’
//		-> ’dummy’

void LexParser::Rn(){
	if(nt.type == "IDENTIFIER" || nt.type == "STRING" || nt.type == "INT" ){
		readToken(nt);
	}else if(nt.value=="true"){
		readToken(nt);
		treeBuilder("true",0);
	}else if(nt.value == "false"){
		readToken(nt);
		treeBuilder("false",0);
	}else if(nt.value == "nil"){
		readToken(nt);
		treeBuilder("nil",0);
	}else if(nt.value == "dummy"){
		readToken(nt);
		treeBuilder("dummy",0);
	}else if(nt.value == "("){
		readToken(nt);
		E();
		Token t(")",")");
		readToken(t);
	}
}

// ########################### Definitions ###########################

// D -> Da ’within’ D
//	 -> Da ;

void LexParser::D(){
	Da();
	if(nt.value == "within"){
		readToken(nt);
		D();
		treeBuilder("within",2);
	}
}

// Da -> Dr ( ’and’ Dr )+ 
//	  -> Dr ;

void LexParser::Da(){
	Dr();
	if(nt.value== "and"){
		int n = 0;
		Token temp = nt;
		do{
			readToken(nt);
			Dr();
			n++;
		}while(nt.value == "and");
		treeBuilder(temp.value,n+1);
	}
}

// Dr -> ’rec’ Db
//    -> Db ;

void LexParser::Dr(){
	if(nt.value != "rec"){
		Db();
	}else{
		Token temp = nt;
		readToken(nt);
		Db();
		treeBuilder(temp.value,1);
	}
}

// Db -> Vl ’=’ E
//	  -> ’<IDENTIFIER>’ Vb+ ’=’ E
//	  -> ’(’ D ’)’ ;

void LexParser::Db(){
	if(nt.value == "("){
		readToken(nt);
		D();
		Token t(")","OPERATOR");
		readToken(t);
	} else if(nt.type == "IDENTIFIER" && (peekNextToken().value == "," || peekNextToken().value == "=")){
		Vl();
		Token t("=","OPERATOR");
		readToken(t);
		E();
		treeBuilder("=",2);
	} else{
		readToken(nt);
		int chars = 1;
		Vb();
		while(nt.type == "IDENTIFIER" || nt.value== "("){
			chars++;
			Vb();
		}
		Token t("=","OPERATOR");
		readToken(t);
		E();
		treeBuilder("function_form", chars+2);
	}
}

// ########################### Variables ###########################

// Vb -> ’<IDENTIFIER>’
//    -> ’(’ Vl ’)’
//    -> ’(’ ’)’

void LexParser::Vb(){
	if(nt.type == "IDENTIFIER"){
		readToken(nt);
	}else if(nt.value == "("){
		readToken(nt);
		if(nt.value == ")"){
			readToken(nt);
			treeBuilder("()",0);
		}else{
			Vl();
			Token t(")",")");
			readToken(t);
		}
	}
}

// Vl -> ’<IDENTIFIER>’ list ’,’

void LexParser::Vl(){
	readToken(nt);
	if(nt.value == ","){
		int chars =0;
		do{
			chars++;
			readToken(nt); 
			readToken(nt); 
		}
		while(nt.value == ",");
		treeBuilder(",",chars+1);
	}
}

// Method to build the tree and put into stack of trees
void LexParser::treeBuilder(string tokenVal, int popTreeCnt){
	Node* tempNode = new Node;
	Token tempToken(tokenVal,tokenVal);

	tempNode->value = tempToken;
	if(popTreeCnt != 0){
		while(!trees.empty() && popTreeCnt > 1){
			Node* curr = trees.top();
			trees.pop();
			makeRightNode(curr);
			popTreeCnt--;
		}
		Node* top = trees.top();
		trees.pop();
		if(top != NULL)
			tempNode->left = top;
	}
	trees.push(tempNode);
	return;
}

// Method to pretty print the tree
void LexParser::printTree(){
	while(!trees.empty()){
		Node* t = trees.top();
		trees.pop();
		preOrdTraversal(t,string(""));	
	}
}

// Helper methods

// Method to check the char is among the rpal keyword set
bool LexParser::aKeyword(string st) {
  return keyword_set.find(st) != keyword_set.end();
}

// Method to check the char is among the rpal operator set
bool LexParser::anOperator(char c) {
  return operator_set.find(c) != operator_set.end();
}

// Method to check the char is comment or tab or new line
bool LexParser::aComment(char c){
	if(c=='\\' || c=='\'' || c=='t' || c== 'n')
		return true;
	return false;
}

// Method to check the char is punctuation or alpha or digit or operator
bool LexParser::aPunctuation(char c){
	if(c=='(' || c==')' || c==';' || c==',' || c== ' ' || isalpha(c) || isdigit(c) || anOperator(c))
		return true;
	return false;
}

// Method for creating token if the char is identifier
Token LexParser::tokenizeIdentifier(char c){

	Token t;
	t.value += c;
		while(true){
			if(presentVal != size){
				c = inputString.at(presentVal++);
				if(!isalpha(c) && !isdigit(c) && c != '_'){
					presentVal--;
					break;
				}else{
					t.value += c;
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

// Method for creating token if the char is string start
Token LexParser::tokenizeString(char c){

	Token t;
	t.value = c;
	while(true){
		c = inputString.at(presentVal++);
		
		if(aPunctuation(c)){
			t.value += c;
		}
		else if(c == '\''){
			t.type = "STRING";
			t.value += c;
			break;
		}
		else if(c == '\\'){
			char nextc = inputString.at(presentVal++);
			if(aComment(nextc)){
				t.value += c;
				t.value += nextc;
			}else{
				throw "unable to create string token";
			}
		}
	}
	return t;
}

// Method for creating token if the char is operator
Token LexParser::tokenizeOperator(char c){
	
	Token t;

	if(c == '/'){
		if(inputString.at(presentVal++) == '/'){
			while(true){
				c = inputString.at(presentVal++);
				
				if(anOperator(c) || aPunctuation(c) || aComment(c)){
					continue;
				}
				else if(c == '\n'){
					presentVal--;
					break;
				}
			}
			tokenizeStr();
		}
		else
			presentVal--;
	}
	t.value +=c;
	while(true){
		if(presentVal == size){
			break;
		}else{
			c = inputString.at(presentVal++);
			if(!anOperator(c)){
				presentVal--;
				break;
			}else{
				t.value +=c;
			}
		}
	}
	t.type = "OPERATOR";
	
	return t;
}

// Method for creating token if the char is integer
Token LexParser::tokenizeInteger(char c){

	Token t;
	t.value += c;
	while(true){
		if(presentVal == size){
			break;
		}else{
			c = inputString.at(presentVal++);
			if(!isdigit(c)){
				presentVal--;
				break;
			}else{
				t.value +=c;
			}
		}
	}
	t.type = "INT";
	return t;
}

// Method for creating token if the char is punctuation
Token LexParser::tokenizePunctuation(char c){
	Token t;
	t.type = c;
	t.value = c;
	return t;
}

// Method to get the next token by moving pointer ahead
Token LexParser::getNextToken(){
	Token t;
	//cout << currPtr << "-" << tokens.size() << endl;
	if(currPtr < tokens.size())
		t = tokens[currPtr++];
	else
		Token t("End", "End");
	//cout << t.value << endl;
	return t;
}

// Method to peek the token from the vector
Token LexParser::peekNextToken(){
	Token t = tokens[currPtr];
	return t;
}

// Method to read the token and mark it based on type
void LexParser::readToken(Token token){

	if(token.value != nt.value)
		exit(1);

	if(token.type == "INT")
		treeBuilder("<INT:" + token.value + ">", 0);	
	else if(token.type == "IDENTIFIER")
		treeBuilder("<ID:" + token.value + ">", 0);
	else if(token.type == "STRING")
		treeBuilder("<STR:" + token.value + ">", 0);
	
	nt = getNextToken();
	
	if(nt.value == "END" || tokensLeft == false)
		throw "All tokens parsed";
}

// Method to pop the top of the stack and make it as a 
// right node to its parent to make it binary
void LexParser::makeRightNode(Node* node){
	Node* parentNode = trees.top();
	trees.pop();
	parentNode->right = node;
	trees.push(parentNode);
}

// Method to traverse the tree in PreOrder
void LexParser::preOrdTraversal(Node* t, string level){
	formattedPrint(t->value,level);
	if(t->left != NULL)
		preOrdTraversal(t->left, "." + level);
	if(t->right != NULL)
		preOrdTraversal(t->right,level);
}

// Method to print the tree with dots based on level of the nodes
void LexParser::formattedPrint(Token t,string level){
	if(t.value != "true" && t.value != "false" && t.value != "nil" && t.value != "dummy"){
		cout << level << t.value << endl;
	}else{
		cout << level << '<' << t.value << '>' << endl;
	}
}
