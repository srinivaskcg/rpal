#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

extern ifstream inStream;

class Token {

public:

    Token(){}
    Token(std::string value, std::string type);

    string type;
    string value;

};

class Lexer {

public:

    Lexer(){}
    
    bool isIdentifier(string);
	bool isKeyword(string);
	bool isInteger();
	bool isOperator(char);
	bool isString();
	void tokenizeString(string);

    string type;

};

class Parser {

public:

	Lexer lex;
    Parser(string parseString);

    string getNextToken(string);
    
    void E();
    void Ew();

	void D();
	void Da();
	void Dr();
	void Db();

	void Vl();
	void Vb();

	void T();
	void Ta();
	void Tc();

	void B();
	void Bt();
	void Bs();
	void Bp();

	void A();
	void At();
	void Af();
	void Ap();

	void R();
	void Rn();

};
#endif //HEADER_H