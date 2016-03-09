/*
 * FileReader.cpp
 *
 *  Created on: Oct 14, 2011
 *      Author: Saurabh Saxena
 *      // UFID        : 21817195
 */
#include <iostream>
#include <fstream>
#include <string>
#include "FileReader.h"

using namespace std;

FileReader::FileReader(char* fileName) {
	this->fileName = fileName;
}

FileReader::~FileReader() {
	// TODO Auto-generated destructor stub
}

std::string FileReader::getFileContent(){
	std::string returnString;
	std::string tempString;
	ifstream inputFile;
	inputFile.open(this->fileName, ios::in);
	if(inputFile.is_open()){
		while(inputFile.good()){
			getline(inputFile,tempString);
			returnString +=  tempString+'\n';
		}
	} else{
		cerr << "Error: Problem opening input file";
	}
	return returnString;
}

