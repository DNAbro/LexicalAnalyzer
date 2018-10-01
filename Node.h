#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <vector>


using namespace std;
//class Node;

/*class Tree{
private:
	Node* root;

public:
	Tree(Node* node){
		root = node;
	}

	Node* getRoot(){
		return root;
	}
	
};*/

//#include "Tree.h"

class Node{

private:
	Node* parent;				///who the parent is
	//Node** children;			//any number of children
	std::vector<Node*> childVec;
	bool leaf;					//if I am a leaf or not
	//int numberOfChildren;
	string data;

public:

	//Constructors
	Node(string d){
		data = d;
		leaf = true;
		//childVec = NULL;
	}
	

	Node(){

	}

	

	void setParent(Node* p){
		parent = p;
	}
	
	void addTree(Node *a){		//pointer cause c++ is goddamn nightmare language

		a->setParent(this);		//oh god did i finally figure this out
		std::vector<Node*>::iterator it;

		if (childVec.empty()){
			childVec.push_back(a);
		}
		else{
			it = childVec.begin();
			childVec.insert(it, a);
		}
		leaf = false;
		
		//if (it. == NULL)
		
		//tree.getRoot().setParent(this);
	}

	void printOutTree(int c){
		int i = 0;
		for (i; i < c; i++){
			std::cout << ". ";
		}
		std::cout << data << "(" << childVec.size() << ")\n";
		//cout << data;

		//checkLeaf();
		if (!leaf){
			std::vector<Node*>::iterator it;
			it = childVec.begin();
			int count = 0;
			for (it; it < childVec.end(); it++){
				//count++;
				childVec.at(count)->printOutTree(c+1);
				count++;
			}
			/*it = childVec.end();
			int count = 0;
			for (it; it > childVec.begin(); it--){
				childVec.at(childVec.size() - 1 - count)->printOutTree(c + 1);
				count++;
			}*/
		}

	}

	

	//need thing to build tree
	//and to traverse tree


};
#endif