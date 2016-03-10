#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "LexicalAnalyzer.h"
#include "RecursiveParser.h"

using namespace std;

string openFile(char* fileName){
	
	string fileContent;
	string nextLine;

	ifstream inputFile;

	//cout << fileName;

	inputFile.open(fileName);
	
	if(inputFile.fail())
		cerr << "Error: Problem opening input file";

	if(inputFile.is_open()){
		while(inputFile.good()){
			getline(inputFile,nextLine);
			fileContent +=  nextLine + "\n";
		}
	}
	//cout << fileContent << endl;

	return fileContent;
}

int main(int argc,char *argv[]) {
	if(argc < 3){
		//cout<< "Usage: p1 -ast fileLocation"<<endl;
		return 0;
	}
	int i;
	char* input;
	bool astFound = false;
	for(int i=0;i<argc;i++){
		//cout <<argv[i]<<endl;
		if(strcmp(argv[i],"-ast") == 0){
			astFound = true;
			break;
		}
	}
	if(astFound == true){
		
		input = argv[2];
		//cout << input << endl;
		string codeString = openFile(input);

		if(codeString.size() == 0)
			return 0;
		LexicalAnalyzer la(codeString);
		RecursiveParser rp(la);
		rp.parse();
		rp.printTree();
	}else{
		cout<< "Usage: p1 -ast fileLocation"<<endl;
	}
	return 0;
}
