#include <string>
using namespace std;

#ifndef TOKEN_H_
#define TOKEN_H_

class Token {
public:
	Token();
	Token(string value, string type);
	virtual ~Token();
	string value;
	string type;

};

#endif // TOKEN_H_ 
