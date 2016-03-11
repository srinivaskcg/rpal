#include <iostream>
#include "RecursiveParser.h"
#include <stack>
#include "Token.h"

using namespace std;

string Parser::ID = "IDENTIFIER";
string Parser::STR = "STRING";
string Parser::INT = "INT";
string Parser::KEY = "KEYWORD";
string Parser::OPT = "OPERATOR";

Parser::Parser(Lexer la){
	this->la = la;
	this->moreTokens = true;
}

Parser::~Parser(){

}

void Parser::addRightChild(Node* node){
	Node* parentNode = trees.top();
	trees.pop();
	parentNode->right = node;
	trees.push(parentNode);
}

void Parser::readToken(Token token){

	//cout << "Inside read" << "\t" <<token.type << "\t" << token.value << "\t" << nextToken.value<< endl;
	if(moreTokens == false)
		throw "Error: No more token to parse";
	if(token.value != nextToken.value){
		//cout << "Expected "<<token.value << " but found" <<nextToken.value;
		exit(1);
	}
	if(token.type == ID){
		treeBuilder("<ID:" + token.value + ">", 0);
	}
	else if(token.type == STR){
		treeBuilder("<STR:" + token.value + ">", 0);
	} 
	else if(token.type == INT){
		treeBuilder("<INT:" + token.value + ">", 0);	
	}
	try{
		//cout << la.currPtr << "\t" << la.tokens.size() << endl;
		if(la.currPtr == la.tokens.size())
			throw "No more tokens";

		nextToken = la.getNextToken();
	}catch(const char* message){
		moreTokens = false;
		Token endToken("$$","$$");
		nextToken = endToken;
	}
}


void Parser::treeBuilder(string tokenVal, int popTreeCnt){
	//cout<< "Inside treeBuilder "<< token.value<<endl;
	//cout<< "Stack size:"<<stack.getSize()<<endl;
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


void Parser::E(){
	//cout<<"Inside E()"<<endl;
	if(nextToken.value == "let"){
		Token letToken("let",KEY);
		//cout << "In E" << endl;
		readToken(letToken);
		D();
		Token inToken("in",KEY);
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
		}while(nextToken.type == ID || nextToken.type == "(");
		Token dotToken(".",OPT);
		readToken(dotToken);
		E();
		treeBuilder("lambda",n+1);
	} else{
		Ew();
	}
	//cout<<"End of E()"<<endl;
}

void Parser::Ew(){
	//cout<<"Inside Ew()"<<endl;
	T();
	if(nextToken.value == "where"){
		Token t("where",KEY);
		readToken(t);
		Dr();
		treeBuilder("where",2);
	}
	//cout<<"End of Ew()"<<endl;
}

void Parser::T(){
	//cout<<"Inside parseT()"<<endl;
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
	//cout<<"End of parseT()"<<endl;
}

void Parser::Ta(){
	//cout<<"Inside parseTa()"<<endl;
	Tc();
	while(nextToken.value == "aug"){
		Token temp = nextToken;
		readToken(nextToken);
		Tc();
		treeBuilder("aug",2);
	}
	//cout<<"End of parseTa()"<<endl;
}

void Parser::Tc(){
	//cout<<"Inside parseTc()"<<endl;
	B();
	if(nextToken.value == "->"){
		readToken(nextToken);
		Tc();
		Token elseToken("|",OPT);
		readToken(elseToken);
		Tc();
		treeBuilder("->",3);
	}
	//cout<<"End of parseTc()"<<endl;
}

void Parser::B(){
	//cout<<"Inside parseB()"<<endl;
	Bt();
	while(nextToken.value == "or"){
		Token temp = nextToken;
		readToken(nextToken);
		Bt();
		treeBuilder("or",2);
	}
	//cout<<"End of parseB()"<<endl;
}

void Parser::Bt(){
	//cout<<"Inside parseBt()"<<endl;
	Bs();
	while(nextToken.value == "&"){
		Token temp = nextToken;
		readToken(nextToken);
		Bs();
		treeBuilder("&",2);
	}
	//cout<<"End of parseBt()"<<endl;
}

void Parser::Bs(){
	//cout<<"Inside parseBs()"<<endl;
	if(nextToken.value == "not"){
		Token temp = nextToken;
		readToken(nextToken);
		Bp();
		treeBuilder("not",1);
	}else{
		Bp();
	}
	//cout<<"End of parseBs()"<<endl;
}

void Parser::Bp(){
	//cout<<"Inside parseBp()"<<endl;
	A();
	if(nextToken.value=="gr" or nextToken.value == ">"){
		readToken(nextToken);
		A();
		treeBuilder("gr",2);
	}else if(nextToken.value == "ls" or nextToken.value == "<"){
		readToken(nextToken);
		A();
		treeBuilder("ls",2);
	}else if(nextToken.value =="ge" or nextToken.value == ">=" ){
		readToken(nextToken);
		A();
		treeBuilder("ge",2);
	}else if(nextToken.value =="le" or nextToken.value == "<=" ){
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
	//cout<<"End of parseBp()"<<endl;
}

void Parser::A(){
	//cout<<"Inside parseA()"<<endl;
	if(nextToken.value == "-"){
		Token negToken("-",OPT);
		readToken(negToken);
		At();
		treeBuilder("neg",1);
	}else if(nextToken.value == "+"){
		Token posToken("+",OPT);
		readToken(posToken);
		At();
	}else{
		At();
	}
	while(nextToken.value == "+" or nextToken.value == "-"){
		Token temp = nextToken;
		readToken(nextToken);
		At();
		treeBuilder(temp.value,2);
	}
	//cout<<"End of parseA()"<<endl;
}

void Parser::At(){
	//cout<<"Inside parseAt()"<<endl;
	Af();
	//cout << nextToken.value << endl;
	while(nextToken.value == "*" or nextToken.value=="/"){
		Token temp = nextToken;
		readToken(nextToken);
		Af();
		treeBuilder(temp.value,2);
	}
	//cout<<"End of parseAt()"<<endl;
}

void Parser::Af(){
	//cout<<"Inside parseAf()"<<endl;
	Ap();
	while(nextToken.value == "**"){
		Token temp = nextToken;
		readToken(nextToken);
		Af();
		treeBuilder(temp.value,2);
	}
	//cout<<"End of parseAf()"<<endl;
}

void Parser::Ap(){
	//cout<<"Inside parseAp()"<<endl;
	R();
	while(nextToken.value == "@"){
		Token temp = nextToken;
		readToken(nextToken); 
		if(nextToken.type != ID)
			throw "Expected Identifier found in parseAp()";
		readToken(nextToken); 
		R();
		treeBuilder(temp.value,3);
	}
	//cout<<"End of parseAp()"<<endl;
}

void Parser::R(){
	//cout<<"Inside parseR()"<<endl;
	Rn();
	while(nextToken.type == ID || nextToken.type == STR || nextToken.type == INT ||
			nextToken.value == "true" || nextToken.value == "false" || nextToken.value == "nil" ||
			nextToken.value == "(" || nextToken.value == "dummy"){
		Rn();
		treeBuilder("gamma",2);
	}
	//cout<<"End of parseR()"<<endl;
}

void Parser::Rn(){
	//cout<<"Inside parseRn()"<<endl;
	//cout << nextToken.value << endl;
	//cout << "----" << nextToken.type << endl;
	//cout << nextToken.value << endl;
	//cout << nextToken.type << endl;
	if(nextToken.type == ID or nextToken.type == STR or nextToken.type == INT ){
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
	//cout<<"End of parseRn()"<<endl;
}

void Parser::D(){
	//cout<<"Inside parseD()"<<endl;
	Da();
	if(nextToken.value == "within"){
		readToken(nextToken);
		D();
		treeBuilder("within",2);
	}
	//cout<<"End of parseD()"<<endl;
}

void Parser::Da(){
	//cout<<"Inside parseDa()"<<endl;
	Dr();
	if(nextToken.value== "and"){
		int n = 1;
		Token temp = nextToken;
		while(nextToken.value == "and"){
			readToken(nextToken);
			Dr();
			n++;
		}
		treeBuilder(temp.value,n);
	}
	//cout<<"End of parseDa()"<<endl;
}

void Parser::Dr(){
	//cout<<"Inside parseDr()"<<endl;
	if(nextToken.value == "rec"){
		Token temp = nextToken;
		readToken(nextToken);
		Db();
		treeBuilder(temp.value,1);
	}else{
		Db();
	}
	//cout<<"End of parseDr()"<<endl;
}

void Parser::Db(){
	//cout<<"Inside parseDb()"<<endl;
	//cout<<nextToken.type + " "+la.peekNextToken().value <<endl;
	//Check if the next token is V1
	if(nextToken.value == "("){
		//cout << "Inside if";
		readToken(nextToken);
		D();
		Token t(")",OPT);
		readToken(t);
	} else if(nextToken.type == ID && (la.peekNextToken().value == "," || la.peekNextToken().value == "=")){
		// if next token is '=' or ',' then rule is Vl '=' E
		//cout << "else if";
		Vl();
		Token t("=",OPT);
		readToken(t);
		E();
		//cout<< "Return from E" << endl;
		treeBuilder("=",2);
		//cout<<"Tree build "<<endl;
	} else{// else rule is '<ID>' Vb+ '=' E
		//cout << "Else";
		readToken(nextToken);
		int n = 1;
		Vb();
		while(nextToken.type == ID or nextToken.value== "("){
			n++;
			Vb();
		}
		Token t("=",OPT);
		readToken(t);
		E();
		treeBuilder("function_form", n+2);
	}
	//cout<<"End of parseDb()"<<endl;
}

void Parser::Vb(){
	//cout<<"Inside parseVb()"<<endl;
	if(nextToken.type == ID){
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
	//cout<<"End of parseVb()"<<endl;
}

void Parser::Vl(){
	//cout<<"Inside parseV1()"<<endl;
	readToken(nextToken);
	int n =1;
	if(nextToken.value == ","){
		while(nextToken.value == ","){
			n++;
			readToken(nextToken); 
			readToken(nextToken); 
		}
		treeBuilder(",",n);
	}
	//cout<<"End of parseV1()"<<endl;
}


void Parser::printTree(){
	//cout<<"Stack size: "<<stack.getSize()<<endl;
	while(!trees.empty()){
		Node* t = trees.top();
		trees.pop();
		preOrder(t,string(""));	
	}
}


void Parser::preOrder(Node* t, string dots){
	formattedPrint(t->value,dots);
	if(t->left != NULL)
		preOrder(t->left, "." + dots);
	if(t->right != NULL)
		preOrder(t->right,dots);
}

void Parser::formattedPrint(Token t,string dots){
	/*if(t.type == ID){
		cout <<dots<<"<ID:"<< t.value<<'>'<<endl;
	}else if(t.type == INT){
		cout <<dots<<"<INT:"<< t.value<<'>'<<endl;
	}else if(t.type == STR){
		cout << dots<<"<STR:"<<t.value<<'>'<<endl;
	}else */
	/*if(t.value == "true" || t.value == "false" || t.value == "nil" || t.value== "dummy"){
		cout << dots<<'<'<<t.value<<'>'<<endl;
	}else{
		cout <<dots<<t.value<<endl;
	}*/
	if(t.value != "true" && t.value != "false" && t.value != "nil" && t.value != "dummy"){
		cout <<dots<<t.value<<endl;
	}else{
		cout << dots<<'<'<<t.value<<'>'<<endl;
	}
}

void Parser::parse(){
	la.tokenizeStr();
	la.currPtr = 0;
	//cout << "In parse" << endl;
	nextToken = la.getNextToken();
	//cout << nextToken.value<<endl;
	E();
}
