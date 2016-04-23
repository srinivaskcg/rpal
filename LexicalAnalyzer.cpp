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


LexicalAnalyzer::LexicalAnalyzer(string inputString) {
	this->inputString = inputString;
	this->size = inputString.size();
	this->presentVal = 0;
	this->currPtr = 0;
	this->tokensLeft = true;
	this->moreTokens = true;
}

LexicalAnalyzer::~LexicalAnalyzer() {

}

LexicalAnalyzer::LexicalAnalyzer(){

}

// Main method for Lexical Analysis and Recursive Descent Parser
void LexicalAnalyzer::parse(){
	//cout << "Inside Parse" << endl;
	tokenizeStr();
	//cout << "Tokenize completed" << endl;
	nextToken = getNextToken();
	//cout << "Next Token Retrieved" << endl;
	parseE();
	//printTree();
}

// Method to tokenize the string and generate tokens
void LexicalAnalyzer::tokenizeStr()
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

// Method to check the char is among the rpal keyword set
bool LexicalAnalyzer::aKeyword(string st) {
  return keyword_set.find(st) != keyword_set.end();
}

// Method to check the char is among the rpal operator set
bool LexicalAnalyzer::anOperator(char c) {
  return operator_set.find(c) != operator_set.end();
}

// Method to check the char is comment or tab or new line
bool LexicalAnalyzer::aComment(char c){
	if(c=='\\' || c=='\'' || c=='t' || c== 'n')
		return true;
	return false;
}

// Method to check the char is punctuation or alpha or digit or operator
bool LexicalAnalyzer::aPunctuation(char c){
	if(c=='(' || c==')' || c==';' || c==',' || c== ' ' || isalpha(c) || isdigit(c) || anOperator(c))
		return true;
	return false;
}

// Method for creating token if the char is identifier
Token LexicalAnalyzer::tokenizeIdentifier(char c){

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
Token LexicalAnalyzer::tokenizeString(char c){

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
Token LexicalAnalyzer::tokenizeOperator(char c){
	
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
		else{
			presentVal--;
		}
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
Token LexicalAnalyzer::tokenizeInteger(char c){

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
	t.type = "INTEGER";
	return t;
}

// Method for creating token if the char is punctuation
Token LexicalAnalyzer::tokenizePunctuation(char c){
	Token t;
	t.type = c;
	t.value = c;
	return t;
}

// Method to get the next token by moving pointer ahead
Token LexicalAnalyzer::getNextToken(){
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
Token LexicalAnalyzer::peekNextToken(){
	Token t = tokens[currPtr];
	return t;
}

string LexicalAnalyzer::ID = "IDENTIFIER";
string LexicalAnalyzer::STR = "STRING";
string LexicalAnalyzer::INT = "INTEGER";
string LexicalAnalyzer::KEY = "KEYWORD";
string LexicalAnalyzer::OPT = "OPERATOR";


void LexicalAnalyzer::read(Token token){

	if(token.value != nextToken.value)
		exit(1);

	if(token.type == "INTEGER" || token.type == "IDENTIFIER" || token.type == "STRING")
		buildTree(token,0);
	
	nextToken = getNextToken();
	
	if(nextToken.value == "END" || tokensLeft == false)
		throw "All tokens parsed";
}


void LexicalAnalyzer::buildTree(Token tokenVal, int popTreeCnt){
	TreeNode* tempNode = new TreeNode;
	//Token tempToken(tokenVal,tokenVal);

	tempNode->value = tokenVal;
	if(popTreeCnt != 0){
		while(!stk.empty() && popTreeCnt > 1){
			TreeNode* curr = stk.top();
			stk.pop();
			makeRightNode(curr);
			popTreeCnt--;
		}
		TreeNode* top = stk.top();
		stk.pop();
		if(top != NULL)
			tempNode->left = top;
	}
	stk.push(tempNode);
	return;
}

// Method to pop the top of the stack and make it as a 
// right node to its parent to make it binary
void LexicalAnalyzer::makeRightNode(TreeNode* node){
	TreeNode* parentNode = stk.top();
	stk.pop();
	parentNode->right = node;
	stk.push(parentNode);
}


void LexicalAnalyzer::parseE(){
	//cout<<"Inside parseE()"<<endl;
	//cout << nextToken.value << endl;
	if(nextToken.value == "let"){
		Token letToken("let",KEY);
		read(letToken);
		parseD();
		Token inToken("in",KEY);
		read(inToken);
		parseE();
		Token nodeToken("let","let");
		buildTree(nodeToken,2);
	} else if(nextToken.value == "fn"){
		Token fnToken("fn","fn");
		read(fnToken);
		int n=0;
		do{
			parseVb();
			n++;
		}while(nextToken.type == ID or nextToken.type == "(");
		Token dotToken(".",OPT);
		read(dotToken);
		parseE();
		Token nodeToken("lambda","lambda");
		buildTree(nodeToken,n+1);
	} else{
		parseEw();
	}
	//cout<<"End of parseE()"<<endl;
}

void LexicalAnalyzer::parseEw(){
	//cout<<"Inside parseEw()"<<endl;
	parseT();
	if(nextToken.value == "where"){
		Token t("where",KEY);
		read(t);
		parseDr();
		Token nodeToken("where","where");
		buildTree(nodeToken,2);
	}
	//cout<<"End of parseEw()"<<endl;
}



void LexicalAnalyzer::parseT(){
	//cout<<"Inside parseT()"<<endl;
	parseTa();
	if(nextToken.value == ","){
		int n = 0;
		do{
			read(nextToken);
			parseTa();
			n++;
		}while(nextToken.value == ",");
		Token tauToken("tau","tau");
		tauToken.tauCount = n+1;
		buildTree(tauToken,n+1);
	}
	//cout<<"End of parseT()"<<endl;
}

void LexicalAnalyzer::parseTa(){
	//cout<<"Inside parseTa()"<<endl;
	parseTc();
	while(nextToken.value == "aug"){
		Token temp = nextToken;
		read(nextToken);
		parseTc();
		buildTree(temp,2);
	}
	//cout<<"End of parseTa()"<<endl;
}

void LexicalAnalyzer::parseTc(){
	//cout<<"Inside parseTc()"<<endl;
	parseB();
	if(nextToken.value == "->"){
		read(nextToken);
		parseTc();
		Token elseToken("|",OPT);
		read(elseToken);
		parseTc();
		Token nodeToken("->","->");
		buildTree(nodeToken,3);
	}
	//cout<<"End of parseTc()"<<endl;
}

void LexicalAnalyzer::parseB(){
	//cout<<"Inside parseB()"<<endl;
	parseBt();
	while(nextToken.value == "or"){
		nextToken.type = LexicalAnalyzer::OPT;
		Token temp = nextToken;
		read(nextToken);
		parseBt();
		buildTree(temp,2);
	}
	//cout<<"End of parseB()"<<endl;
}

void LexicalAnalyzer::parseBt(){
	//cout<<"Inside parseBt()"<<endl;
	parseBs();
	while(nextToken.value == "&"){
		nextToken.type = LexicalAnalyzer::OPT;
		Token temp = nextToken;
		read(nextToken);
		parseBs();
		buildTree(temp,2);
	}
	//cout<<"End of parseBt()"<<endl;
}

void LexicalAnalyzer::parseBs(){
	//cout<<"Inside parseBs()"<<endl;
	if(nextToken.value == "not"){
		nextToken.type = "not";
		Token temp = nextToken;
		read(nextToken);
		parseBp();
		buildTree(temp,1);
	}else{
		parseBp();
	}
	//cout<<"End of parseBs()"<<endl;
}

void LexicalAnalyzer::parseBp(){
	//cout<<"Inside parseBp()"<<endl;
	parseA();
	if(nextToken.value=="gr" or nextToken.value == ">"){
		nextToken.type = LexicalAnalyzer::OPT;
		parseBpHelper(nextToken, "gr");
	}else if(nextToken.value == "ls" or nextToken.value == "<"){
		nextToken.type = LexicalAnalyzer::OPT;
		parseBpHelper(nextToken,"ls");
	}else if(nextToken.value =="ge" or nextToken.value == ">=" ){
		nextToken.type = LexicalAnalyzer::OPT;
		parseBpHelper(nextToken, "ge");
	}else if(nextToken.value =="le" or nextToken.value == "<=" ){
		nextToken.type = LexicalAnalyzer::OPT;
		parseBpHelper(nextToken, "le");
	}else if(nextToken.value =="eq" ){
		nextToken.type = LexicalAnalyzer::OPT;
		parseBpHelper(nextToken, "eq");
	}else if(nextToken.value =="ne"){
		nextToken.type = LexicalAnalyzer::OPT;
		parseBpHelper(nextToken, "ne");
	}
	//cout<<"End of parseBp()"<<endl;
}

void LexicalAnalyzer::parseBpHelper(Token nextToken, string tokenValue){
	//cout<<"Inside parseBpHelper()"<<endl;
	read(nextToken);
	parseA();
	Token t( tokenValue,LexicalAnalyzer::OPT);
	buildTree(t,2);
	//cout<<"End of parseBpHelper()"<<endl;
}

void LexicalAnalyzer::parseA(){
	//cout<<"Inside parseA()"<<endl;
	if(nextToken.value == "-"){
		Token negToken("-",OPT);
		read(negToken);
		parseAt();
		Token nodeToken("neg","neg");
		buildTree(nodeToken,1);
	}else if(nextToken.value == "+"){
		Token posToken("+",OPT);
		read(posToken);
		parseAt();
	}else{
		parseAt();
	}
	while(nextToken.value == "+" or nextToken.value == "-"){
		Token temp = nextToken;
		read(nextToken);
		parseAt();
		buildTree(temp,2);
	}
	//cout<<"End of parseA()"<<endl;
}

void LexicalAnalyzer::parseAt(){
	//cout<<"Inside parseAt()"<<endl;
	parseAf();
	while(nextToken.value == "*" or nextToken.value=="/"){
		Token temp = nextToken;
		read(nextToken);
		parseAf();
		buildTree(temp,2);
	}
	//cout<<"End of parseAt()"<<endl;
}

void LexicalAnalyzer::parseAf(){
	//cout<<"Inside parseAf()"<<endl;
	parseAp();
	while(nextToken.value == "**"){
		Token temp = nextToken;
		read(nextToken);
		parseAf();
		buildTree(temp,2);
	}
	//cout<<"End of parseAf()"<<endl;
}

void LexicalAnalyzer::parseAp(){
	//cout<<"Inside parseAp()"<<endl;
	parseR();
	while(nextToken.value == "@"){
		Token temp = nextToken;
		read(nextToken); //Read '@'
		if(nextToken.type != ID)
			throw "Expected Identifier found in parseAp()";
		read(nextToken); //Read identifier
		parseR();
		buildTree(temp,3);
	}
	//cout<<"End of parseAp()"<<endl;
}

void LexicalAnalyzer::parseR(){
	//cout<<"Inside parseR()"<<endl;
	parseRn();
	while(nextToken.type == ID or nextToken.type == STR or nextToken.type == INT or
			nextToken.value == "true" or nextToken.value == "false" or nextToken.value == "nil" or
			nextToken.value == "(" or nextToken.value == "dummy"){
		parseRn();
		Token nodeToken("gamma","gamma");
		buildTree(nodeToken,2);
	}
	//cout<<"End of parseR()"<<endl;
}

void LexicalAnalyzer::parseRn(){
	//cout<<"Inside parseRn()"<<endl;
	if(nextToken.type == ID or nextToken.type == STR or nextToken.type == INT ){
			read(nextToken);
	}else if(nextToken.value=="true"){
		parseRHelper(nextToken, "true");
	}else if(nextToken.value == "false"){
		parseRHelper(nextToken, "false");
	}else if(nextToken.value == "nil"){
		parseRHelper(nextToken, "nil");
	}else if(nextToken.value == "dummy"){
		parseRHelper(nextToken,"dummy");
	}else if(nextToken.value == "("){
		read(nextToken);
		parseE();
		Token t(")",")");
		read(t);
	}
	//cout<<"End of parseRn()"<<endl;
}

void LexicalAnalyzer::parseRHelper(Token t, string value){
	//cout<<"Inside parseRHelper()"<<endl;
	read(t);
	Token nodeToken(value,value);
	buildTree(nodeToken,0);
	//cout<<"End of parseRHelper()"<<endl;
}

void LexicalAnalyzer::parseD(){
	//cout<<"Inside parseD()"<<endl;
	parseDa();
	if(nextToken.value == "within"){
		read(nextToken);
		parseD();
		Token nodeToken("within", "within");
		buildTree(nodeToken,2);
	}
	//cout<<"End of parseD()"<<endl;
}

void LexicalAnalyzer::parseDa(){
	//cout<<"Inside parseDa()"<<endl;
	parseDr();
	if(nextToken.value== "and"){
		int n = 1;
		Token temp = nextToken;
		while(nextToken.value == "and"){
			read(nextToken);
			parseDr();
			n++;
		}
		buildTree(temp,n);
	}
	//cout<<"End of parseDa()"<<endl;
}

void LexicalAnalyzer::parseDr(){
	//cout<<"Inside parseDr()"<<endl;
	if(nextToken.value == "rec"){
		Token temp = nextToken;
		read(nextToken);
		parseDb();
		buildTree(temp,1);
	}else{
		parseDb();
	}
	//cout<<"End of parseDr()"<<endl;
}

void LexicalAnalyzer::parseDb(){
	//cout<<"Inside parseDb()"<<endl;
	//cout<<nextToken.type + " "+la.peekNextToken().value <<endl;
	//Check if the next token is V1
	if(nextToken.value == "("){
		read(nextToken);
		parseD();
		Token t(")",OPT);
		read(t);
	} else if(nextToken.type == ID && (peekNextToken().value == "," || peekNextToken().value == "=")){
		// if next token is '=' or ',' then rule is Vl '=' E
		parseV1();
		Token t("=",OPT);
		read(t);
		parseE();
		//cout<< "Return from E" << endl;
		Token nodeToken("=","=");
		buildTree(nodeToken,2);
		//cout<<"Tree build "<<endl;
	} else{// else rule is '<ID>' Vb+ '=' E
		read(nextToken);
		int n = 1;
		parseVb();
		while(nextToken.type == ID or nextToken.value== "("){
			n++;
			parseVb();
		}
		Token t("=",OPT);
		read(t);
		parseE();
		Token nodeToken("function_form", "function_form");
		buildTree(nodeToken, n+2);
	}
	//cout<<"End of parseDb()"<<endl;
}

void LexicalAnalyzer::parseVb(){
	//cout<<"Inside parseVb()"<<endl;
	if(nextToken.type == ID){
		read(nextToken);
	}else if(nextToken.value == "("){
		read(nextToken);
		if(nextToken.value == ")"){
			read(nextToken);
			Token nodeToken("()","()");
			buildTree(nodeToken,0);
		}else{
			parseV1();
			Token t(")",")");
			read(t);
		}
	}
	//cout<<"End of parseVb()"<<endl;
}

void LexicalAnalyzer::parseV1(){
	//cout<<"Inside parseV1()"<<endl;
	read(nextToken);
	int n =1;
	if(nextToken.value == ","){
		while(nextToken.value == ","){
			n++;
			read(nextToken); //Reading ,
			read(nextToken); //Reading ID
		}
		Token nodeToken(",",",");
		buildTree(nodeToken,n);
	}
	//cout<<"End of parseV1()"<<endl;
}


void LexicalAnalyzer::printTree(){
	//cout<<"stk size: "<<stk.getSize()<<endl;
	TreeNode* t = stk.top();
	stk.pop();
	stk.push(t);
	preOrder(t,std::string(""));
}


void LexicalAnalyzer::preOrder(TreeNode* t, std::string dots){
	formattedPrint(t->value,dots);
	string dots1 = "." + dots;
	if(t->left != NULL)
		preOrder(t->left,dots1);
	if(t->right != NULL)
		preOrder(t->right,dots);
}

void LexicalAnalyzer::formattedPrint(Token t,std::string dots){
	if(t.type == ID){
		//cout <<dots<<"<ID:"<< t.value<<'>'<<endl;
	}else if(t.type == INT){
		//cout <<dots<<"<INT:"<< t.value<<'>'<<endl;
	}else if(t.type == STR){
		//cout << dots<<"<STR:"<<t.value<<'>'<<endl;
	}else if(t.value == "true" or t.value == "false" or t.value == "nil" or t.value== "dummy"){
		//cout << dots<<'<'<<t.value<<'>'<<endl;
	}else{
		//cout <<dots<<t.value<<endl;
	}
}


TreeNode* LexicalAnalyzer::getTree(){
	TreeNode* s = stk.top();
	stk.pop();
	return s;
}

