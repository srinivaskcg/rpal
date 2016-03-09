/*
 * FileReader.h
 *
 *  Created on: Oct 14, 2011
 *      Author: Saurabh Saxena
 *      // UFID        : 21817195
 */

#ifndef FILEREADER_H_
#define FILEREADER_H_

class FileReader {
private:
	char* fileName;
public:
	FileReader(char* fileName);
	virtual ~FileReader();
	std::string getFileContent();

};

#endif /* FILEREADER_H_ */
