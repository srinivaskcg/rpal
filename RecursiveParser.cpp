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

Parser::Parser(LexicalAnalyzer la){
	this->la = la;
	this->moreTokens = true;
}

Parser::~Parser(){

}

void Parser::addRightChild(TreeNode* treeNode){
	TreeNode* parentNode = trees.top();
	trees.pop();
	parentNode->right = treeNode;
	trees.push(parentNode);
}

void Parser::read(Token token){

	//cout << "Inside read" << "\t" <<token.type << "\t" << token.value << "\t" << nextToken.value<< endl;
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


void Parser::buildTree(Token token, int numOfNodes){
	//cout<< "Inside buildTree "<< token.value<<endl;
	//cout<< "Stack size:"<<stack.getSize()<<endl;
	TreeNode* newNode = new TreeNode;
	newNode->value = token;
	if(numOfNodes != 0){
		int i;
		for(i=0;i<numOfNodes-1;i++){
			//cout<<"Count "+i<<endl;
			TreeNode* curr = trees.top();
			trees.pop();
			addRightChild(curr);
		}
		TreeNode* top = trees.top();
		trees.pop();
		if(top != NULL)
			newNode->left = top;
	}
	trees.push(newNode);
}


void Parser::E(){
	//cout<<"Inside E()"<<endl;
	if(nextToken.value == "let"){
		Token letToken("let",KEY);
		//cout << "In E" << endl;
		read(letToken);
		D();
		Token inToken("in",KEY);
		read(inToken);
		E();
		Token nodeToken("let","let");
		buildTree(nodeToken,2);
	} else if(nextToken.value == "fn"){
		Token fnToken("fn","fn");
		read(fnToken);
		int n=0;
		do{
			Vb();
			n++;
		}while(nextToken.type == ID or nextToken.type == "(");
		Token dotToken(".",OPT);
		read(dotToken);
		E();
		Token nodeToken("lambda","lambda");
		buildTree(nodeToken,n+1);
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
		read(t);
		Dr();
		Token nodeToken("where","where");
		buildTree(nodeToken,2);
	}
	//cout<<"End of Ew()"<<endl;
}



void Parser::T(){
	//cout<<"Inside parseT()"<<endl;
	Ta();
	if(nextToken.value == ","){
		int n = 0;
		do{
			read(nextToken);
			Ta();
			n++;
		}while(nextToken.value == ",");
		Token tauToken("tau","tau");
		buildTree(tauToken,n+1);
	}
	//cout<<"End of parseT()"<<endl;
}

void Parser::Ta(){
	//cout<<"Inside parseTa()"<<endl;
	Tc();
	while(nextToken.value == "aug"){
		Token temp = nextToken;
		read(nextToken);
		Tc();
		buildTree(temp,2);
	}
	//cout<<"End of parseTa()"<<endl;
}

void Parser::Tc(){
	//cout<<"Inside parseTc()"<<endl;
	B();
	if(nextToken.value == "->"){
		read(nextToken);
		Tc();
		Token elseToken("|",OPT);
		read(elseToken);
		Tc();
		Token nodeToken("->","->");
		buildTree(nodeToken,3);
	}
	//cout<<"End of parseTc()"<<endl;
}

void Parser::B(){
	//cout<<"Inside parseB()"<<endl;
	Bt();
	while(nextToken.value == "or"){
		Token temp = nextToken;
		read(nextToken);
		Bt();
		buildTree(temp,2);
	}
	//cout<<"End of parseB()"<<endl;
}

void Parser::Bt(){
	//cout<<"Inside parseBt()"<<endl;
	Bs();
	while(nextToken.value == "&"){
		Token temp = nextToken;
		read(nextToken);
		Bs();
		buildTree(temp,2);
	}
	//cout<<"End of parseBt()"<<endl;
}

void Parser::Bs(){
	//cout<<"Inside parseBs()"<<endl;
	if(nextToken.value == "not"){
		Token temp = nextToken;
		read(nextToken);
		Bp();
		buildTree(temp,1);
	}else{
		Bp();
	}
	//cout<<"End of parseBs()"<<endl;
}

void Parser::Bp(){
	//cout<<"Inside parseBp()"<<endl;
	A();
	if(nextToken.value=="gr" or nextToken.value == ">"){
		BpHelper(nextToken, "gr");
	}else if(nextToken.value == "ls" or nextToken.value == "<"){
		BpHelper(nextToken,"ls");
	}else if(nextToken.value =="ge" or nextToken.value == ">=" ){
		BpHelper(nextToken, "ge");
	}else if(nextToken.value =="le" or nextToken.value == "<=" ){
		BpHelper(nextToken, "le");
	}else if(nextToken.value =="eq" ){
		BpHelper(nextToken, "eq");
	}else if(nextToken.value =="ne"){
		BpHelper(nextToken, "ne");
	}
	//cout<<"End of parseBp()"<<endl;
}

void Parser::BpHelper(Token nextToken, string tokenValue){
	//cout<<"Inside parseBpHelper()"<<endl;
	read(nextToken);
	A();
	Token t(tokenValue, tokenValue);
	buildTree(t,2);
	//cout<<"End of parseBpHelper()"<<endl;
}

void Parser::A(){
	//cout<<"Inside parseA()"<<endl;
	if(nextToken.value == "-"){
		Token negToken("-",OPT);
		read(negToken);
		At();
		Token nodeToken("neg","neg");
		buildTree(nodeToken,1);
	}else if(nextToken.value == "+"){
		Token posToken("+",OPT);
		read(posToken);
		At();
	}else{
		At();
	}
	while(nextToken.value == "+" or nextToken.value == "-"){
		Token temp = nextToken;
		read(nextToken);
		At();
		buildTree(temp,2);
	}
	//cout<<"End of parseA()"<<endl;
}

void Parser::At(){
	//cout<<"Inside parseAt()"<<endl;
	Af();
	//cout << nextToken.value << endl;
	while(nextToken.value == "*" or nextToken.value=="/"){
		Token temp = nextToken;
		read(nextToken);
		Af();
		buildTree(temp,2);
	}
	//cout<<"End of parseAt()"<<endl;
}

void Parser::Af(){
	//cout<<"Inside parseAf()"<<endl;
	Ap();
	while(nextToken.value == "**"){
		Token temp = nextToken;
		read(nextToken);
		Af();
		buildTree(temp,2);
	}
	//cout<<"End of parseAf()"<<endl;
}

void Parser::Ap(){
	//cout<<"Inside parseAp()"<<endl;
	R();
	while(nextToken.value == "@"){
		Token temp = nextToken;
		read(nextToken); //Read '@'
		if(nextToken.type != ID)
			throw "Expected Identifier found in parseAp()";
		read(nextToken); //Read identifier
		R();
		buildTree(temp,3);
	}
	//cout<<"End of parseAp()"<<endl;
}

void Parser::R(){
	//cout<<"Inside parseR()"<<endl;
	Rn();
	while(nextToken.type == ID or nextToken.type == STR or nextToken.type == INT or
			nextToken.value == "true" or nextToken.value == "false" or nextToken.value == "nil" or
			nextToken.value == "(" or nextToken.value == "dummy"){
		Rn();
		Token nodeToken("gamma","gamma");
		buildTree(nodeToken,2);
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
		//cout << "In" << endl;
			read(nextToken);
			//cout << "return" << endl;
	}else if(nextToken.value=="true"){
		RHelper(nextToken, "true");
	}else if(nextToken.value == "false"){
		RHelper(nextToken, "false");
	}else if(nextToken.value == "nil"){
		RHelper(nextToken, "nil");
	}else if(nextToken.value == "dummy"){
		RHelper(nextToken,"dummy");
	}else if(nextToken.value == "("){
		read(nextToken);
		E();
		Token t(")",")");
		read(t);
	}
	//cout<<"End of parseRn()"<<endl;
}

void Parser::RHelper(Token t, string value){
	//cout<<"Inside parseRHelper()"<<endl;
	read(t);
	Token nodeToken(value,value);
	buildTree(nodeToken,0);
	//cout<<"End of parseRHelper()"<<endl;
}

void Parser::D(){
	//cout<<"Inside parseD()"<<endl;
	Da();
	if(nextToken.value == "within"){
		read(nextToken);
		D();
		Token nodeToken("within", "within");
		buildTree(nodeToken,2);
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
			read(nextToken);
			Dr();
			n++;
		}
		buildTree(temp,n);
	}
	//cout<<"End of parseDa()"<<endl;
}

void Parser::Dr(){
	//cout<<"Inside parseDr()"<<endl;
	if(nextToken.value == "rec"){
		Token temp = nextToken;
		read(nextToken);
		Db();
		buildTree(temp,1);
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
		read(nextToken);
		D();
		Token t(")",OPT);
		read(t);
	} else if(nextToken.type == ID && (la.peekNextToken().value == "," || la.peekNextToken().value == "=")){
		// if next token is '=' or ',' then rule is Vl '=' E
		//cout << "else if";
		Vl();
		Token t("=",OPT);
		read(t);
		E();
		//cout<< "Return from E" << endl;
		Token nodeToken("=","=");
		buildTree(nodeToken,2);
		//cout<<"Tree build "<<endl;
	} else{// else rule is '<ID>' Vb+ '=' E
		//cout << "Else";
		read(nextToken);
		int n = 1;
		Vb();
		while(nextToken.type == ID or nextToken.value== "("){
			n++;
			Vb();
		}
		Token t("=",OPT);
		read(t);
		E();
		Token nodeToken("function_form", "function_form");
		buildTree(nodeToken, n+2);
	}
	//cout<<"End of parseDb()"<<endl;
}

void Parser::Vb(){
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
			Vl();
			Token t(")",")");
			read(t);
		}
	}
	//cout<<"End of parseVb()"<<endl;
}

void Parser::Vl(){
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


void Parser::printTree(){
	//cout<<"Stack size: "<<stack.getSize()<<endl;
	TreeNode* t = trees.top();
	trees.pop();
	preOrder(t,std::string(""));
}


void Parser::preOrder(TreeNode* t, std::string dots){
	formattedPrint(t->value,dots);
	string dots1 = "." + dots;
	if(t->left != NULL)
		preOrder(t->left,dots1);
	if(t->right != NULL)
		preOrder(t->right,dots);
}

void Parser::formattedPrint(Token t,std::string dots){
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

void Parser::parse(){
	la.tokenizeStr();
	la.currPtr = 0;
	//cout << "In parse" << endl;
	nextToken = la.getNextToken();
	//cout << nextToken.value<<endl;
	E();
}
