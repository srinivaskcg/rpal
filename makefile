p1 : RpalProject1.o Lexer.o Parser.o Node.o Token.o
	g++ -o p1 RpalProject1.o Lexer.o Parser.o Node.o Token.o

RpalProject1.o : RpalProject1.cpp Lexer.h Parser.h
	g++ -std=c++11 -c RpalProject1.cpp
	
Lexer.o : Lexer.cpp Lexer.h Token.h
	g++ -std=c++11 -c Lexer.cpp

Parser.o : Parser.cpp Parser.h Token.h
	g++ -std=c++11 -c Parser.cpp

Node.o : Node.cpp Node.h
	g++ -std=c++11 -c Node.cpp

Token.o : Token.cpp Token.h
	g++ -std=c++11 -c Token.cpp

run :
	./p1

test :
	perl testDir/difftest.pl -1 "testDir/rpal -ast -noout FILE" -2 "./p1 -ast FILE" -t testDir/tests/

cl :
	rm *.o p1
