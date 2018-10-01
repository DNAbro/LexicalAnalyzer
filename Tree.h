#ifndef TREE_H
#define TREE_H
#include <iostream>
//#include "Node.h"

//class Node;

class Tree{
private:
	Node root;

public:
	Tree(Node node){
		root = node;
	}

	Node getRoot(){
		return root;
	}
};
#endif