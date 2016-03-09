//============================================================================
// Name        : PLPProj1.cpp
// Author      : Saurabh Saxena
// UFID        : 21817195
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "FileReader.h"
#include "LexicalAnalyzer.h"
#include "RecursiveParser.h"

using namespace std;

int main(int argc,char *argv[]) {
	if(argc < 3){
		//cout<< "Usage: p1 -ast fileLocation"<<endl;
		return 0;
	}
	int i;
	bool astFound = false;
	for(int i=0;i<argc;i++){
		//cout <<argv[i]<<endl;
		if(strcmp(argv[i],"-ast") == 0){
			astFound = true;
			break;
		}
	}
	if(astFound == true){
		FileReader fr(argv[argc-1]);
		string fileContent = fr.getFileContent();
		if(fileContent.size() == 0)
			return 0;
		LexicalAnalyzer la(fileContent);
		RecursiveParser rp(la);
		rp.parse();
		rp.printTree();
	}else{
		//cout<< "Usage: p1 -ast fileLocation"<<endl;
	}
	return 0;
}
