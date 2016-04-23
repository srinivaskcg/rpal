all :
	g++ PLPProj1.cpp LexicalAnalyzer.cpp Token.cpp TreeNode.cpp TreeTransformer.cpp CSEMachine.cpp -std=c++11 -o p2

cl :
	rm -f *.o p2
