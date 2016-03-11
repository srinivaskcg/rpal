# Make file for PLPProj1

p1 : LexicalAnalyzer.o PLPProj1.o RecursiveParser.o Token.o TreeNode.o
	g++ -o p1 LexicalAnalyzer.o PLPProj1.o RecursiveParser.o Token.o TreeNode.o
	
LexicalAnalyzer.o : LexicalAnalyzer.cpp LexicalAnalyzer.h Token.h
	g++ -c LexicalAnalyzer.cpp

PLPProj1.o : PLPProj1.cpp LexicalAnalyzer.h RecursiveParser.h
	g++ -c PLPProj1.cpp

RecursiveParser.o : RecursiveParser.cpp RecursiveParser.h Token.h
	g++ -c RecursiveParser.cpp

# Stack.o : Stack.cpp Stack.h
# 	g++ -c Stack.cpp

Token.o : Token.cpp Token.h
	g++ -c Token.cpp

Node.o : TreeNode.cpp TreeNode.h
	g++ -c TreeNode.cpp

run :
	./p1

test :
	perl testDir/difftest.pl -1 "testDir/rpal -ast -noout FILE" -2 "./p1 -ast -noout FILE" -t testDir/tests/

cl :
	rm *.o p1
