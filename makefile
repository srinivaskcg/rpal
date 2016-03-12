all :
	g++ RpalProject1.cpp LexParser.cpp Token.cpp Node.cpp -std=c++11 -o p1 

cl :
	rm -f *.o p1
