//Andres Borjas
//Sub-C Parser

#include <iostream>
#include "AST.h"
#include "Parser.h"
#include "BuildTree.h"
using namespace std;

int main(){


	/*cout << "Hello, World!";

	int test;
	cin >> test;*/

	Parser testboi("test3.file");

	//testboi.testFunction();

	testboi.readOutFile();

	string* tokenList = testboi.getTokenHolder();

	//cout << "This is the first thing in the list:" << tokenList[0];

	BuildTree testTreeBoi(tokenList);
	testTreeBoi.buildTreeTest();

	int a;
	cin >> a;

	return 0;




}