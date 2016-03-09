// UFID        : 21817195
// Author 	   : Saurabh Saxena
#include <iostream>
#include "RecursiveParser.h"
#include "Token.h"

using namespace std;

string RecursiveParser::ID = "IDENTIFIER";
string RecursiveParser::STR = "STRING";
string RecursiveParser::INT = "INTEGER";
string RecursiveParser::KEY = "KEYWORD";
string RecursiveParser::OPT = "OPERATOR";

RecursiveParser::RecursiveParser(LexicalAnalyzer la){
	this->la = la;
	this->moreTokens = true;
}

RecursiveParser::~RecursiveParser(){

}

void RecursiveParser::read(Token token){
	if(moreTokens == false)
		throw "Error: No more token to parse";
	if(token.value != nextToken.value){
		//cout << "Expected "<<token.value << " but found" <<nextToken.value;
		exit(1);
	}
	if(token.type == ID or token.type == STR or token.type == INT){
		buildTree(token, 0);
	}
	try{
		nextToken = la.getNextToken();
	}catch(const char* message){
		moreTokens = false;
		Token endToken("$$","$$");
		nextToken = endToken;
	}
}


void RecursiveParser::buildTree(Token token, int numOfNodes){
	//cout<< "Inside buildTree "<< token.value<<endl;
	//cout<< "Stack size:"<<stack.getSize()<<endl;
	TreeNode* newNode = new TreeNode;
	newNode->value = token;
	if(numOfNodes != 0){
		int i;
		for(i=0;i<numOfNodes-1;i++){
			//cout<<"Count "+i<<endl;
			TreeNode* curr = stack.pop();
			stack.addRightChild(curr);
		}
		TreeNode* top = stack.pop();
		if(top != NULL)
			newNode->left = top;
	}
	stack.push(newNode);
}


void RecursiveParser::parseE(){
	//cout<<"Inside parseE()"<<endl;
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

void RecursiveParser::parseEw(){
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



void RecursiveParser::parseT(){
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
		buildTree(tauToken,n+1);
	}
	//cout<<"End of parseT()"<<endl;
}

void RecursiveParser::parseTa(){
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

void RecursiveParser::parseTc(){
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

void RecursiveParser::parseB(){
	//cout<<"Inside parseB()"<<endl;
	parseBt();
	while(nextToken.value == "or"){
		Token temp = nextToken;
		read(nextToken);
		parseBt();
		buildTree(temp,2);
	}
	//cout<<"End of parseB()"<<endl;
}

void RecursiveParser::parseBt(){
	//cout<<"Inside parseBt()"<<endl;
	parseBs();
	while(nextToken.value == "&"){
		Token temp = nextToken;
		read(nextToken);
		parseBs();
		buildTree(temp,2);
	}
	//cout<<"End of parseBt()"<<endl;
}

void RecursiveParser::parseBs(){
	//cout<<"Inside parseBs()"<<endl;
	if(nextToken.value == "not"){
		Token temp = nextToken;
		read(nextToken);
		parseBp();
		buildTree(temp,1);
	}else{
		parseBp();
	}
	//cout<<"End of parseBs()"<<endl;
}

void RecursiveParser::parseBp(){
	//cout<<"Inside parseBp()"<<endl;
	parseA();
	if(nextToken.value=="gr" or nextToken.value == ">"){
		parseBpHelper(nextToken, "gr");
	}else if(nextToken.value == "ls" or nextToken.value == "<"){
		parseBpHelper(nextToken,"ls");
	}else if(nextToken.value =="ge" or nextToken.value == ">=" ){
		parseBpHelper(nextToken, "ge");
	}else if(nextToken.value =="le" or nextToken.value == "<=" ){
		parseBpHelper(nextToken, "le");
	}else if(nextToken.value =="eq" ){
		parseBpHelper(nextToken, "eq");
	}else if(nextToken.value =="ne"){
		parseBpHelper(nextToken, "ne");
	}
	//cout<<"End of parseBp()"<<endl;
}

void RecursiveParser::parseBpHelper(Token nextToken, string tokenValue){
	//cout<<"Inside parseBpHelper()"<<endl;
	read(nextToken);
	parseA();
	Token t(tokenValue, tokenValue);
	buildTree(t,2);
	//cout<<"End of parseBpHelper()"<<endl;
}

void RecursiveParser::parseA(){
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

void RecursiveParser::parseAt(){
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

void RecursiveParser::parseAf(){
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

void RecursiveParser::parseAp(){
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

void RecursiveParser::parseR(){
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

void RecursiveParser::parseRn(){
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

void RecursiveParser::parseRHelper(Token t, string value){
	//cout<<"Inside parseRHelper()"<<endl;
	read(t);
	Token nodeToken(value,value);
	buildTree(nodeToken,0);
	//cout<<"End of parseRHelper()"<<endl;
}

void RecursiveParser::parseD(){
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

void RecursiveParser::parseDa(){
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

void RecursiveParser::parseDr(){
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

void RecursiveParser::parseDb(){
	//cout<<"Inside parseDb()"<<endl;
	//cout<<nextToken.type + " "+la.peekNextToken().value <<endl;
	//Check if the next token is V1
	if(nextToken.value == "("){
		read(nextToken);
		parseD();
		Token t(")",OPT);
		read(t);
	} else if(nextToken.type == ID && (la.peekNextToken().value == "," || la.peekNextToken().value == "=")){
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

void RecursiveParser::parseVb(){
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

void RecursiveParser::parseV1(){
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


void RecursiveParser::printTree(){
	//cout<<"Stack size: "<<stack.getSize()<<endl;
	TreeNode* t = stack.pop();
	preOrder(t,std::string(""));
}


void RecursiveParser::preOrder(TreeNode* t, std::string dots){
	formattedPrint(t->value,dots);
	string dots1 = "." + dots;
	if(t->left != NULL)
		preOrder(t->left,dots1);
	if(t->right != NULL)
		preOrder(t->right,dots);
}

void RecursiveParser::formattedPrint(Token t,std::string dots){
	if(t.type == ID){
		cout <<dots<<"<ID:"<< t.value<<'>'<<endl;
	}else if(t.type == INT){
		cout <<dots<<"<INT:"<< t.value<<'>'<<endl;
	}else if(t.type == STR){
		cout << dots<<"<STR:"<<t.value<<'>'<<endl;
	}else if(t.value == "true" or t.value == "false" or t.value == "nil" or t.value== "dummy"){
		cout << dots<<'<'<<t.value<<'>'<<endl;
	}else{
		cout <<dots<<t.value<<endl;
	}
}

void RecursiveParser::parse(){
	nextToken = la.getNextToken();
	//cout << nextToken.value<<endl;
	parseE();
}
