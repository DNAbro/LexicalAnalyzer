//Andres Borjas
//Sub-C Parser

/*#include <iostream>
#include "AST.h"
#include "Parser.h"
#include "BuildTree.h"
using namespace std;

int main(int argc, char* argv[]){


	std::string filePath;


	if (argc > 2){
		filePath = std::string(argv[2]);
	}
	else{
		filePath = std::string(argv[1]);
	}

	//Parser testboi("test25.file");

	Parser testboi(filePath);

	//testboi.testFunction();

	testboi.readOutFile();

	string* tokenList = testboi.getTokenHolder();

	//cout << "This is the first thing in the list:" << tokenList[0];

	BuildTree testTreeBoi(tokenList);

	if (argc > 2 && strcmp(argv[1], "-ast") == 0){
		testTreeBoi.buildTreeTest();
	}
	


	return 0;




}*/