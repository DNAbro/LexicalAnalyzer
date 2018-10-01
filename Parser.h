#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>


class Parser{

private:
	//ifstream myFile;			//The File that will be read.
	string filePath;			//get the filePath, maybe?
	string line;				//current line that is being read
	//char str[];
	string tokenHolder[1024];
	int count;
	bool commentIsOn;			//will not record parsing for coments
	bool commentIsOnTillEndOfLine;

public:
	Parser(){}					//blank instantiation

	Parser(string a){
		filePath = a;
		count = 0;
		commentIsOn = false;
		commentIsOnTillEndOfLine = false;
		//ifstream myFile(a);		//idk if this works
	}

	//char * strtok ( char * str, const char * delimiters );

	void readOutFile(){
		ifstream myFile(filePath);
		if (myFile.is_open()){

			
			
			while (getline(myFile, line))
			{
				/*string temp = line;
				std::string::iterator end_pos = std::remove(temp.begin(), temp.end(), ' ');
				temp.erase(end_pos, temp.end());*/

				if (commentIsOnTillEndOfLine){
					commentIsOnTillEndOfLine = false;
					commentIsOn = false;			//note be wary of it reading the 
				}
				
				char strToBeToken[1024];
				strncpy(strToBeToken, line.c_str(), sizeof(strToBeToken));
				//strncpy(strToBeToken, temp.c_str(), sizeof(strToBeToken));
				strToBeToken[sizeof(strToBeToken)-1] = 0;
				
				//char str[] = (char)line;
				char * pch;
				pch = strtok(strToBeToken, " ");	//note will probably need other delimiters like ; 

				//ask question about := and :
				while (pch != NULL)
				{


					tokenHolder[count] = pch;
					string temp = tokenHolder[count];		//assigns whatever pch is to string temp


					



					//removes tabs from the the string
					std::string::iterator end_pos = std::remove(temp.begin(), temp.end(), '\t');
					temp.erase(end_pos, temp.end());

					//////////////////////////////////
					//need to make sure it doesn't add to tokenHolder when there is a {}
					std::string beginComment = "{";
					std::string endComment = "}";
					std::size_t findBegin = temp.find(beginComment);
					std::size_t findEnd = temp.find(endComment);
					if (findBegin != std::string::npos){
						commentIsOn = true;
					}
					/////////////////////////////////////////

					std::string otherKindOfComment = "#";
					std::size_t findCom = temp.find(otherKindOfComment);

					if (findCom != std::string::npos && !commentIsOn){
						commentIsOn = true;
						commentIsOnTillEndOfLine = true;
					}

					////////////////////////////////////////////////////

					std::string leftPar = "(";
					std::string rightPar = ")";
					std::size_t foundLeftPar = temp.find(leftPar);

					if (foundLeftPar != std::string::npos && temp != leftPar && !commentIsOn){
						
						if (foundLeftPar != 0){		//if there is something before the left parenthensis

							std::string beforeLeftPar = temp.substr(0, foundLeftPar);
							//left parenthis is already a string above
							std::string afterLeftPar = temp.substr(foundLeftPar + 1, temp.size());

							sendItThroughAgain(beforeLeftPar, true);
							//tokenHolder[count] = beforeLeftPar;
							//cout << count << ": " << tokenHolder[count] << '\n';
							//count++;

							tokenHolder[count] = leftPar;
							//cout << count << ": " << tokenHolder[count] << '\n';
							count++;

							//temp = afterLeftPar;
							temp = sendItThroughAgain(afterLeftPar, false);
						}
						else{		//if there isn't anything before it, it doesn't need a before
							std::string afterLeftPar = temp.substr(foundLeftPar + 1, temp.size());

							tokenHolder[count] = leftPar;
							//cout << count << ": " << tokenHolder[count] << '\n';
							count++;

							//temp = afterLeftPar;
							temp = sendItThroughAgain(afterLeftPar, false);

						}
					}

					//////////////////////////////////////////////////////////////////





		//FIXME THERE MAY BE MAJOR ISSUES FOR THE ORDER OF THINGS LIKE + - * := =


					//:= versus ://////////////////////////////////////////////////////////

					std::string colon(":");
					std::size_t foundColon = temp.find(colon);

					if (foundColon != std::string::npos && temp != colon && !commentIsOn){

						std::string str2(":=");
						std::size_t found = temp.find(str2);
						std::size_t end = found + 1;		//reasons

						if (found != std::string::npos && temp != str2 && !commentIsOn){		//check if it := is by itself

							//std::cout << "found ':=' -> " << found << '\n';
							std::string before = temp.substr(0, found);
							std::string colEq = temp.substr(found, end);
							std::string after = temp.substr(end + 1, temp.size());

							if (before != ""){
								//tokenHolder[count] = before;
								//cout << count << ": " << tokenHolder[count] << '\n';
								//count++;
								sendItThroughAgain(before, true);
							}

							colEq = ":=";
							tokenHolder[count] = colEq;
							//cout << count << ": " << tokenHolder[count] << '\n';
							count++;

							/*tokenHolder[count] = before;
							cout << count << ": " << tokenHolder[count] << '\n';
							count++;*/
							//temp = after;
							temp = sendItThroughAgain(after, false);
							//will be set right after this

						}
						else if(temp != "" && temp != str2){			//needs to check for if := is by itself so it doesn't try and think it's just ':'
							std::string before = temp.substr(0, foundColon);
							std::string after = temp.substr(foundColon + 1, temp.size());

							sendItThroughAgain(before, true);
							//tokenHolder[count] = before;
							//cout << count << ": " << tokenHolder[count] << '\n';
							//count++;

							tokenHolder[count] = colon;
							//cout << count << ": " << tokenHolder[count] << '\n';
							count++;

							//temp = after;
							temp = sendItThroughAgain(after, false);
						}

					}
					//////////////////////////////////////////////////////////////////////////////
					

					//////////////////////////////////////////////////////////////////////////////

					std::string greaterThan("<");
					std::size_t foundGT = temp.find(greaterThan);
					if (foundGT != std::string::npos && temp != greaterThan && !commentIsOn && temp != ""){
						std::string str2("<=");
						std::size_t found = temp.find(str2);
						std::size_t end = found + 1;		//reasons

						std::string str3("<>");
						std::size_t found3 = temp.find(str3);
						std::size_t end3 = found + 1;		//reasons
						if (found != std::string::npos && temp != str2 && !commentIsOn){
							//std::cout << "found ':=' -> " << found << '\n';
							std::string before = temp.substr(0, found);
							std::string doub = temp.substr(found, end);
							std::string after = temp.substr(end + 1, temp.size());

							if (before != ""){
								//tokenHolder[count] = before;
								//cout << count << ": " << tokenHolder[count] << '\n';
								//count++;
								sendItThroughAgain(before, true);
							}

							doub = "<=";
							tokenHolder[count] = doub;
							//cout << count << ": " << tokenHolder[count] << '\n';
							count++;

							/*tokenHolder[count] = before;
							cout << count << ": " << tokenHolder[count] << '\n';
							count++;*/
							//temp = after;
							temp = sendItThroughAgain(after, false);
							//will be set right after this
						}
						else if (found3 != std::string::npos && temp != str3 && !commentIsOn){
							std::string before = temp.substr(0, found);
							std::string doub = temp.substr(found, end3);
							std::string after = temp.substr(end3 + 1, temp.size());

							if (before != ""){
								//tokenHolder[count] = before;
								//cout << count << ": " << tokenHolder[count] << '\n';
								//count++;
								sendItThroughAgain(before, true);
							}

							doub = "<>";
							tokenHolder[count] = doub;
							//cout << count << ": " << tokenHolder[count] << '\n';
							count++;

							/*tokenHolder[count] = before;
							cout << count << ": " << tokenHolder[count] << '\n';
							count++;*/
							//temp = after;
							temp = sendItThroughAgain(after, false);
							//will be set right after this

						}
						else if (temp != "" && temp != str2 && temp != str3){
							std::string before = temp.substr(0, foundGT);
							std::string after = temp.substr(foundGT + 1, temp.size());

							sendItThroughAgain(before, true);
							//tokenHolder[count] = before;
							//cout << count << ": " << tokenHolder[count] << '\n';
							//count++;

							tokenHolder[count] = greaterThan;
							//cout << count << ": " << tokenHolder[count] << '\n';
							count++;

							//temp = after;
							temp = sendItThroughAgain(after, false);
						}
					}

					std::string lessThan(">");
					std::size_t foundLT = temp.find(lessThan);
					if (foundLT != std::string::npos && temp != lessThan && !commentIsOn && temp != ""){

						std::string str2(">=");
						std::size_t found = temp.find(str2);
						std::size_t end = found + 1;		//reasons
						if (found != std::string::npos && temp != str2 && !commentIsOn){
							//std::cout << "found ':=' -> " << found << '\n';
							std::string before = temp.substr(0, found);
							std::string doub = temp.substr(found, end);
							std::string after = temp.substr(end + 1, temp.size());

							if (before != ""){
								//tokenHolder[count] = before;
								//cout << count << ": " << tokenHolder[count] << '\n';
								//count++;
								sendItThroughAgain(before, true);
							}

							doub = ">=";
							tokenHolder[count] = doub;
							//cout << count << ": " << tokenHolder[count] << '\n';
							count++;

							/*tokenHolder[count] = before;
							cout << count << ": " << tokenHolder[count] << '\n';
							count++;*/
							//temp = after;
							temp = sendItThroughAgain(after, false);
							//will be set right after this
						}
						else if (temp != "" && temp != str2){
							std::string before = temp.substr(0, foundLT);
							std::string after = temp.substr(foundLT + 1, temp.size());

							sendItThroughAgain(before, true);
							//tokenHolder[count] = before;
							//cout << count << ": " << tokenHolder[count] << '\n';
							//count++;

							tokenHolder[count] = lessThan;
							//cout << count << ": " << tokenHolder[count] << '\n';
							count++;

							//temp = after;
							temp = sendItThroughAgain(after, false);
						}


					}

					/////////////////////////////////////////////////////////////////////////////////
					std::string equal("=");
					std::size_t foundEqual = temp.find(equal);

					if (foundEqual != std::string::npos && temp != equal && !commentIsOn && temp != ":=" && temp != "<=" && temp != ">="){

						std::string before = temp.substr(0, foundEqual);
						//add already exists
						std::string after = temp.substr(foundEqual + 1, temp.size());

						sendItThroughAgain(before, true);
						//tokenHolder[count] = before;
						//cout << count << ": " << tokenHolder[count] << '\n';
						//count++;

						tokenHolder[count] = equal;
						//cout << count << ": " << tokenHolder[count] << '\n';
						count++;

						//temp = after;
						temp = sendItThroughAgain(after, false);
					}


					////////////////////////////////////////////////////////////////////////////////

					std::string add("+");
					std::size_t foundAdd = temp.find(add);
					if (foundAdd != std::string::npos && temp != add && !commentIsOn && temp != ""){
						std::string before = temp.substr(0, foundAdd);
						//add already exists
						std::string after = temp.substr(foundAdd + 1, temp.size());

						//tokenHolder[count] = before;
						//cout << count << ": " << tokenHolder[count] << '\n';
						//count++;
						sendItThroughAgain(before, true);

						tokenHolder[count] = add;
						//cout << count << ": " << tokenHolder[count] << '\n';
						count++;

						//temp = after;
						temp = sendItThroughAgain(after, false);
					}

					///////////////////////////////////////////////////////////////////////////////////

					std::string mult("*");
					std::size_t foundMult = temp.find(mult);
					if (foundMult != std::string::npos && temp != mult && !commentIsOn && temp != ""){
						std::string before = temp.substr(0, foundMult);
						//add already exists
						std::string after = temp.substr(foundMult + 1, temp.size());

						//tokenHolder[count] = before;
						//cout << count << ": " << tokenHolder[count] << '\n';
						//count++;
						sendItThroughAgain(before, true);

						tokenHolder[count] = mult;
						//cout << count << ": " << tokenHolder[count] << '\n';
						count++;

						//temp = after;
						temp = sendItThroughAgain(after, false);
					}

					///////////////////////////////////////////////////////////////////////////////////
					std::string subtract("-");
					std::size_t foundSubtract = temp.find(subtract);
					if (foundSubtract != std::string::npos && temp != subtract && !commentIsOn && temp != ""){
						std::string before = temp.substr(0, foundSubtract);
						//add already exists
						std::string after = temp.substr(foundSubtract + 1, temp.size());

						sendItThroughAgain(before,true);
						//tokenHolder[count] = before;
						//cout << count << ": " << tokenHolder[count] << '\n';
						//count++;

						tokenHolder[count] = subtract;
						//cout << count << ": " << tokenHolder[count] << '\n';
						count++;

						//temp = after;
						temp = sendItThroughAgain(after, false);
					}

					////////////////////////////////////////////////////////////////////////////

					std::string division("/");
					std::size_t foundDiv = temp.find(division);
					if (foundDiv != std::string::npos && temp != division && !commentIsOn && temp != ""){
						std::string before = temp.substr(0, foundDiv);
						//add already exists
						std::string after = temp.substr(foundDiv + 1, temp.size());

						//tokenHolder[count] = before;
						//cout << count << ": " << tokenHolder[count] << '\n';
						//count++;
						sendItThroughAgain(before, true);

						tokenHolder[count] = division;
						//cout << count << ": " << tokenHolder[count] << '\n';
						count++;

						//temp = after;
						temp = sendItThroughAgain(after, false);
					}


					








					////////////////////////////////////////////////////////////////
					std::string comma = ",";
					std::size_t foundComma = temp.find(comma);

					if (foundComma != std::string::npos && !commentIsOn && temp != comma){
						std::string beforeComma = temp.substr(0, foundComma);
						std::string afterComma = temp.substr(foundComma + 1);

						//tokenHolder[count] = beforeComma;
						//tokenHolder[count] = sendItThroughAgain(beforeComma); //go to bottom and fix it, no need for equate i think
						sendItThroughAgain(beforeComma,true);

						//cout << count << ": " << tokenHolder[count] << '\n';
						//count++;

						tokenHolder[count] = comma;
						//cout << count << ": " << tokenHolder[count] << '\n';
						count++;

						//temp = afterComma;
						temp = sendItThroughAgain(afterComma, false);
						//FIXME BELOW

					}



					///////////////////////////////////////////////////////////////////
					



					//end of Right Parenthesis
					std::size_t foundRightPar = temp.find(rightPar);

					if (foundRightPar != std::string::npos && temp != rightPar && !commentIsOn && temp != ""){

						if (foundRightPar != 0){		//

							std::string beforeRightPar = temp.substr(0, foundRightPar);
							//right parenthis is already a string above
							std::string afterRightPar = temp.substr(foundRightPar + 1, temp.size());

							sendItThroughAgain(beforeRightPar, true);
							//tokenHolder[count] = beforeRightPar;
							//cout << count << ": " << tokenHolder[count] << '\n';
							//count++;

							tokenHolder[count] = rightPar;
							//cout << count << ": " << tokenHolder[count] << '\n';
							count++;

							//temp = afterRightPar;
							temp = sendItThroughAgain(afterRightPar, false);
						}
						else{		//if there isn't anything before it, it doesn't need a before
							std::string afterRightPar = temp.substr(foundRightPar + 1, temp.size());

							tokenHolder[count] = rightPar;
							//cout << count << ": " << tokenHolder[count] << '\n';
							count++;

							//testing this shiz
							//temp = afterRightPar;
							temp = sendItThroughAgain(afterRightPar, false);
							

						}
					}


					///////////////////////////////////////////////////////////////

					//If there is a ; at the end, remove it and make it it's own token
					std::string semi = ";";
					std::size_t foundSemi = temp.find(semi);

					if (foundSemi != std::string::npos && !commentIsOn && temp != semi){	//so above doesn't accidently add a blank
						std::string beforeSemi = temp.substr(0, foundSemi);
						std::string afterSemi = temp.substr(foundSemi + 1, temp.size());
						sendItThroughAgain(beforeSemi, true);
						//tokenHolder[count] = beforeSemi;
						//cout << count << ": " << tokenHolder[count] << '\n';
						//count++;
						tokenHolder[count] = semi;
						//cout << count << ": " << tokenHolder[count] << '\n';
						count++;

						//temp = semi;		//makes the string a ";"
						temp = sendItThroughAgain(afterSemi, false);
					}

					/*std::string comma = ",";
					std::size_t foundComma = temp.find(comma);
					/*
					if (foundComma != std::string::npos && !commentIsOn && temp != comma){
						std::string beforeComma = temp.substr(0, foundComma);
						std::string afterComma = temp.substr(foundComma + 1);
						tokenHolder[count] = beforeComma;
						cout << count << ": " << tokenHolder[count] << '\n';
						count++;

						tokenHolder[count] = comma;
						count++;

						temp = afterComma;
						//FIXME BELOW

					}*/
					//////////////////////////////////////////////////////////////////
					//There is '.' and '..'

					std::string period(".");
					std::size_t foundPeriod = temp.find(period);

					if (foundPeriod != std::string::npos && temp != period && !commentIsOn){

						std::string str2("..");
						std::size_t found = temp.find(str2);
						std::size_t end = found + 1;		//reasons

						if (found != std::string::npos && temp != str2 && !commentIsOn){		//check if it := is by itself

							//std::cout << "found ':=' -> " << found << '\n';
							std::string before = temp.substr(0, found);
							std::string doubPeriod = temp.substr(found, end);
							std::string after = temp.substr(end + 1, temp.size());

							tokenHolder[count] = before;
							//cout << count << ": " << tokenHolder[count] << '\n';
							count++;

							tokenHolder[count] = doubPeriod;
							//cout << count << ": " << tokenHolder[count] << '\n';
							count++;

							/*tokenHolder[count] = before;
							cout << count << ": " << tokenHolder[count] << '\n';
							count++;*/
							temp = after;
							//will be set right after this

						}
						else if (temp != "" && temp != str2){			//needs to check for if := is by itself so it doesn't try and think it's just ':'
							std::string before = temp.substr(0, foundPeriod);

							tokenHolder[count] = before;
							//cout << count << ": " << tokenHolder[count] << '\n';
							count++;

							temp = period;
						}

					}



					//records the latest token, most general case
					if (!commentIsOn && temp != ""){			//had to add "" clause cause for some reason a space was getting through
					tokenHolder[count] = temp;

					//printf("%s\n", pch);
					//cout << count << ": " << tokenHolder[count] << '\n';

					count++;
					}

					//move this to the end so it doesn't grab the end }
					if (findEnd != std::string::npos){
						commentIsOn = false;
					}

					

					/*if (commentIsOnTillEndOfLine){

						if (temp == line.end()){
							cout << "I'm empty";
						
						}
						/*if (findEndOfLine != std::string::npos){
					
							commentIsOnTillEndOfLine = false;
							commentIsOn = false;
						}
						
					}*/
					
					pch = strtok(NULL, " ");
				}
				//cout << line << '\n';*/
			}
			myFile.close();
		}
		else{
			cout << "Could not read file or maybe I failed in find it idk";
		}
	}


	void testFunction(){
		int a;
		std::cout << "I am testing class stuff.";
		std::cin >> a;
	}

	string* getTokenHolder(){
		return tokenHolder;
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
















	//hacky way of making it check again. Literally same block of code as above but I'm afraid to break something so i just copied it instead of replacing above with the function
	string sendItThroughAgain(string temp, bool onLeftSide){


		//std::string::iterator end_pos = std::remove(temp.begin(), temp.end(), '\t');
		//temp.erase(end_pos, temp.end());

		//////////////////////////////////
		//need to make sure it doesn't add to tokenHolder when there is a {}
		std::string beginComment = "{";
		std::string endComment = "}";
		std::size_t findBegin = temp.find(beginComment);
		std::size_t findEnd = temp.find(endComment);
		if (findBegin != std::string::npos){
			commentIsOn = true;
		}
		/////////////////////////////////////////

		std::string otherKindOfComment = "#";
		std::size_t findCom = temp.find(otherKindOfComment);

		if (findCom != std::string::npos && !commentIsOn){
			commentIsOn = true;
			commentIsOnTillEndOfLine = true;
		}

		////////////////////////////////////////////////////

		std::string leftPar = "(";
		std::string rightPar = ")";
		std::size_t foundLeftPar = temp.find(leftPar);

		if (foundLeftPar != std::string::npos && temp != leftPar && !commentIsOn){

			if (foundLeftPar != 0){		//if there is something before the left parenthensis

				std::string beforeLeftPar = temp.substr(0, foundLeftPar);
				//left parenthis is already a string above
				std::string afterLeftPar = temp.substr(foundLeftPar + 1, temp.size());

				sendItThroughAgain(beforeLeftPar, true);
				//tokenHolder[count] = beforeLeftPar;
				//cout << count << ": " << tokenHolder[count] << '\n';
				//count++;

				tokenHolder[count] = leftPar;
				//cout << count << ": " << tokenHolder[count] << '\n';
				count++;

				//temp = afterLeftPar;
				temp = sendItThroughAgain(afterLeftPar, false);
			}
			else{		//if there isn't anything before it, it doesn't need a before
				std::string afterLeftPar = temp.substr(foundLeftPar + 1, temp.size());

				tokenHolder[count] = leftPar;
				//cout << count << ": " << tokenHolder[count] << '\n';
				count++;

				//temp = afterLeftPar;
				temp = sendItThroughAgain(afterLeftPar, false);


			}
		}

		//////////////////////////////////////////////////////////////////





		//FIXME THERE MAY BE MAJOR ISSUES FOR THE ORDER OF THINGS LIKE + - * := =


		//:= versus ://////////////////////////////////////////////////////////

		std::string colon(":");
		std::size_t foundColon = temp.find(colon);

		if (foundColon != std::string::npos && temp != colon && !commentIsOn){

			std::string str2(":=");
			std::size_t found = temp.find(str2);
			std::size_t end = found + 1;		//reasons

			if (found != std::string::npos && temp != str2 && !commentIsOn){		//check if it := is by itself

				//std::cout << "found ':=' -> " << found << '\n';
				std::string before = temp.substr(0, found);
				std::string colEq = temp.substr(found, end);
				std::string after = temp.substr(end + 1, temp.size());

				if (before != ""){
					//tokenHolder[count] = before;
					//cout << count << ": " << tokenHolder[count] << '\n';
					//count++;
					sendItThroughAgain(before, true);
				}

				colEq = ":=";
				tokenHolder[count] = colEq;
				//cout << count << ": " << tokenHolder[count] << '\n';
				count++;

				/*tokenHolder[count] = before;
				cout << count << ": " << tokenHolder[count] << '\n';
				count++;*/
				//temp = after;
				temp = sendItThroughAgain(after, false);
				//will be set right after this

			}
			else if (temp != "" && temp != str2){			//needs to check for if := is by itself so it doesn't try and think it's just ':'
				std::string before = temp.substr(0, foundColon);
				std::string after = temp.substr(foundColon + 1, temp.size());

				sendItThroughAgain(before, true);
				//tokenHolder[count] = before;
				//cout << count << ": " << tokenHolder[count] << '\n';
				//count++;

				tokenHolder[count] = colon;
				//cout << count << ": " << tokenHolder[count] << '\n';
				count++;

				//temp = after;
				temp = sendItThroughAgain(after, false);
			}

		}
		//////////////////////////////////////////////////////////////////////////////
		

		///////////////////////////////////////////////////////////////////////////////
		std::string greaterThan("<");
		std::size_t foundGT = temp.find(greaterThan);
		if (foundGT != std::string::npos && temp != greaterThan && !commentIsOn && temp != ""){
			std::string str2("<=");
			std::size_t found = temp.find(str2);
			std::size_t end = found + 1;		//reasons

			std::string str3("<>");
			std::size_t found3 = temp.find(str3);
			std::size_t end3 = found + 1;		//reasons
			if (found != std::string::npos && temp != str2 && !commentIsOn){
				//std::cout << "found ':=' -> " << found << '\n';
				std::string before = temp.substr(0, found);
				std::string doub = temp.substr(found, end);
				std::string after = temp.substr(end + 1, temp.size());

				if (before != ""){
					//tokenHolder[count] = before;
					//cout << count << ": " << tokenHolder[count] << '\n';
					//count++;
					sendItThroughAgain(before, true);
				}

				doub = "<=";
				tokenHolder[count] = doub;
				//cout << count << ": " << tokenHolder[count] << '\n';
				count++;

				/*tokenHolder[count] = before;
				cout << count << ": " << tokenHolder[count] << '\n';
				count++;*/
				//temp = after;
				temp = sendItThroughAgain(after, false);
				//will be set right after this
			}
			else if (found3 != std::string::npos && temp != str3 && !commentIsOn){
				std::string before = temp.substr(0, found);
				std::string doub = temp.substr(found, end3);
				std::string after = temp.substr(end3 + 1, temp.size());

				if (before != ""){
					//tokenHolder[count] = before;
					//cout << count << ": " << tokenHolder[count] << '\n';
					//count++;
					sendItThroughAgain(before, true);
				}

				doub = "<>";
				tokenHolder[count] = doub;
				//cout << count << ": " << tokenHolder[count] << '\n';
				count++;

				/*tokenHolder[count] = before;
				cout << count << ": " << tokenHolder[count] << '\n';
				count++;*/
				//temp = after;
				temp = sendItThroughAgain(after, false);
				//will be set right after this

			}
			else if (temp != "" && temp != str2 && temp != str3){
				std::string before = temp.substr(0, foundGT);
				std::string after = temp.substr(foundGT + 1, temp.size());

				sendItThroughAgain(before, true);
				//tokenHolder[count] = before;
				//cout << count << ": " << tokenHolder[count] << '\n';
				//count++;

				tokenHolder[count] = greaterThan;
				//cout << count << ": " << tokenHolder[count] << '\n';
				count++;

				//temp = after;
				temp = sendItThroughAgain(after, false);
			}
		}

		std::string lessThan(">");
		std::size_t foundLT = temp.find(lessThan);
		if (foundLT != std::string::npos && temp != lessThan && !commentIsOn && temp != ""){

			std::string str2(">=");
			std::size_t found = temp.find(str2);
			std::size_t end = found + 1;		//reasons
			if (found != std::string::npos && temp != str2 && !commentIsOn){
				//std::cout << "found ':=' -> " << found << '\n';
				std::string before = temp.substr(0, found);
				std::string doub = temp.substr(found, end);
				std::string after = temp.substr(end + 1, temp.size());

				if (before != ""){
					//tokenHolder[count] = before;
					//cout << count << ": " << tokenHolder[count] << '\n';
					//count++;
					sendItThroughAgain(before, true);
				}

				doub = ">=";
				tokenHolder[count] = doub;
				//cout << count << ": " << tokenHolder[count] << '\n';
				count++;

				/*tokenHolder[count] = before;
				cout << count << ": " << tokenHolder[count] << '\n';
				count++;*/
				//temp = after;
				temp = sendItThroughAgain(after, false);
				//will be set right after this
			}
			else if (temp != "" && temp != str2){
				std::string before = temp.substr(0, foundLT);
				std::string after = temp.substr(foundLT + 1, temp.size());

				sendItThroughAgain(before, true);
				//tokenHolder[count] = before;
				//cout << count << ": " << tokenHolder[count] << '\n';
				//count++;

				tokenHolder[count] = lessThan;
				//cout << count << ": " << tokenHolder[count] << '\n';
				count++;

				//temp = after;
				temp = sendItThroughAgain(after, false);
			}


		}

		//////////////////////////////////////////////////////////////////////////////

		std::string equal("=");
		std::size_t foundEqual = temp.find(equal);

		if (foundEqual != std::string::npos && temp != equal && !commentIsOn && temp != ":=" && temp != "<=" && temp != ">="){

			std::string before = temp.substr(0, foundEqual);
			//add already exists
			std::string after = temp.substr(foundEqual + 1, temp.size());

			sendItThroughAgain(before, true);
			//tokenHolder[count] = before;
			//cout << count << ": " << tokenHolder[count] << '\n';
			//count++;

			tokenHolder[count] = equal;
			//cout << count << ": " << tokenHolder[count] << '\n';
			count++;

			//temp = after;
			temp = sendItThroughAgain(after, false);
		}
		////////////////////////////////////////////////////////////////////////////////

		std::string add("+");
		std::size_t foundAdd = temp.find(add);
		if (foundAdd != std::string::npos && temp != add && !commentIsOn && temp != ""){
			std::string before = temp.substr(0, foundAdd);
			//add already exists
			std::string after = temp.substr(foundAdd + 1, temp.size());

			sendItThroughAgain(before, true);
			//tokenHolder[count] = before;
			//cout << count << ": " << tokenHolder[count] << '\n';
			//count++;

			tokenHolder[count] = add;
			//cout << count << ": " << tokenHolder[count] << '\n';
			count++;

			//temp = after;
			temp = sendItThroughAgain(after, false);
		}

		///////////////////////////////////////////////////////////////////////////////////

		std::string mult("*");
		std::size_t foundMult = temp.find(mult);
		if (foundMult != std::string::npos && temp != mult && !commentIsOn && temp != ""){
			std::string before = temp.substr(0, foundMult);
			//add already exists
			std::string after = temp.substr(foundMult + 1, temp.size());

			sendItThroughAgain(before, true);;
			//tokenHolder[count] = before;
			//cout << count << ": " << tokenHolder[count] << '\n';
			//count++;

			tokenHolder[count] = mult;
			//cout << count << ": " << tokenHolder[count] << '\n';
			count++;

			//temp = after;
			temp = sendItThroughAgain(before, false);
		}

		///////////////////////////////////////////////////////////////////////////////////
		std::string subtract("-");
		std::size_t foundSubtract = temp.find(subtract);
		if (foundSubtract != std::string::npos && temp != subtract && !commentIsOn && temp != ""){
			std::string before = temp.substr(0, foundSubtract);
			//add already exists
			std::string after = temp.substr(foundSubtract + 1, temp.size());

			sendItThroughAgain(before, true);
			//tokenHolder[count] = before;
			//cout << count << ": " << tokenHolder[count] << '\n';
			//count++;

			tokenHolder[count] = subtract;
			//cout << count << ": " << tokenHolder[count] << '\n';
			count++;

			//temp = after;
			temp = sendItThroughAgain(after, false);
		}

		////////////////////////////////////////////////////////////////////////////

		std::string division("/");
		std::size_t foundDiv = temp.find(division);
		if (foundDiv != std::string::npos && temp != division && !commentIsOn && temp != ""){
			std::string before = temp.substr(0, foundDiv);
			//add already exists
			std::string after = temp.substr(foundDiv + 1, temp.size());

			//tokenHolder[count] = before;
			//cout << count << ": " << tokenHolder[count] << '\n';
			//count++;
			sendItThroughAgain(before, true);

			tokenHolder[count] = division;
			//cout << count << ": " << tokenHolder[count] << '\n';
			count++;

			//temp = after;
			temp = sendItThroughAgain(after, false);
		}






		//////////////////////////////////////////////////////////////////////
		std::string comma = ",";
		std::size_t foundComma = temp.find(comma);

		if (foundComma != std::string::npos && !commentIsOn && temp != comma){
			std::string beforeComma = temp.substr(0, foundComma);
			std::string afterComma = temp.substr(foundComma + 1);

			//tokenHolder[count] = beforeComma;
			//tokenHolder[count] = sendItThroughAgain(beforeComma); //go to bottom and fix it, no need for equate i think
			sendItThroughAgain(beforeComma, true);

			//cout << count << ": " << tokenHolder[count] << '\n';
			//count++;

			tokenHolder[count] = comma;
			//cout << count << ": " << tokenHolder[count] << '\n';
			count++;

			//temp = afterComma;
			temp = sendItThroughAgain(afterComma, false);
			//FIXME BELOW

		}




		////////////////////////////////////////////////////////////////




		//end of Right Parenthesis
		std::size_t foundRightPar = temp.find(rightPar);

		if (foundRightPar != std::string::npos && temp != rightPar && !commentIsOn && temp != ""){

			if (foundRightPar != 0){		//

				std::string beforeRightPar = temp.substr(0, foundRightPar);
				//right parenthis is already a string above
				std::string afterRightPar = temp.substr(foundRightPar + 1, temp.size());

				sendItThroughAgain(beforeRightPar, true);
				//tokenHolder[count] = beforeRightPar;
				//cout << count << ": " << tokenHolder[count] << '\n';
				//count++;

				tokenHolder[count] = rightPar;
				//cout << count << ": " << tokenHolder[count] << '\n';
				count++;

				//temp = afterRightPar;
				temp = sendItThroughAgain(afterRightPar, false);
			}
			else{		//if there isn't anything before it, it doesn't need a before
				std::string afterRightPar = temp.substr(foundRightPar + 1, temp.size());

				tokenHolder[count] = rightPar;
				//cout << count << ": " << tokenHolder[count] << '\n';
				count++;

				//testing this shiz
				//temp = afterRightPar;
				temp = sendItThroughAgain(afterRightPar, false);

			}
		}


		///////////////////////////////////////////////////////////////

		//If there is a ; at the end, remove it and make it it's own token
		std::string semi = ";";
		std::size_t foundSemi = temp.find(semi);

		if (foundSemi != std::string::npos && !commentIsOn && temp != semi){	//so above doesn't accidently add a blank
			std::string beforeSemi = temp.substr(0, foundSemi);
			std::string afterSemi = temp.substr(foundSemi + 1, temp.size());
			sendItThroughAgain(beforeSemi, true);
			//tokenHolder[count] = beforeSemi;
			//cout << count << ": " << tokenHolder[count] << '\n';
			//count++;
			tokenHolder[count] = semi;
			//cout << count << ": " << tokenHolder[count] << '\n';
			count++;

			//temp = semi;		//makes the string a ";"
			temp = sendItThroughAgain(afterSemi, false);

		}
		//////////////////////////////////////////////////////////////////
		//There is '.' and '..'

		std::string period(".");
		std::size_t foundPeriod = temp.find(period);

		if (foundPeriod != std::string::npos && temp != period && !commentIsOn){

			std::string str2("..");
			std::size_t found = temp.find(str2);
			std::size_t end = found + 1;		//reasons

			if (found != std::string::npos && temp != str2 && !commentIsOn){		//check if it := is by itself

				//std::cout << "found ':=' -> " << found << '\n';
				std::string before = temp.substr(0, found);
				std::string doubPeriod = temp.substr(found, end);
				std::string after = temp.substr(end + 1, temp.size());

				tokenHolder[count] = before;
				//cout << count << ": " << tokenHolder[count] << '\n';
				count++;

				tokenHolder[count] = doubPeriod;
				//cout << count << ": " << tokenHolder[count] << '\n';
				count++;

				/*tokenHolder[count] = before;
				cout << count << ": " << tokenHolder[count] << '\n';
				count++;*/
				temp = after;
				//will be set right after this

			}
			else if (temp != "" && temp != str2){			//needs to check for if := is by itself so it doesn't try and think it's just ':'
				std::string before = temp.substr(0, foundPeriod);

				tokenHolder[count] = before;
				//cout << count << ": " << tokenHolder[count] << '\n';
				count++;

				temp = period;
			}

		}



		//records the latest token, most general case
		if (!commentIsOn && temp != "" && onLeftSide){			//had to add "" clause cause for some reason a space was getting through
			tokenHolder[count] = temp;

			//printf("%s\n", pch);
			//cout << count << ": " << tokenHolder[count] << '\n';

			count++;
		}
		/*
		//move this to the end so it doesn't grab the end }
		if (findEnd != std::string::npos){
			commentIsOn = false;
		}
		pch = strtok(NULL, " ");
		tokenHolder[count] = pch;
		temp = tokenHolder[count];		//assigns whatever pch is to string temp*/

		return temp;
	}

};