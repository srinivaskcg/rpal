#include <iostream>
#include "LexParser.h"
#include "Token.h"
#include <string>
#include <set>
#include <cstdlib>
using namespace std;

LexParser::LexParser(string inputString) {
	this->inputString = inputString;
	this->size = inputString.size();
	this->presentVal = 0;
	this->currPtr = 0;
	this->moreTokens = true;
}

void LexParser::parse(){
	tokenizeStr();
	nextToken = getNextToken();
	E();
	printTree();
}

void LexParser::tokenizeStr()
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

Token LexParser::tokenizeIdentifier(char ch){

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

Token LexParser::tokenizeInteger(char ch){

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

Token LexParser::tokenizeString(char ch){

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


Token LexParser::tokenizeOperator(char ch){
	
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

Token LexParser::tokenizePunctuation(char ch){
	Token t;
	t.type = ch;
	t.value = ch;
	return t;
}

Token LexParser::getNextToken(){
	Token t = tokens[currPtr++];
	return t;
}

Token LexParser::peekNextToken(){
	Token t = tokens[currPtr];
	return t;
}

bool LexParser::anOperator(char ch) {
  return operator_set.find(ch) != operator_set.end();
}

bool LexParser::aKeyword(string st) {
  return keyword_set.find(st) != keyword_set.end();
}

void LexParser::addRightChild(Node* node){
	Node* parentNode = trees.top();
	trees.pop();
	parentNode->right = node;
	trees.push(parentNode);
}

void LexParser::readToken(Token token){

	//cout << "Inside read" << "\t" <<token.type << "\t" << token.value << "\t" << nextToken.value<< endl;
	if(moreTokens == false)
		throw "Error: No more token to parse";
	if(token.value != nextToken.value){
		exit(1);
	}
	if(token.type == "IDENTIFIER"){
		treeBuilder("<ID:" + token.value + ">", 0);
	}
	else if(token.type == "STRING"){
		treeBuilder("<STR:" + token.value + ">", 0);
	} 
	else if(token.type == "INT"){
		treeBuilder("<INT:" + token.value + ">", 0);	
	}
	try{
		if(currPtr == tokens.size())
			throw "No more tokens";
		nextToken = getNextToken();
	}catch(const char* message){
		moreTokens = false;
		Token endToken("$$","$$");
		nextToken = endToken;
	}
}

void LexParser::treeBuilder(string tokenVal, int popTreeCnt){
	Node* tempNode = new Node;
	Token tempToken(tokenVal,tokenVal);

	tempNode->value = tempToken;
	if(popTreeCnt != 0){
		while(!trees.empty() && popTreeCnt > 1){
			Node* curr = trees.top();
			trees.pop();
			addRightChild(curr);
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

void LexParser::E(){
	if(nextToken.value == "let"){
		Token letToken("let","KEYWORD");
		readToken(letToken);
		D();
		Token inToken("in","KEYWORD");
		readToken(inToken);
		E();
		treeBuilder("let",2);
	} else if(nextToken.value == "fn"){
		Token fnToken("fn","fn");
		readToken(fnToken);
		int n=0;
		do{
			Vb();
			n++;
		}while(nextToken.type == "IDENTIFIER" || nextToken.type == "(");
		Token dotToken(".","OPERATOR");
		readToken(dotToken);
		E();
		treeBuilder("lambda",n+1);
	} else{
		Ew();
	}
}

void LexParser::Ew(){
	T();
	if(nextToken.value == "where"){
		Token t("where","KEYWORD");
		readToken(t);
		Dr();
		treeBuilder("where",2);
	}
}

void LexParser::T(){
	Ta();
	if(nextToken.value == ","){
		int n = 0;
		do{
			readToken(nextToken);
			Ta();
			n++;
		}while(nextToken.value == ",");
		treeBuilder("tau",n+1);
	}
}

void LexParser::Ta(){
	Tc();
	while(nextToken.value == "aug"){
		Token temp = nextToken;
		readToken(nextToken);
		Tc();
		treeBuilder("aug",2);
	}
}

void LexParser::Tc(){
	B();
	if(nextToken.value == "->"){
		readToken(nextToken);
		Tc();
		Token elseToken("|","OPERATOR");
		readToken(elseToken);
		Tc();
		treeBuilder("->",3);
	}
}

void LexParser::B(){
	Bt();
	while(nextToken.value == "or"){
		Token temp = nextToken;
		readToken(nextToken);
		Bt();
		treeBuilder("or",2);
	}
}

void LexParser::Bt(){
	Bs();
	while(nextToken.value == "&"){
		Token temp = nextToken;
		readToken(nextToken);
		Bs();
		treeBuilder("&",2);
	}
}

void LexParser::Bs(){
	if(nextToken.value == "not"){
		Token temp = nextToken;
		readToken(nextToken);
		Bp();
		treeBuilder("not",1);
	}else{
		Bp();
	}
}

void LexParser::Bp(){
	A();
	if(nextToken.value=="gr" || nextToken.value == ">"){
		readToken(nextToken);
		A();
		treeBuilder("gr",2);
	}else if(nextToken.value == "ls" || nextToken.value == "<"){
		readToken(nextToken);
		A();
		treeBuilder("ls",2);
	}else if(nextToken.value =="ge" || nextToken.value == ">=" ){
		readToken(nextToken);
		A();
		treeBuilder("ge",2);
	}else if(nextToken.value =="le" || nextToken.value == "<=" ){
		readToken(nextToken);
		A();
		treeBuilder("le",2);
	}else if(nextToken.value =="eq" ){
		readToken(nextToken);
		A();
		treeBuilder("eq",2);
	}else if(nextToken.value =="ne"){
		readToken(nextToken);
		A();
		treeBuilder("ne",2);
	}
}

void LexParser::A(){
	if(nextToken.value == "-"){
		Token negToken("-","OPERATOR");
		readToken(negToken);
		At();
		treeBuilder("neg",1);
	}else if(nextToken.value == "+"){
		Token posToken("+","OPERATOR");
		readToken(posToken);
		At();
	}else{
		At();
	}
	while(nextToken.value == "+" || nextToken.value == "-"){
		Token tmp = nextToken;
		readToken(nextToken);
		At();
		treeBuilder(tmp.value,2);
	}
}

void LexParser::At(){
	Af();
	while(nextToken.value == "*" || nextToken.value=="/"){
		Token temp = nextToken;
		readToken(nextToken);
		Af();
		treeBuilder(temp.value,2);
	}
}

void LexParser::Af(){
	Ap();
	while(nextToken.value == "**"){
		Token temp = nextToken;
		readToken(nextToken);
		Af();
		treeBuilder(temp.value,2);
	}
}

void LexParser::Ap(){
	R();
	while(nextToken.value == "@"){
		Token temp = nextToken;
		readToken(nextToken); 
		if(nextToken.type != "IDENTIFIER")
			throw "Expected Identifier found in parseAp()";
		readToken(nextToken); 
		R();
		treeBuilder(temp.value,3);
	}
}

void LexParser::R(){
	Rn();
	while(nextToken.type == "IDENTIFIER" || nextToken.type == "STRING" || nextToken.type == "INT" ||
			nextToken.value == "true" || nextToken.value == "false" || nextToken.value == "nil" ||
			nextToken.value == "(" || nextToken.value == "dummy"){
		Rn();
		treeBuilder("gamma",2);
	}
}

void LexParser::Rn(){
	if(nextToken.type == "IDENTIFIER" || nextToken.type == "STRING" || nextToken.type == "INT" ){
		readToken(nextToken);
	}else if(nextToken.value=="true"){
		readToken(nextToken);
		treeBuilder("true",0);
	}else if(nextToken.value == "false"){
		readToken(nextToken);
		treeBuilder("false",0);
	}else if(nextToken.value == "nil"){
		readToken(nextToken);
		treeBuilder("nil",0);
	}else if(nextToken.value == "dummy"){
		readToken(nextToken);
		treeBuilder("dummy",0);
	}else if(nextToken.value == "("){
		readToken(nextToken);
		E();
		Token t(")",")");
		readToken(t);
	}
}

void LexParser::D(){
	Da();
	if(nextToken.value == "within"){
		readToken(nextToken);
		D();
		treeBuilder("within",2);
	}
}

void LexParser::Da(){
	Dr();
	if(nextToken.value== "and"){
		int n = 0;
		Token temp = nextToken;
		do{
			readToken(nextToken);
			Dr();
			n++;
		}while(nextToken.value == "and");
		treeBuilder(temp.value,n+1);
	}
}

void LexParser::Dr(){
	if(nextToken.value != "rec"){
		Db();
	}else{
		Token temp = nextToken;
		readToken(nextToken);
		Db();
		treeBuilder(temp.value,1);
	}
}

void LexParser::Db(){
	if(nextToken.value == "("){
		readToken(nextToken);
		D();
		Token t(")","OPERATOR");
		readToken(t);
	} else if(nextToken.type == "IDENTIFIER" && (peekNextToken().value == "," || peekNextToken().value == "=")){
		Vl();
		Token t("=","OPERATOR");
		readToken(t);
		E();
		treeBuilder("=",2);
	} else{
		readToken(nextToken);
		int chars = 1;
		Vb();
		while(nextToken.type == "IDENTIFIER" || nextToken.value== "("){
			chars++;
			Vb();
		}
		Token t("=","OPERATOR");
		readToken(t);
		E();
		treeBuilder("function_form", chars+2);
	}
}

void LexParser::Vb(){
	if(nextToken.type == "IDENTIFIER"){
		readToken(nextToken);
	}else if(nextToken.value == "("){
		readToken(nextToken);
		if(nextToken.value == ")"){
			readToken(nextToken);
			treeBuilder("()",0);
		}else{
			Vl();
			Token t(")",")");
			readToken(t);
		}
	}
}

void LexParser::Vl(){
	readToken(nextToken);
	if(nextToken.value == ","){
		int chars =0;
		do{
			chars++;
			readToken(nextToken); 
			readToken(nextToken); 
		}
		while(nextToken.value == ",");
		treeBuilder(",",chars+1);
	}
}

void LexParser::printTree(){
	while(!trees.empty()){
		Node* t = trees.top();
		trees.pop();
		preOrdTraversal(t,string(""));	
	}
}

void LexParser::preOrdTraversal(Node* t, string dots){
	formattedPrint(t->value,dots);
	if(t->left != NULL)
		preOrdTraversal(t->left, "." + dots);
	if(t->right != NULL)
		preOrdTraversal(t->right,dots);
}

void LexParser::formattedPrint(Token t,string dots){
	if(t.value != "true" && t.value != "false" && t.value != "nil" && t.value != "dummy"){
		cout << dots << t.value << endl;
	}else{
		cout << dots << '<' << t.value << '>' << endl;
	}
}