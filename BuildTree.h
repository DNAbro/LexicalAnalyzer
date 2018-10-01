#include <iostream>
#include <string>
#include <stack>
//#include "Tree.h"
#include "Node.h"


using namespace std;


class BuildTree{

private:
	string tokens[1024];
	int countIndex;
	//std::stack<Tree> mystack;
	std::stack<Node*> mystack;
	//int caseCount;
	//need a stack of trees
	

public:

	BuildTree(string* tokenList){
		tokens[0] = tokenList[0];
		int copyCount = 0;
		while (tokenList[copyCount] != ""){
			tokens[copyCount] = tokenList[copyCount];
			copyCount++;
		}
		//caseCount = 0;
		cout << "This is first thing in Buildtree:" << tokens[0] << "\n";
		cout << "This is last thing in Buildtree:" << tokens[copyCount-1] << "\n";

		//cout << tokens[0];

		countIndex = 0;
		//mystack ?? initialize it
		
	}

	void buildTreeTest(){
		string blah;
		//cout << "This is first thing in Buildtree:" << tokens[0];
		Tiny();

		mystack.top()->printOutTree(0);


		
	}


	//Start of program

	/*
	Tiny       -> 'program' Name ':' Consts Types Dclns
                 SubProgs Body Name '.' 		=> "program";
	*/
	void Tiny(){
		//read 1st token
		if (tokens[countIndex] == "program"){
			cout << "it's yah boi program";
			advanceTokenCount();
			Name();
			
			if (tokens[countIndex] == ":"){
				advanceTokenCount();
				//cout << "I saw the colon bithc";
			}
			//error if not
			Consts();
			Types();
			Dclns();
			SubProgs();
			Body();
			Name();
			if (tokens[countIndex] == "."){
				//end of program.
				cout << "I reached the end of the program.\n";
			}
			makeTree("program",7);

		}
	}


	/*
	Consts     -> 'const' Const list ',' ';'		=> "consts"
				-> 						=> "consts";
	*/
	void Consts(){

		int count = 0;
		if (tokens[countIndex] == "consts"){
			advanceTokenCount();
			while (tokens[countIndex] != ";"){
				
				Const();
				count++;
			}
		}
		makeTree("consts",count);
	}

	/*
	Const      -> Name '=' ConstValue			=> "const";
	*/
	//Can I name this that?
	void Const(){
		Name();
		if (tokens[countIndex] == "="){
			advanceTokenCount();
			ConstValue();
		}
		makeTree("const",2);

	}

	/*
	ConstValue -> '<integer>'
	-> '<char>'
	-> Name;
	*/
	void ConstValue(){
		if (checkIfIdentifierCharStringOrInteger(tokens[countIndex]) == "integer"){
			makeTree(tokens[countIndex], 0);
			advanceTokenCount();
			
			makeTree("<integer>",1);
		}
		else if (checkIfIdentifierCharStringOrInteger(tokens[countIndex]) == "char"){
			makeTree(tokens[countIndex], 0);
			advanceTokenCount();
			makeTree("<char>",1);
			//i need something else here.
		}
		else{
			Name();
		}
		
		

	}

	/*
	Types      -> 'type' (Type ';')+			=> "types"
				-> 						=> "types";
	*/
	void Types(){
		int count = 0;
		//need count probably?
		//1 or more
		if (tokens[countIndex] == "type"){

			advanceTokenCount();
			count++;
			Type();
			if (tokens[countIndex] == ";"){
				advanceTokenCount();
			}

			//if the next one is a Type->name()->identifier
			while (checkIfIdentifierCharStringOrInteger(tokens[countIndex]) == "identifier"){
				count++;
				Type();
				if (tokens[countIndex] == ";"){
					advanceTokenCount();
				}
			}
			
			

		}
		makeTree("types",count);
	}

	/*
	Type       -> Name '=' LitList				=> "type";
	*/
	void Type(){
		Name();
		if (tokens[countIndex] == "="){
			advanceTokenCount();
			LitList();
		}
		makeTree("type",2);
	}

	/*
	LitList    -> '(' Name list ',' ')' 			=> "lit";
	*/
	void LitList(){
		int count = 0;
		if (tokens[countIndex] == "("){
			advanceTokenCount();
			while (tokens[countIndex] != ")"){

				Name();	//what is list
				count++;

				if (tokens[countIndex] == ","){
					advanceTokenCount();
				}

				
			}
			advanceTokenCount();	//forgot this before
		}

		makeTree("lit",count);
	}

	/*
	SubProgs   -> Fcn*					=> "subprogs";
	*/
	void SubProgs(){
		int count = 0;
		//can be zero or more so maybe peek at the next one?
		while (tokens[countIndex] == "function"){
			Fcn();
			count++;
		}
		

		makeTree("subprogs",count);
	}

	/*
	Fcn        -> 'function' Name '(' Params ')' ':' Name ';'
	Consts Types Dclns Body Name ';'	=> "fcn";

	*/
	void Fcn(){
		if (tokens[countIndex] == "function"){
			advanceTokenCount();
			Name();
			if (tokens[countIndex] == "("){
				advanceTokenCount();

				Params();

				if (tokens[countIndex] == ")"){
					advanceTokenCount();

					if (tokens[countIndex] == ":"){
						advanceTokenCount();
						Name();
						if (tokens[countIndex] == ";"){
							advanceTokenCount();
							Consts();
							Types();
							Dclns();
							Body();
							Name();
							if (tokens[countIndex] == ";"){
								advanceTokenCount();
							}
						}
					}
				}
			}

		}

		makeTree("fcn",8);
	}

	/*
	Params     -> Dcln list ';'				=> "params";
	*/
	void Params(){
		int count = 0;
		//list so multiple
		while (checkIfIdentifierCharStringOrInteger(tokens[countIndex]) == "identifier"){
			Dcln();
			count++;
			if (tokens[countIndex] == ";"){
				advanceTokenCount();
			}
		}
		

		makeTree("params",count);
	}

	/*
	Dclns      -> 'var' (Dcln ';')+				=> "dclns"
           -> 						=> "dclns";

	*/
	void Dclns(){
		int count = 0;
		

		if (tokens[countIndex] == "var"){
			count++;		//because it needs to be added here
			advanceTokenCount();
			Dcln();

			if (tokens[countIndex] == ";"){
				advanceTokenCount();
			}
			//i think the next thing should be identifier
			while (checkIfIdentifierCharStringOrInteger(tokens[countIndex]) == "identifier"){
				Dcln();
				count++;
				if (tokens[countIndex] == ";"){
					advanceTokenCount();
				}
			}
		}




		
		//+ means 1 or more
		makeTree("dclns",count);
	}

	/*
	Dcln       -> Name list ',' ':' Name			=> "var";
	*/
	void Dcln(){
		int count = 1;
		bool colFound = false;

		while (checkIfIdentifierCharStringOrInteger(tokens[countIndex]) == "identifier" && !colFound){
			count++;
			Name();

			//list???
			if (tokens[countIndex] == ","){
				advanceTokenCount();
			}

			if (tokens[countIndex] == ":"){
				advanceTokenCount();
				colFound = true;
			}
		}
		

		Name();

		makeTree("var",count);
	}

	/*
	Body       -> 'begin' Statement list ';' 'end'      	=> "block";
	*/
	void Body(){
		//int count = 0;	//uncomment if broke
		int count = 1;
		if (tokens[countIndex] == "begin"){
			advanceTokenCount();

			Statement();
			while (tokens[countIndex] == ";"){
				advanceTokenCount();
				Statement();
				count++;
			}
			//this works testing other implementatio for <null>
			/*while (tokens[countIndex] != "end"){
				Statement();
				count++;
				if (tokens[countIndex] == ";"){
					advanceTokenCount();
				}

				
			}*/
			if (tokens[countIndex] == "end"){
				advanceTokenCount();
			}
			
		}

		makeTree("block", count);
	}

	/*
	Statement  -> Assignment
           -> 'output' '(' OutExp list ',' ')'    	=> "output"
           -> 'if' Expression 'then' Statement
                            ('else' Statement)?       	=> "if"
           -> 'while' Expression 'do' Statement      	=> "while"
	   -> 'repeat' Statement list ';' 'until'
	          Expression        			=> "repeat"
	   -> 'for' '(' ForStat ';' ForExp ';' ForStat ')'
	         Statement				=> "for"
	   -> 'loop' Statement list ';' 'pool'		=> "loop"
           -> 'case' Expression 'of' Caseclauses
                   OtherwiseClause 'end'                => "case"
           -> 'read' '(' Name list ',' ')'		=> "read"
	   -> 'exit'					=> "exit"
	   -> 'return' Expression			=> "return"
           -> Body
	   ->                                        	=> "<null>";
	*/
	void Statement(){
		//if the next one is Assignment
		if (checkIfIdentifierCharStringOrInteger(tokens[countIndex]) == "identifier"){//first thing in Assignment is an identifier i think
			Assignment();
		}
		else if (tokens[countIndex] == "output"){
			int count = 0;
			bool end = false;
			advanceTokenCount();

			if (tokens[countIndex] == "("){
				advanceTokenCount();
				while (!end){
					count++;
					OutExp();

					if (tokens[countIndex] == ","){
						advanceTokenCount();
					}

					if (tokens[countIndex] == ")"){
						advanceTokenCount();
						end = true;
					}
				}
				
				//list aks tomorrow

				
				makeTree("output",count);
			}
		}
		else if (tokens[countIndex] == "if"){

			int count = 2;

			/*-> 'if' Expression 'then' Statement
				('else' Statement) ? = > "if"*/
			advanceTokenCount();
			Expression();
			if (tokens[countIndex] == "then"){
				advanceTokenCount();
				Statement();
			}
			if (tokens[countIndex] == "else"){
				advanceTokenCount();
				Statement();
				count++;
			}

			makeTree("if",count);


		}
		else if (tokens[countIndex] == "while"){
			/*-> 'while' Expression 'do' Statement      	=> "while"*/
			advanceTokenCount();
			Expression();
			if (tokens[countIndex] == "do"){
				advanceTokenCount();
				Statement();
			}

			makeTree("while",2);

		}
		else if (tokens[countIndex] == "repeat"){
			int count = 1;
			/*-> 'repeat' Statement list ';' 'until'
	          Expression        			=> "repeat"*/

			advanceTokenCount();
			while (tokens[countIndex] != "until"){
				count++;
				Statement();

				if (tokens[countIndex] == ";"){
					advanceTokenCount();
				}
			}
			advanceTokenCount();

			Expression();
			makeTree("repeat",count);

		}
		else if (tokens[countIndex] == "for"){
			/*-> 'for' '(' ForStat ';' ForExp ';' ForStat ')'
	         Statement				=> "for"*/
			advanceTokenCount();
			if (tokens[countIndex] == "("){
				advanceTokenCount();
				ForStat();
				if (tokens[countIndex] == ";"){
					advanceTokenCount();
					ForExp();
					if (tokens[countIndex] == ";"){
						advanceTokenCount();		//im not sure why i didn't have this here.
						ForStat();
						if (tokens[countIndex] == ")"){
							advanceTokenCount();
							Statement();
							makeTree("for",4);
						}
					}
				}
			}
			
		}
		else if (tokens[countIndex] == "loop"){
			/* -> 'loop' Statement list ';' 'pool'		=> "loop"*/
			advanceTokenCount();
			int count = 0;
			while (tokens[countIndex] != "pool"){
				count++;
				Statement();
				if (tokens[countIndex] == ";"){
					advanceTokenCount();
				}
			}
			advanceTokenCount();
			makeTree("loop",count);

		}
		else if (tokens[countIndex] == "case"){
			/*-> 'case' Expression 'of' Caseclauses
                   OtherwiseClause 'end'                => "case"*/
			int caseCount = 0;
			advanceTokenCount();
			Expression();
			if (tokens[countIndex] == "of"){
				advanceTokenCount();
				caseCount = Caseclauses();			//in order to get the number of cases
				caseCount = OtherwiseClause() + caseCount;
				if (tokens[countIndex] == "end"){
					advanceTokenCount();
					makeTree("case", 2+caseCount);  //this might not be right
					//caseCount = 0;
				}
			}

		}
		else if (tokens[countIndex] == "read"){
			/*-> 'read' '(' Name list ',' ')'		=> "read"*/
			int count = 0;
			advanceTokenCount();
			if (tokens[countIndex] == "("){
				advanceTokenCount();

				while (tokens[countIndex] != ")"){
					count++;
					Name();
					if (tokens[countIndex] == ","){
						advanceTokenCount();
					}
				}
				advanceTokenCount();
			}
			makeTree("read", count);
		}
		else if (tokens[countIndex] == "exit"){
			/*
			-> 'exit'					=> "exit"
			-> 'return' Expression			=> "return"
			*/
			advanceTokenCount();
			makeTree("exit",0);

		}
		else if (tokens[countIndex] == "return"){
			advanceTokenCount();
			Expression();
			makeTree("return",1);

		}
		else if (tokens[countIndex] == "begin"){//body() cause it begins with "begin"
			Body();
		}
		else{
			//null portion, nothing matches so done? do i advance?
			makeTree("<null>",0);
		}


	}

	/*
	OutExp     -> Expression				=> "integer"
           -> StringNode				=> "string";
	*/
	void OutExp(){
		if (checkIfIdentifierCharStringOrInteger(tokens[countIndex]) == "string"){
			StringNode();
			makeTree("string",1);
		}
		else{
			Expression();
			makeTree("integer",1);
		}
	}

	/*
	StringNode -> '<string>';

	*/
	void StringNode(){
		advanceTokenCount();
		makeTree("<string>", 0);
		//this might be weird

	}

	/*
	Caseclauses-> (Caseclause ';')+;
	*/
	int Caseclauses(){
	//void Caseclauses(){
		//1 or more
		int caseCount = 0;

		Caseclause();
		if (tokens[countIndex] == ";"){
			advanceTokenCount();
		}
		while (checkIfIdentifierCharStringOrInteger(tokens[countIndex]) == "identifier" || checkIfIdentifierCharStringOrInteger(tokens[countIndex]) == "integer" || checkIfIdentifierCharStringOrInteger(tokens[countIndex]) == "char"){//might need more?
			Caseclause();
			caseCount++;	//because the loop for CaseClause needs to returned to a different funciton
			if (tokens[countIndex] == ";"){
				advanceTokenCount();
			}

		}
		return caseCount;

	}

	/*
	Caseclause -> CaseExpression list ',' ':' Statement     => "case_clause";
	*/
	void Caseclause(){
		int count = 1;
		while (tokens[countIndex] != ":"){
			CaseExpression();
			count++;
			if (tokens[countIndex] == ","){
				advanceTokenCount();
			}
		}
		advanceTokenCount();		//forgot this earlier.
		Statement();
		makeTree("case_clause",count);

	}

	/*
	CaseExpression -> ConstValue
	-> ConstValue '..' ConstValue		=> "..";
	*/
	void CaseExpression(){
		ConstValue();
		if (tokens[countIndex] == ".."){
			advanceTokenCount();
			ConstValue();
			makeTree("..",2);
		}
	}

	/*
	OtherwiseClause -> 'otherwise' Statement                => "otherwise"
                -> ;
	*/
	int OtherwiseClause(){
		int toReturn = 0;
		if (tokens[countIndex] == "otherwise"){
			toReturn = 1;
			advanceTokenCount();
			Statement();
			makeTree("otherwise",1);
		}
		//?just a semicolon?
		return toReturn;
	}

	/*
	Assignment -> Name ':=' Expression  	        	=> "assign"
           -> Name ':=:' Name                           => "swap"; 
	*/
	void Assignment(){
		Name();
		if (tokens[countIndex] == ":="){
			advanceTokenCount();
			Expression();
			makeTree("assign",2);
		}
		else{//:=: but currently parser can't handle this
			advanceTokenCount();
			Name();
			makeTree("swap",2);
		}

	}

	/*
	ForStat    -> Assignment
           -> 						=> "<null>";
	*/
	void ForStat(){
		if (checkIfIdentifierCharStringOrInteger(tokens[countIndex]) == "identifier"){
			Assignment();
		}
		else{
			makeTree("<null>",0);
		}
	}

	/*
	ForExp     -> Expression
           -> 						=> "true";
	*/
	void ForExp(){
		if (expHelper(tokens[countIndex])){
			Expression();
		}
		else{
			makeTree("true",0);
		}

	}

	//literally just to make ForExp cleaner.
	bool expHelper(string s){
		if (s == "-" || s == "+" || s == "not" || s == "eof" || s == "succ" || s == "pred" || s == "chr" || s == "ord" || s == "("){
			return true;
		}
		else if (checkIfIdentifierCharStringOrInteger(s) == "identifier" || checkIfIdentifierCharStringOrInteger(s) == "integer" || checkIfIdentifierCharStringOrInteger(s) == "char"){
			return true;
		}
		else{
			return false;
		}
	}
	//want helper function to tell if expression
	/*
	Expression -> Term	                         		
	   -> Term '<=' Term				=> "<="
	   -> Term '<' Term				=> "<"
	   -> Term '>=' Term				=> ">="
	   -> Term '>' Term				=> ">"
	   -> Term '=' Term				=> "="
	   -> Term '<>' Term				=> "<>";
	*/
	void Expression(){
		Term();
		if (tokens[countIndex] == "<="){
			advanceTokenCount();
			Term();
			makeTree("<=",2);
		}
		else if (tokens[countIndex] == "<"){
			advanceTokenCount();
			Term();
			makeTree("<",2);
		}
		else if (tokens[countIndex] == ">="){
			advanceTokenCount();
			Term();
			makeTree(">=",2);
		}
		else if (tokens[countIndex] == ">"){
			advanceTokenCount();
			Term();
			makeTree(">",2);
		}
		else if (tokens[countIndex] == "="){
			advanceTokenCount();
			Term();
			makeTree("=",2);
		}
		else if (tokens[countIndex] == "<>"){
			advanceTokenCount();
			Term();
			makeTree("<>",2);
		}

	}

	/*
	Term       -> Factor 
	   -> Term '+' Factor				=> "+"
	   -> Term '-' Factor				=> "-"
	   -> Term 'or' Factor				=> "or";
	*/
	void Term(){
		//i forgot the name of thing but I can do this due to rules
		Factor();

		while (tokens[countIndex] == "+" || tokens[countIndex] == "-" || tokens[countIndex] == "or"){
			if (tokens[countIndex] == "+"){
				advanceTokenCount();
				Term();
				Factor();
				makeTree("+",2);
			}
			else if (tokens[countIndex] == "-"){
				advanceTokenCount();
				Term();
				Factor();
				makeTree("-",2);
			}
			else if (tokens[countIndex] == "or"){
				advanceTokenCount();
				Term();
				Factor();
				makeTree("or",2);
			}
		}

	}

	/*
	Factor     -> Factor '*' Primary			=> "*"
	   -> Factor '/' Primary			=> "/"
	   -> Factor 'and' Primary			=> "and"
	   -> Factor 'mod' Primary			=> "mod"
	   -> Primary;
	*/
	void Factor(){
		Primary();

		while (tokens[countIndex] == "*" || tokens[countIndex] == "/" || tokens[countIndex] == "and" || tokens[countIndex] == "mod"){
			if (tokens[countIndex] == "*"){
				advanceTokenCount();
				
				Factor();
				Primary();
				makeTree("*",2);
			}
			else if (tokens[countIndex] == "/"){
				advanceTokenCount();
				Factor();
				Primary();
				makeTree("/",2);
			}
			else if (tokens[countIndex] == "and"){
				advanceTokenCount();
				Factor();
				Primary();
				makeTree("and",2);
			}
			else if (tokens[countIndex] == "mod"){
				advanceTokenCount();
				Factor();
				Primary();
				makeTree("mod",2);
			}
		}

	}

	/*
	Primary    -> '-' Primary                            	=> "-"
	   -> '+' Primary				
	   -> 'not' Primary				=> "not"
	   -> 'eof'                                     => "eof"
           -> Name
           -> '<integer>'
	   -> '<char>'
	   -> Name '(' Expression list ',' ')'		=> "call"
           -> '(' Expression ')'
	   -> 'succ' '(' Expression ')' 		=> "succ"
	   -> 'pred' '(' Expression ')' 		=> "pred"
	   -> 'chr' '(' Expression ')' 			=> "chr"
	   -> 'ord' '(' Expression ')' 			=> "ord";
	*/
	void Primary(){
		if (tokens[countIndex] == "-"){
			advanceTokenCount();
			Primary();
			makeTree("-",1);
		}
		else if (tokens[countIndex] == "+"){
			advanceTokenCount();
			Primary();
			
		}
		else if (tokens[countIndex] == "not"){
			advanceTokenCount();
			Primary();
			makeTree("not",1);
		}
		else if (tokens[countIndex] == "eof"){
			advanceTokenCount();
			
			makeTree("eof",0);
		}
		else if (tokens[countIndex] == "succ"){
			advanceTokenCount();
			if (tokens[countIndex] == "("){
				advanceTokenCount();
				Expression();

				if (tokens[countIndex] == ")"){
					advanceTokenCount();
					makeTree("succ",1);
				}
			}

		}
		else if (tokens[countIndex] == "pred"){
			advanceTokenCount();
			if (tokens[countIndex] == "("){
				advanceTokenCount();
				Expression();

				if (tokens[countIndex] == ")"){
					advanceTokenCount();
					makeTree("pred",1);
				}
			}

		}
		else if (tokens[countIndex] == "chr"){
			advanceTokenCount();
			if (tokens[countIndex] == "("){
				advanceTokenCount();
				Expression();

				if (tokens[countIndex] == ")"){
					advanceTokenCount();
					makeTree("chr",1);
				}
			}

		}
		else if (tokens[countIndex] == "ord"){
			advanceTokenCount();
			if (tokens[countIndex] == "("){
				advanceTokenCount();
				Expression();

				if (tokens[countIndex] == ")"){
					advanceTokenCount();
					makeTree("ord",1);
				}
			}

		}
		else if (tokens[countIndex] == "("){
			advanceTokenCount();
			Expression();

			if (tokens[countIndex] == ")"){
				advanceTokenCount();
				
			}
		}
		else if (checkIfIdentifierCharStringOrInteger(tokens[countIndex]) == "char"){
			makeTree(tokens[countIndex], 0);
			advanceTokenCount();

			makeTree("<char>",1);
			
		}
		else if (checkIfIdentifierCharStringOrInteger(tokens[countIndex]) == "integer"){
			makeTree(tokens[countIndex], 0);
			advanceTokenCount();
			makeTree("<integer>",1);

		}
		else if (checkIfIdentifierCharStringOrInteger(tokens[countIndex]) == "identifier"){

			Name();
			int count = 1;
			if (tokens[countIndex] == "("){
				
				advanceTokenCount();

				
				while (tokens[countIndex] != ")"){
					Expression();
					if (tokens[countIndex] == ","){
						advanceTokenCount();
					}
					count++;
				}
				advanceTokenCount();		//added this cause ) shouldn't have been there i think.
				makeTree("call",count);
			}
			
		}


	}

	/*
	Name       -> '<identifier>';
	*/
	void Name(){
		if (checkIfIdentifierCharStringOrInteger(tokens[countIndex]) == "identifier"){
			makeTree(tokens[countIndex],0);
			makeTree("<identifier>",1);
		}
		advanceTokenCount();
	}

	void advanceTokenCount(){
		//cout << "Got to position:" << countIndex << " and see " << tokens[countIndex] << ".\n";
		countIndex++;
	}

	//you know how sometimes you make a mistake and it would be more of a pain to fix it than to just deal with it?
	//that's this method. since I didn't do tokens I check stuff here. 
	string checkIfIdentifierCharStringOrInteger(string s){

		bool has_only_digits = (s.find_first_not_of("0123456789") == string::npos);
		char* ptr;
		ptr = &s.at(0);
		char fl = s.at(0);
		if (has_only_digits){
			return "integer";
		}
		/*else if (ptr == "'"){
			return "char";
		}*/
		else if (fl == '\''){
			return "char";
		}
		else if (ptr == "\""){
			return "string";
		}
		else if(checkHelper(s)){
			return "identifier";
		}
		else{
			return "null";	//idk
		}

		//there is actually no case in all the test files for string???
		//but I'm assuming it's  " "  " seen
	}

	//i messed up earlier so this is a patchwork fix
	bool checkHelper(string s){
		if (s == "program" || s == ":" || s == "." || s == ";" || s == "=" || s == "type" || s == "function" || s == "var" || s == "begin" || s == "end" || s == "output" || s == "if" || s == "then" || s == "else" || s == "while" || s == "repeat" || s == "otherwise" || s == "const" || s == "begin" || s == "end" || s == ","){
			return false;
		}
		else if (s == "until" || s == "for" || s == "loop" || s == "pool" || s == "case" || s == "of" || s == "read" || s == "exit" || s == "return" || s == ":=" || s == ":=:" || s == "<=" || s == "<" || s == ">=" || s == ">" || s == "=" || s == ".."){
			return false;
		}
		else if(s == "<>" || s == "+" || s == "-" || s == "or" || s == "*" || s == "/" || s == "and" || s == "mod" || s == "not" || s == "eof" || s == "succ" || s == "pred" || s == "chr" || s == "ord" || s == "(" || s == ")" || s == "do"){
			return false;
		}
		else{
			return true;
		}
	}

	void makeTree(string typeOfNode, int popOff){
		
		//this is pretty much it lmao
		Node* node = new Node(typeOfNode);	
		//Tree f(&node);
		//f = mystack.pop();

		if (popOff != 0){
			for (int i = 0; i < popOff; i++){
				node->addTree(mystack.top());
				mystack.pop();
				//node.addTree(&f);
			}
		}
		//Tree tobePushed(node);
		//mystack.push(tobePushed);		//probs fine
		mystack.push(node);
	}


};