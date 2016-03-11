#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
using namespace std;

class Token {
public:
	Token();
	Token(string value, string type);
	virtual ~Token();
	string value;
	string type;

};

#endif // TOKEN_H_ 
