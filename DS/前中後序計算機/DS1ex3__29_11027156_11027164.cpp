#include <string> // string class
#include <cstdlib> // system, atoi
#include <vector>
#include <iostream>
#define MAX_LEN 255 // maximum string length
using namespace std;

struct Node{
	string str;
	Node *next;
};

typedef Node * NPtr;

int priority(char op) {   // 加減乘除順序要對! 
    if (op == '+' || op == '-')
      return 1;
    else if (op == '*' || op == '/')
      return 2;
    else   
      return 0;
}
string reverse(string s){
	string newstr;
	int i=0,j=0;
	while(s[i]!='\0'){
		i++;
	}
	for(j=i-1;j>=0;j--){
		newstr = newstr + s[j];
	}
	
	return newstr;
}
void inToprefix(string infix, vector<string> &prefix) {  // 堆疊 
    vector<char> stack;
    char ch;
    string str;
    int i;
    for(int i = infix.size()-1; i >= 0; i--) {

        str.clear();

        if (infix[i] == ')')
        	stack.push_back(infix[i]); // 直接放進去 

        else if (infix[i] == '+'||infix[i] == '-'||infix[i] == '*'||infix[i] == '/') {
        	if ( !stack.empty() ) {  // 要確定不是空的不然會出包 

            while(priority(stack.back()) > priority(infix[i])) {
                str = stack.back();
                stack.pop_back();
                prefix.push_back(str);
            }

            }
            stack.push_back(infix[i]);  // 存入堆疊
            str.clear();
        }

        else if (infix[i] == '('){
        	while( stack.back() != ')') { // 輸出至 )
                str = stack.back();
                stack.pop_back();
                prefix.push_back(str);
            }
            stack.pop_back();  // 不輸出 )
            str.clear();

		}
        else if (infix[i]>='0'&& infix[i] <='9') { // 直接放
            str = infix[i];

			while ( infix[i-1]>='0'&& infix[i-1] <='9') {  // 下一位還是數字
                i--;
                str = str + infix[i];
			}
            str = reverse(str);
            prefix.push_back(str);
            str.clear();
        } 

    }
    while(stack.back() != '\0' && stack.back() != ')') {
            str = stack.back();
            stack.pop_back();
            if ( str == "+" || str == "/"|| str == "-" || str == "*") // 把剩下的放進去 
              prefix.push_back(str);
    }
}

NPtr challenge(string s){
    string infix = s;
    vector<string> prefix;
    NPtr ans = new Node;
    NPtr temp;
    NPtr head;
    NPtr temp2;
    temp = ans;
    head = ans;
    inToprefix(infix, prefix);

    for(int j = prefix.size()-1; j >= 0; j--) {
      if ( prefix[j] != " " && prefix[j] != "") {
        temp -> str =  prefix[j];
        temp -> next = new Node;
        temp2 = temp;
        temp = temp -> next;
      }        
    }
    
    
    temp2 -> next = NULL;  // 把多的用掉 
    cout << "Prefix expression: ";
    head = ans;
    while( ans != NULL ){
    	if (ans -> next == NULL ) // 沒下一個了 
    	  cout <<ans -> str ;
    	else 
    	  cout <<ans -> str <<",";
    	ans = ans -> next;
	}
	cout << endl;
    
    return head;

}

void inToPostfix(string infix, vector<string> &postfix) {  // 堆疊 
    vector<char> stack;
    char ch;
    string str;
    int i;
    bool contd = false;
    for(i = 0; i < infix.length(); i++) {

        str.clear();

        if (infix[i] == '(')
        	stack.push_back(infix[i]); // 直接放進去 

        else if (infix[i] == '+'||infix[i] == '-'||infix[i] == '*'||infix[i] == '/') {
        	if ( !stack.empty() ) {  // 要確定不是空的不然會出包 

            while(priority(stack.back()) >= priority(infix[i])) {
                str = stack.back();
                stack.pop_back();
                postfix.push_back(str);
            }

            }
            stack.push_back(infix[i]);  // 存入堆疊
            str.clear();
        }

        else if (infix[i] == ')'){
        	while( stack.back() != '(') { // 輸出至 (
                str = stack.back();
                stack.pop_back();
                postfix.push_back(str);
            }
            stack.pop_back();  // 不輸出 (
            str.clear();

		}
        else if (infix[i]>='0'&& infix[i] <='9') { // 直接放
            str = infix[i];

			while ( infix[i+1]>='0'&& infix[i+1] <='9') {  // 下一位還是數字
                i++;
                str = str + infix[i];
			}

            postfix.push_back(str);
            str.clear();
        } 

    }
    while(stack.back() != '\0' && stack.back() != '(') {
            str = stack.back();
            stack.pop_back();
            if ( str == "+" || str == "/"|| str == "-" || str == "*") // 把剩下的放進去 
              postfix.push_back(str);
    }
}


void bracket(string input,int &count){  // 左括號加，右括號減 
	for(int i = 0; i < input.length(); i++){
		if ( input[i] == '(')
		  count ++;
		else if ( input[i] == ')')
		  count --;
		if ( count <0 )
		  return;
	}

}

bool legalchar(string s){
	for(int i = 0; i < s.length(); i++){
		if ( s[i] == ' ' )
		  continue;
		else if ( !(( s[i]>='0' &&s[i]<='9' ) || s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '(' || s[i] == ')'))
		  return false;
	}

	return true;
}

int notation(string s){

	for(int i = 0; i < s.length(); i++){
	  if ( s[i] == ' ' )
		  continue;
	  else if ( i == 0 ){  // 第一個的例外
	    if( s[i] == '(' && (s[i+1] == '+' || s[i+1] == '-' || s[i+1] == '*' || s[i+1] == '/'|| s[i+1] == ')'))
		  return 3;
		else if ( s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/')
		  return 3;
	  }
	  else {
	  	  if( s[i] == '(' && (s[i+1] == '+' || s[i+1] == '-' || s[i+1] == '*' || s[i+1] == '/'|| s[i+1] == ')'))
		    return 3;
		  else if( s[i] == '(' && (( s[i-1]>='0' &&s[i-1]<='9' )))
		    return 2;
		  else if( s[i] == ')' && (s[i-1] == '+' || s[i-1] == '-' || s[i-1] == '*' || s[i-1] == '/'))
		    return 3;
		  else if( s[i] == ')' && ( s[i+1]>='0' &&s[i+1]<='9' ))
		    return 2;
		  else if ( (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/')&& (s[i+1] == '+' || s[i+1] == '-' || s[i+1] == '*' || s[i+1] == '/'))
		    return 1;
		  else if ( (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/')&& (s[i+1] == '\0')  )
		    return 3;

	  }


	}

	return 0;

}


string split(string s )
{
	int current = 0; //最初由 0 的位置開始找
	int next;
	vector<string> buf;
	string newstr;
	while (1)
	{
		next = s.find_first_of(" ", current);
		if (next != current)
		{
			string tmp = s.substr(current, next - current);
			if (tmp.size() != 0) //忽略空字串
				buf.push_back(tmp);
		}
		if (next == string::npos) break;
		current = next + 1; //下次由 next + 1 的位置開始找起。
	}

	for (int i = 0 ; i <buf.size(); i++)
	  newstr = newstr+buf[i];

	return newstr;
}



bool allnumcheck(string s){ // 看看是不是單純數字 
	bool ox = true;
	for(int i = 0; i < s.length(); i++){
		if(!(s[i]>='0' &&s[i]<='9'))
	      ox = false;	
    }
    return ox;
}

int case1(string input)
{

	int count = 0 ;
	int errornum = 0;
	input = split(input);
	bracket(input, count);


	if (input == ""){
		cout << "Error! Enter new thing!"<<endl;
	    return 0;
	}
	if (!legalchar(input)){
		cout << "Error1! Illegal char!"<<endl;
	    return 0;
	}
	if ( count < 0) {
		cout << "Error2! Extra close parenthesis!"<<endl;
		return 0;
    }
	if ( count > 0 ) {
	    cout << "Error2! Extra open parenthesis!"<<endl;
	    return 0;
    }
    
    errornum = notation(input);
	if ( errornum == 1 ){
		cout << "Error3! Extra operator!"<<endl;
	    return 0;
	}
	else if (errornum == 2){
		cout << "Error3! Extra operand!"<<endl;
	    return 0;	
	}
	else if (errornum == 3){
		cout << "Error3! It is not infix."<<endl;
	    return 0;
		
	} 
	if(allnumcheck(input)){
		cout << "It is a legitimate infix expression." << endl;
		cout << "Postfix expression: "<< input << endl;
		cout <<  "Answer: "<< input << endl;
		return 0;
	}

    return 1;


}

NPtr case2( string s ){
    string infix = s;
    vector<string> postfix;
    NPtr ans = new Node;
    NPtr temp;
    NPtr head;
    NPtr temp2;
    temp = ans;
    head = ans;
    inToPostfix(infix, postfix);
    /*
    for(int i = 0; i < postfix.size() ; i++) {
        cout <<  postfix[i] << " ";
    }
    */
    for(int j = 0; j < postfix.size() ; j++) {
      if ( postfix[j] != " " && postfix[j] != "") {
        temp -> str =  postfix[j];
        temp -> next = new Node;
        temp2 = temp;
        temp = temp -> next;
      }        
    }
    
    
    temp2 -> next = NULL;  // 把多的用掉 
    // temp = NULL; 
    cout << "Postfix expression: ";
    
    while( ans != NULL ){
    	if (ans -> next == NULL ) // 沒下一個了 
    	  cout <<ans -> str ;
    	else 
    	  cout <<ans -> str <<",";
    	ans = ans -> next;
	}
	cout << endl;
    
    return head;
}




int postfixEvaluation(vector < string > & postfix, bool &divisor) {
  vector < int > stk;
  for (string token: postfix) {
    if (token == "+" || token == "-" || token == "*" || token == "/") {
      int first = stk.at(stk.size()-1);
      stk.pop_back();
      int second = stk.at(stk.size()-1);
      stk.pop_back();
      if (token == "+")
        stk.push_back(first + second);
      else if (token == "-")
        stk.push_back(second - first);
      else if (token == "*")
        stk.push_back(first * second);
      else{
        if(first==0) {
            cout << "divisor shouldn't be 0!";
            divisor=false;
            return 0;
        }
        stk.push_back(second / first);
      }

    }

    else
      stk.push_back(stoi(token));
  }
  return stk.at(stk.size()-1);
}




int main(void)
{ int command = 0; // user command
 do
 { string infixS;
   NPtr  postfixforptr;
   NPtr  prefixforptr;
   NPtr  trash;
   vector<string> postfix;
   vector<string> prefix;
   int correct;
   int value;
   bool divisor;
 cout << endl << "* Arithmetic Expression Evaluator *";
 cout << endl << "* 0. QUIT *";
 cout << endl << "* 1. Infix2postfix Evaluation *";
 cout << endl << "***********************************";
 cout << endl << "Input a choice(0, 1): ";
 cin >> command; // get a command


 cin.ignore(MAX_LEN, '\n'); // clear the input buffer
 switch (command)
 { case 0: break;


 case 1: cout << endl << "Input an infix expression: ";
	getline(cin, infixS);
	correct=case1(infixS);
	if(correct==0) break;

 	postfixforptr = case2(infixS);
    prefixforptr = challenge(infixS);

 	while( postfixforptr != NULL ){
    	postfix.push_back(postfixforptr -> str);
    	trash = postfixforptr; // 記起來 
    	postfixforptr = postfixforptr -> next; // 下面一位 
    	delete trash; // 可以丟了 
    	trash = NULL; // 確保指到空 
    } // while

	while( prefixforptr != NULL ){
    	prefix.push_back(prefixforptr -> str);
    	trash = prefixforptr; // 記起來 
    	prefixforptr = prefixforptr -> next; // 下面一位 
    	delete trash; // 可以丟了 
    	trash = NULL; // 確保指到空 
    	
	}



//case3
	divisor=true;
	value = postfixEvaluation(postfix,divisor);
	if(divisor==true) cout <<  "Answer: " << value << endl;

    

// Mission One: check whether it is an infix expression
// isExp(), isNumber(), isOp(), isPar(), isBalanced(), isInfix()
// Mission Two: transform infix to postfix, stored as a linked list
// infix2postfix()
// Mission Three: evaluate the value of a postfix expression
// postfixEval()
 	break;
 default: cout << endl << "Command does not exist!" << endl;
    break;
 } // end switch


 } while (command != 0); // '0': stop the program


 system("pause"); // pause the display
 return 0;
} // end main
