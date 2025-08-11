#include <bits/stdc++.h>
using namespace std;

int gline = 1;
int gcol = 0;
bool glineProcessed = false; // �W�@�ӪF��B�z���h��true

struct Token{
	string name;
	string type; 
};

// ��k��ϥ�
//     *
//    / \
//   *   * 
struct Node {
    bool isAtom;
    Token atom;
    Node* left;
    Node* right;

    Node(bool atomFlag, Token t = {}, Node* l = nullptr, Node* r = nullptr)
        : isAtom(atomFlag), atom(t), left(l), right(r) {}
};

typedef vector <Token> TokenList ;

Node* LookupPointer(Node* raw);

typedef Node* NodePtr ;

map<string, Node*> env; // key: symbol, value: pointer to Node
// �M����token 
class Tokenizer{		
public:
  void dealwitherror(){
      while ( cin.peek() != '\n' && cin.peek() != EOF ) {
        cin.get() ;
        
        cin.peek() ;
        
      } // while()
	
  } // dealwitherror()
  bool is_whitespace(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
  }

  bool is_separator(char ch) {
    return is_whitespace(ch) || ch == '(' || ch == ')' || ch == '\'' || ch == '"' || ch == ';';
  }
  
  bool IsStringLiteral(istream &input_stream, string &token) {
    token.clear();
    char ch;
    while (input_stream.get(ch)) {
        gcol++;
        if (ch == '"') {
        	token = "\""+token+ "\""; //�����޸����L 
            return true; // �����r��
        }
        if (ch == '\n') {
            return false; // �����~
        }
        if (ch == '\\') {
            if (input_stream.peek() == 'n' || input_stream.peek() == 't' ||
                input_stream.peek() == '"' || input_stream.peek() == '\\') {
                input_stream.get(ch);
                gcol++;
                if (ch == 'n') token += '\n';
                else if (ch == 't') token += '\t';
                else if (ch == '"') token += '\"';
                else token += '\\';
            } 
			else {
                token += '\\';
            }
        } 
		else {
            token += ch;
        }
    }
    return false; // EOF before closing quote
  }
  	
  // ��¥Hseparators��token������ 
  Token gettoken(istream &input_stream) {
    Token Token;
    string t;
    char ch;

    while (input_stream.get(ch)) {
        gcol++;
        if (ch == '\n') {
            if ( glineProcessed ){
            	glineProcessed = false;
            	gline = 1;
            	gcol = 0;
			}
			else{
				gline++;
				gcol = 0;
			}
            
        }
        // ���L�ť�
        if (is_whitespace(ch)) continue;
        // �B�z����
        if (ch == ';') {
            while (input_stream.peek() != '\n' && input_stream.peek() != EOF) {
                input_stream.get();
                gcol++;
            }
            ch = input_stream.get();//�⴫��Ū�i��
            if (ch == '\n') {
              gline++;
              gcol = 0;
            }
            Token.name = "";
            Token.type = "Comment";
            return Token;
        }
        // �B�z�r��
        if (ch == '"') {
            if (IsStringLiteral(input_stream, t)) {
                Token.name = t;
                Token.type = "String";
            } else {
                Token.name = "NCQ";
                Token.type = "";
                // cout << "ERROR (no closing quote) : END-OF-LINE encountered at Line " << gline << " Column " << gcol << endl;
            }
            return Token;
        }
        // �B�z() 
		if (ch == '(') {
            char next = input_stream.peek();
            while( is_whitespace(next) ) {            	
            	next = input_stream.get();
            	gcol++;
            	next = input_stream.peek();
            	if (next == '\n') {
              		gline++;
              		gcol = 0;
            }
			}
            if (next == ')') {
                input_stream.get();
                gcol++;
                Token.name = "()";
                Token.type = "NIL";
                return Token;
            }
		} 

        if (is_separator(ch)) {
            t += ch;
            Token.name = t;
            Token.type = TypeDecide(t);
            return Token;
        }
        // �@�� token�AŪ��J����j�Ÿ�����
        t += ch;
        while (input_stream.get(ch) && !is_separator(ch) && !is_whitespace(ch)) {
            gcol++;
            t += ch;
        }
        if (!is_separator(ch)) {
            // �Y�O�ťաA�^�h�@�Ӧr��
            input_stream.unget();
        } else {
            // �Y�O���j�šA�d�U�ӵ��U�@�� token ��
            input_stream.unget();
        }

        Token.name = t;
        Token.type = TypeDecide(t);
        return Token;
    }

    // EOF
    Token.name = "";
    Token.type = "EOF";
    // cout << "ERROR (no more input) : END-OF-FILE encountered\n";
    return Token;
  }

bool IsInt(const string& str) {
    return regex_match(str, regex("[-+]?[0-9]+"));
}
  
bool IsFloat(const string& str) {
    return regex_match(str, regex("[-+]?(?:[0-9]*\\.[0-9]+|[0-9]+\\.|[0-9]+\\.[0-9]+)"));
}
  
  bool IsT( string str ) {
    if ( str.size() == 1 && str == "t" ) {
      return true ;
    } // if ()
    else if ( str.size() == 2 && str == "#t" ) {
      return true ;
    } // else if ()
    return false ;
  } // IsT()
  
  bool IsNIL( string str ) {
    if ( str.size() == 2 && str == "#f" ) {
      return true ;
    } // if () 
    else if ( str.size() == 3 && str == "nil" ) {
      return true ;
    } // else if ()
    return false ; 
  } // IsNIL()
  
  // ����token 
  string TypeDecide( string str ) {
    string type = "NULL" ;
    if ( str == "(" ) {
      type = "LEFT-PAREN" ;
    } // if()
    else if ( str == ")" ) {
      type = "RIGHT-PAREN" ;
    } // else if()
    else if ( str == "." ) {
      type = "DOT" ;
    } // else if()
    else if ( IsInt( str ) ) {
      type = "INT" ;
    } // else if()
    else if ( IsFloat( str ) ) {
      type = "FLOAT" ;
    } // else if()
    else if ( str == "'" ) {
      type = "QUOTE" ;
    } // else if()
    else if ( IsT( str ) ) {
      type = "T" ;
    } // else if()
    else if ( IsNIL( str ) ) {
      type = "NIL" ;
    } // else if() 
    else {
      type = "SYMBOL" ;
    } // else   
    return type ;
  } // TypeDecide()
 
 
  void ShowAlltoken(const std::vector<Token>& tokens) {
    for (int i = 0 ; i < tokens.size(); i++) {
      cout << "NO." << i << " token is " << tokens[i].name <<" type is " <<  tokens[i].type << endl;
    }
  } 
  
  bool Isexit(const std::vector<Token>& tokens) {
  	//ShowAlltoken(tokens);
    for (size_t i = 0; i < tokens.size(); ++i) {
    	// (exit)
        if (tokens[i].name == "exit") {
            if (i > 0 && i + 1 < tokens.size() &&
                tokens[i - 1].name == "(" && tokens[i + 1].name == ")"&&tokens.size() == 3) {
                return true;
            }
        }
    }
    
    // (exit.()) or (exit.nil) or (exit.#f)
    
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i].name == "exit") {
            if (i > 0 && i + 3 < tokens.size() &&
                tokens[i - 1].name == "(" && tokens[i + 1].name == "."&& tokens[i + 2].type == "NIL"&& tokens[i + 3].name == ")"&&tokens.size() == 5) {
                return true;
            }
        }
    }
    return false;
  }
  
  bool Isclean_environment(const std::vector<Token>& tokens) {
  	//ShowAlltoken(tokens);
    for (size_t i = 0; i < tokens.size(); ++i) {
    	// (clean-environment)
        if (tokens[i].name == "clean-environment") {
            if (i > 0 && i + 1 < tokens.size() &&
                tokens[i - 1].name == "(" && tokens[i + 1].name == ")"&&tokens.size() == 3) {
                return true;
            }
        }
    }
    
    // (clean-environment . () ) or (clean-environment . nil ) or ( clean-environment . #f )
    
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i].name == "clean-environment") {
            if (i > 0 && i + 3 < tokens.size() &&
                tokens[i - 1].name == "(" && tokens[i + 1].name == "."&& tokens[i + 2].type == "NIL"&& tokens[i + 3].name == ")"&&tokens.size() == 5) {
                return true;
            }
        }
    }
    return false;
  }


  void RemoveComments(vector<Token>& tokens) {
    for (auto it = tokens.begin(); it != tokens.end(); ) {
        if (it->type == "Comment") {
            it = tokens.erase(it);
        } 
		else {
            ++it; 
        }
    }
  }
  

  void CleanAlltoken(vector<Token>& tokens) {
    tokens.clear();
    ShowAlltoken(tokens);
  }
	
};

// �M���P�_��k�Apj2�H�᪺�ؾ� 
class Parser {
public:
  void ShowAlltoken(vector<Token>& tokens) {
    for (int i = 0 ; i < tokens.size(); i++) {
      cout << "NO." << i << " token is " << tokens[i].name <<" type is " <<  tokens[i].type << endl;
    }
  }
  
  // �����᭱�����L 
  void dealwitherror(){
  	  // �t�Xgettoken�̭�������P�_�A��\n�d���L�����s���}�l 
      while ( cin.peek() != '\n' && cin.peek() != EOF ) {
        cin.get() ;      
        cin.peek() ;
        
      } // while()
		
  } // dealwitherror()
  
  // �u��atom�A���ΧP�_��k�����L�X��	
  bool OnlyAtom(vector<Token>& tokens) {
    for (int i = 0 ; i < tokens.size(); i++) {
      if( tokens[i].name == "(" || tokens[i].name == ")"|| tokens[i].name == "\'")
        return false;
    }
    return true;
  }
  bool IsAtom(Token t){
  	if( t.name == "(" || t.name == ")"|| t.name == "\'"){
	  
        return false;
    }
    return true;
  }
  
  void printOnlyAtom(vector<Token>& tokens) {
  	//cout << "296~\n";
    //cout << tokens[0].type << " is coming!!";
      if ( tokens[0].type == "INT"){
        int ioutput = atoi( tokens[0].name.c_str() ) ;
        cout << ioutput;
	  }
	  else if ( tokens[0].type == "FLOAT"){
        float foutput = atof( tokens[0].name.c_str() ) ;
        cout << fixed << setprecision(3) << foutput;
	  } 
	  else if ( tokens[0].type == "NIL" ) {
        cout << "nil" ;
      } // else if()
    
      else if ( tokens[0].type == "T" ) {
        cout << "#t" ;
      } // else if()
    
      else if ( tokens[0].type == "QUOTE" ) {
        cout << "quote" ;
      } // else if()
      else if ( tokens[0].type == "String" ) {
        cout <<  tokens[0].name ;
      } // else if()
      else if ( tokens[0].type == "PRIMITIVE") {
       		cout << "#<procedure " << tokens[0].name << ">";
      }
      else if ( tokens[0].type == "DOT" ) { //��W" . "�N�����L�� 
        cout << "ERROR (unexpected token) : atom or '(' expected when token at Line "<< gline <<" Column " <<gcol <<" is >>"<<tokens[0].name<< "<<" ;
        dealwitherror();
		tokens.clear();  
      } // else if()
      else {  // symbol
        cout << tokens[0].name ;     
      } // else	  
    }
  
  void PrintSpace( int count ) {
    for ( int i = 0 ; i < count ; i++ ) {
      cout << " " ;     
    } // for()
  } // PrintSpace()
  
	void pp(Token t){
		if ( t.type == "INT"){
	      int ioutput = atoi( t.name.c_str() ) ;
	      cout << ioutput;
		}
		else if ( t.type == "FLOAT"){
	      float foutput = atof( t.name.c_str() ) ;
	      cout << fixed << setprecision(3) << foutput;
		} 
		else if ( t.type == "NIL" ) {
	      cout << "nil" ;
	    } 
	    else if ( t.type == "T" ) {
	      cout << "#t" ;
	    } 
	    else if ( t.type == "QUOTE" ) {
	      cout << "quote" ;
	    } 
	    else if (t.type == "PRIMITIVE") {
       		cout << "#<procedure " << t.name << ">";
    	}
	    else if ( t.type == "String" ) {
	      cout <<  t.name ;
	    } 
	    else if ( t.type == "DOT" ) {
	      cout << t.name ;  
	    } 
	    else {  // symbol
	      cout << t.name ;     
	    } 	
	}

	vector<Token> processTokens(vector<Token> tokens) {
	    vector<Token> processed;
	    // ��nil ����() 
	    for (const auto& token : tokens) {
	        if (token.type == "NIL") {
	            processed.emplace_back(Token{"(", "LEFT-PAREN"});
	            processed.emplace_back(Token{")", "RIGHT-PAREN"});
	        } else {
	            processed.push_back(token);
	        }
	    }
	    // �� .( �R�� ) 
	    for (size_t i = 0; i < processed.size(); ++i) {
	        if (processed[i].type == "DOT" && i + 1 < processed.size() && processed[i + 1].type == "LEFT-PAREN") {
	            size_t left = i + 1;
	            int parenCount = 1;
	            size_t j = left + 1;
	
	            while (j < processed.size() && parenCount > 0) {
	                if (processed[j].type == "LEFT-PAREN") {
	                    parenCount++;
	                } else if (processed[j].type == "RIGHT-PAREN") {
	                    parenCount--;
	                }
	                ++j;
	            }
	
	            if (parenCount != 0) {
	                // �S������٪� )�A�����Ӥ��|�A�ڥ��P�_����k�S���~�i�Ӫ� 
	                cout << "error: no matching RIGHT-PAREN" << endl;
	                return {};
	            }
	
	            processed.erase(processed.begin() + (j - 1));
	            processed.erase(processed.begin() + i, processed.begin() + i + 2);
	
	            i--;
	        }
	    }
	    return processed;
	}


	// �u�n�J��quote�A�N���s�I�s 
	vector<Token> processQuote(const vector<Token>& tokens, int& idx) {
	    if (idx >= tokens.size()) return {};
	
	    // ��� quote �N�ܦ� ( quote (...) )
	    if (tokens[idx].name == "'") {
	        idx++; // �q'���U�@�Ӷ}�l�B�z 
	        vector<Token> quoted = processQuote(tokens, idx);
	        vector<Token> result;
	        result.push_back(Token{"(", "LEFT-PAREN"});
	        result.push_back(Token{"quote", "QUOTE"});
	        result.insert(result.end(), quoted.begin(), quoted.end());
	        result.push_back(Token{")", "RIGHT-PAREN"});
	
	        return result;
	    }
	
	    // �J�� ()�A�B�z()�̪��F�� 
	    if (tokens[idx].name == "(") {
	        vector<Token> result;
	        result.push_back(Token{"(", "LEFT-PAREN"});
	        idx++;
	        while (idx < tokens.size() && tokens[idx].name != ")") {
	            vector<Token> inner = processQuote(tokens, idx);
	            result.insert(result.end(), inner.begin(), inner.end());
	        }
	        if (idx < tokens.size() && tokens[idx].name == ")") {
	            result.push_back(Token{")", "RIGHT-PAREN"});
	            idx++; 
	        }
	        return result; // ��짹�㪺()�^�� 
	    }
	
	    // �J��atom�����^�� 
	    return { tokens[idx++] };
	}
	
	// �λ��j�B�zquote 
	vector<Token> recurciveQuote(const vector<Token>& tokens) {
	    vector<Token> result;
	    int idx = 0;
	    while (idx < tokens.size()) {
	        vector<Token> expr = processQuote(tokens, idx);
	        result.insert(result.end(), expr.begin(), expr.end());
	    }
	    return result;
	}


	void printTokens(const vector<Token>& tokens) {
		int num_space = 0;
		bool paren = false;
		// �����٪�.(�P)�R�� 
		vector<Token> result = processTokens(tokens); 
		for (size_t i = 0; i < result.size(); ++i) {
		    const auto& token = result[i];
		    // �ˬd() 
		    if (token.name == "(" && i + 1 < result.size() && result[i + 1].name == ")") {
		    	if (paren) { // �W�@���L���O "("
		            cout << " ";
	                cout << "nil"<< endl;
		            paren = false;
		            ++i; // ���L�U�@�� ")"
		            continue;
		        }
		        else{
		    	  PrintSpace(num_space);
		          cout << "nil" << endl;
		          ++i; // ���L�U�@�� ")"
		          continue;
		        }
		    }
		
		    if (token.name == "(") {
		    	if (paren) { // �W�@���L���O "("
		            cout << " ";
	                cout << token.name;
	                num_space += 2;
		            paren = true;
		        }
		        else{
		          PrintSpace(num_space);
		          cout << token.name;
		          num_space += 2;
		          paren = true;
		        }  
		    } 
			else if (token.name == ")") {
				num_space -= 2;
				PrintSpace(num_space);
		        cout << token.name << endl;
		    } 
			else {
		        if (paren) { // �W�@���L���O "("
		            cout << " ";
		            pp(token);
		            cout << endl;
		            paren = false;
		        } 
				else {
		            PrintSpace(num_space);
		            pp(token);
		            cout << endl;
		            paren = false;
		        }
		    }
		}
	}
// �λ��j�T�w�O�_�@�� sexp �̭��O�_�u���@���I 
    int parenparser_check(vector<Token>& tokens,int index,bool &error_msg){
    	bool dot = false;
    	bool newsexp = false;
    	for (int i = index; i < tokens.size()&&!error_msg; i++){
    	  if( tokens[i].name == "." ){ // (. .. .) ���� 
    	    if ( !dot ) {
              dot = true ;            
            } // if()
    	    if(i > 0 && (tokens[i - 1].name == "." || tokens[i - 1].name == "(" ) ){ // .. or (. 
            	cout << "\n> ";
                cout << "ERROR (unexpected token) : atom or '(' expected when token at Line "<< gline <<" Column " <<gcol <<" is >>"<<tokens[i].name<< "<<\n" ;
                dealwitherror();
                //cout << "error1 : .. or (.\n"; 
                glineProcessed = true;
	      	    tokens.clear();
	      	    error_msg = true;
                return 0;    	    	
			}
			if ( i+1 < tokens.size() && tokens[i + 1].name == ")"){ // .)
				cout << "\n> ";
                cout << "ERROR (unexpected token) : atom or '(' expected when token at Line "<< gline <<" Column " <<gcol <<" is >>"<<tokens[i+1].name<< "<<\n" ;
                dealwitherror();
                //cout << "error2 : .)\n"; 
                glineProcessed = true;
	      	    tokens.clear();
	      	    error_msg = true;
                return 0;
			}
	    }
		else if ( tokens[i].name == ")" ) {
          if ( !newsexp ) {
          	// ���)�A���X�o�����j 
            return i ;
          } 
          else if ( newsexp ) {
            newsexp = false ;
            if ( dot && i+1 < tokens.size() && tokens[i + 1].name != ")" ) { // Not((...).(...))
				cout << "\n> ";
                cout << "ERROR (unexpected token) : ')' expected when token at Line "<< gline <<" Column " <<gcol <<" is >>"<<tokens[i+1].name<< "<<\n" ;
                dealwitherror();
                glineProcessed = true;
                //cout << "error3 : ((...).(...)�M���٤��O)\n"; 
	      	    tokens.clear();
	      	    error_msg = true;
                return 0;  
            } 
          } 
        }   
        else {
          if ( dot && i+1 < tokens.size() && tokens[i + 1].name != ")" ) { // ( 1 . 2 3 )
				cout << "\n> ";
                cout << "ERROR (unexpected token) : ')' expected when token at Line "<< gline <<" Column " <<gcol <<" is >>"<<tokens[i+1].name<< "<<\n" ;
                dealwitherror();
                //cout << "error4 : ( 1 . 2 3 )\n"; 
                glineProcessed = true;
	      	    tokens.clear();
	      	    error_msg = true;
                return 0;
          } // if()       
        } // else
        
        if ( i+1 < tokens.size() && tokens[i + 1].name == "(" ) {
          i = parenparser_check( tokens,i + 1, error_msg ) ;     
          //�軼�j�� 
          newsexp = true ;
          i--;
        } // if()
      
    } // for()
    
    return tokens.size() ;
}

    void paser_check(vector<Token>& tokens,Node** root,bool & check){
    	int num = 0; // (++ )--
    	bool quote = false;
    	bool quote_atom = false; // �u��atom 
		bool paren = false;
		bool error_msg = false; // �A�ˬd��k���~�������N�������� 
		for (int i = 0 ; i < tokens.size(); i++) { 
	      if (tokens[i].name == "\'" ){
      	      quote = true;
      	      paren = true;
              if ( i + 1 < tokens.size() && tokens[i + 1].name == "." ) { // '.
		    	cout << "ERROR (unexpected token) : atom or '(' expected when token at Line "<< gline <<" Column " <<gcol <<" is >>"<<tokens[i+1].name<< "<<\n" ;
	      	    glineProcessed = true;
	      	    dealwitherror();
	      	    tokens.clear();
	      	    return;		      
			  }
			  
		  }
		  if (tokens[i].name == "()"){
		  	paren = true;
		  }
          if( tokens[i].name == "(" ){
    		num ++;
    		paren = true;
		  }
	      else if (tokens[i].name == ")" ){
		    num --;
		    paren = true;		  
	      }	      
		   if ( num < 0 ) { // )��(�h 
	      	cout << "\n> ";
	      	cout << "ERROR (unexpected token) : atom or '(' expected when token at Line "<< gline <<" Column " <<gcol <<" is >>"<<tokens[i].name<< "<<\n" ;
	      	dealwitherror();
	      	glineProcessed = true;;
	      	tokens.clear();
	      	return;
		  } 
      }
      
      //���T�w��k 
      parenparser_check(tokens,0,error_msg);
      
      // ��k���T�B�Lerror_msg�A�A�B�zquote
      if ( quote && num == 0 && !error_msg){ 
      	tokens = recurciveQuote(tokens);
  	  }
      //pj1�u�O��L�L�X��
	  /* 
      if (!error_msg && num == 0){
        cout << "\n> ";
        printTokens(tokens);
	    tokens.clear();
		glineProcessed = true;
		gline = 1;
		gcol = 0;	 
	  }
	  */
	  //pj2 �ؾ� 
	  if (!error_msg && num == 0){
        *root = BuildTree(tokens);
        // PrintTree(*root);
	    tokens.clear();
	    check = true;
		glineProcessed = true;
		gline = 1;
		gcol = 0;
		now = 0;	 
	  }
	      
	}
    // �ؾ� 
    int now = 0;
	Node* BuildTree(const vector<Token>& tokens) {
	    if (now >= tokens.size()) return nullptr;
	
	    Token tok = tokens[now];
	
	    if (tok.type == "LEFT-PAREN") {
	        now++; // ���L '('
	
	        if (tokens[now].type == "RIGHT-PAREN") {
	            now++; // ���L ')'
	            return new Node(true, Token{"nil", "NIL"});
	        }
	
	        Node* leftNode = BuildTree(tokens); // �J��(�N�طs���� 
	        Node* root = new Node(false, {}, leftNode, nullptr);
	        Node* cur = root;
	
	        while (now < tokens.size()) {
	            Token next = tokens[now];
	
	            if (next.type == "DOT") {
	                now++; // ���L '.'
	                Node* rightNode = BuildTree(tokens);  // �J��.�N�إk�䪺�� 
	                cur->right = rightNode;
	
	                if (tokens[now].type != "RIGHT-PAREN") {
	                    cout << "ERROR: Expected ')' after dotted pair\n";
	                    return nullptr;
	                } // ���Ӥ��|�A�P�_����k�~�i�Ӻؾ� 
	
	                now++; // ���L ')'
	                return root;
	            }
	            else if (next.type == "RIGHT-PAREN") {
	                now++; // ���L ')'
	                cur->right = new Node(true, Token{"nil", "NIL"});
	                return root;
	            }
	            else {
	                Node* newNode = new Node(false, {}, BuildTree(tokens), nullptr);
	                cur->right = newNode;
	                cur = newNode;
	            }
	        }
	    }
	    else {
	        now++; // ���L atom
	        return new Node(true, tok);
	    }
	
	    return nullptr;
	}
	
    void PrintTree(Node* node) {
        if (!node) return;

        if (node->isAtom) {
            cout << node->atom.name;
        }
        else {
            cout << "( ";
            PrintTree(node->left);
            cout << " . ";
            PrintTree(node->right);
            cout << " )";
        }
    } 
    
};

// �M������ 
class Executor{
	public:
	  void PrintSpace( int count ) {
	    for ( int i = 0 ; i < count ; i++ ) {
	      cout << " " ;     
	    } // for()
	  } // PrintSpace()
  
		void pp(Token t){
			if ( t.type == "INT"){
		      int ioutput = atoi( t.name.c_str() ) ;
		      cout << ioutput;
			}
			else if ( t.type == "FLOAT"){
		      float foutput = atof( t.name.c_str() ) ;
		      cout << fixed << setprecision(3) << foutput;
			} 
			else if ( t.type == "NIL" ) {
		      cout << "nil" ;
		    } 
		    else if ( t.type == "T" ) {
		      cout << "#t" ;
		    } 
		    else if ( t.type == "QUOTE" ) {
		      cout << "quote" ;
		    } 
		    else if (t.type == "PRIMITIVE") {
	       		cout << "#<procedure " << t.name << ">";
	    	}
		    else if ( t.type == "String" ) {
		      cout <<  t.name ;
		    } 
		    else if ( t.type == "DOT" ) {
		      cout << t.name ;  
		    } 
		    else {  // symbol
		      cout << t.name ;     
		    } 	
		}
	
		vector<Token> processTokens(vector<Token> tokens) {
		    vector<Token> processed;
		    // ��nil ����() 
		    for (const auto& token : tokens) {
		        if (token.type == "NIL") {
		            processed.emplace_back(Token{"(", "LEFT-PAREN"});
		            processed.emplace_back(Token{")", "RIGHT-PAREN"});
		        } else {
		            processed.push_back(token);
		        }
		    }
		    // �� .( �R�� ) 
		    for (size_t i = 0; i < processed.size(); ++i) {
		        if (processed[i].type == "DOT" && i + 1 < processed.size() && processed[i + 1].type == "LEFT-PAREN") {
		            size_t left = i + 1;
		            int parenCount = 1;
		            size_t j = left + 1;
		
		            while (j < processed.size() && parenCount > 0) {
		                if (processed[j].type == "LEFT-PAREN") {
		                    parenCount++;
		                } else if (processed[j].type == "RIGHT-PAREN") {
		                    parenCount--;
		                }
		                ++j;
		            }
		
		            if (parenCount != 0) {
		                // �S������٪� )�A�����Ӥ��|�A�ڥ��P�_����k�S���~�i�Ӫ� 
		                cout << "error: no matching RIGHT-PAREN" << endl;
		                return {};
		            }
		
		            processed.erase(processed.begin() + (j - 1));
		            processed.erase(processed.begin() + i, processed.begin() + i + 2);
		
		            i--;
		        }
		    }
		    return processed;
		}
		
		void printTokens(const vector<Token>& tokens) {
			int num_space = 0;
			bool paren = false;
			// �����٪�.(�P)�R�� 
			vector<Token> result = processTokens(tokens); 
			for (size_t i = 0; i < result.size(); ++i) {
			    const auto& token = result[i];
			    // �ˬd() 
			    if (token.name == "(" && i + 1 < result.size() && result[i + 1].name == ")") {
			    	if (paren) { // �W�@���L���O "("
			            cout << " ";
		                cout << "nil"<< endl;
			            paren = false;
			            ++i; // ���L�U�@�� ")"
			            continue;
			        }
			        else{
			    	  PrintSpace(num_space);
			          cout << "nil" << endl;
			          ++i; // ���L�U�@�� ")"
			          continue;
			        }
			    }
			
			    if (token.name == "(") {
			    	if (paren) { // �W�@���L���O "("
			            cout << " ";
		                cout << token.name;
		                num_space += 2;
			            paren = true;
			        }
			        else{
			          PrintSpace(num_space);
			          cout << token.name;
			          num_space += 2;
			          paren = true;
			        }  
			    } 
				else if (token.name == ")") {
					num_space -= 2;
					PrintSpace(num_space);
			        cout << token.name << endl;
			    } 
				else {
			        if (paren) { // �W�@���L���O "("
			            cout << " ";
			            pp(token);
			            cout << endl;
			            paren = false;
			        } 
					else {
			            PrintSpace(num_space);
			            pp(token);
			            cout << endl;
			            paren = false;
			        }
			    }
			}
		}


		void CleanAlltoken(vector<Token>& tokens) {
			tokens.clear();
		}		
		
		void PrintError(Node* node){
		    TokenList tree2token;
			TreeToTokens(node,tree2token); // �N���ܦ^vector �����Χڭ쥻���}�G�L 
			printTokens(tree2token);  
			CleanAlltoken(tree2token);  
		}
	
	// �����^vector ���F������pp 
	void TreeToTokens(Node* node, vector<Token>& tokens) {
	    if (!node) return;
	    if (node->isAtom) {
	        tokens.push_back(node->atom);
	        return;
	    }
	    // �}�Y��( 
	    tokens.push_back(Token{"(", "LEFT-PAREN"});
	    Node* cur = node;
	    while (cur && !cur->isAtom) {
	        // ���l��
	        TreeToTokens(cur->left, tokens);
	
	        // cdr �P�_
	        if (!cur->right) break;
	        if (cur->right->isAtom && cur->right->atom.type != "NIL") {
	            // dot pair
	            tokens.push_back(Token{".", "DOT"});
	            TreeToTokens(cur->right, tokens);
	            break;
	        }
	        else if (cur->right->isAtom && cur->right->atom.type == "NIL") {
	            // ����ڤF
	            break;
	        }
	        // �٦��U�@��
	        cur = cur->right;
	    }
	    tokens.push_back(Token{")", "RIGHT-PAREN"});
	}



	// ��parser�إX�Ӫ��𮳨ӥ�
	void gettree(Node* root){
		if ( root == NULL ){
			cout << "No tree:(" << endl; 
		}
		else{
			cout <<"\nya there is a tree";
			PrintTree(root);
			cout << endl;
		}
		root = NULL;
	}
	
	// ��¥ΨӴ��վ�إX�ӤF�S 
	void PrintTree(Node* node) {
        if (!node) return;

        if (node->isAtom) {
            cout << node->atom.name;
        }
        else {
            cout << "( ";
            PrintTree(node->left);
            cout << " . ";
            PrintTree(node->right);
            cout << " )";
        }
    } 
    
    // ���屼 
    void FreeTree(Node* node) {
	    if (node == nullptr) return;
	
	    if (!node->isAtom) {
	        FreeTree(node->left);
	        FreeTree(node->right);
	    }
	
	    delete node;
	}
	
  	// �ƻs node�קK��� 
	Node* CopyNode(Node* n) {
    	if (!n) return nullptr;
    	if (n->isAtom) return new Node(true, n->atom);
    	return new Node(false, {}, CopyNode(n->left), CopyNode(n->right));
	}

	set<string> primitiveFuncName = {
	    "cons", "car", "cdr", "list", "quote", "define",
	    "atom?", "pair?", "list?", "null?", "integer?",
	    "real?", "number?", "string?", "boolean?", "symbol?",
	    "+", "-", "*", "/", "not", "and", "or",
	    ">", ">=", "<", "<=", "=", 
	    "string-append", "string>?", "string<?", "string=?",
	    "eqv?", "equal?","if","begin","cond",
	    "clean-environment","exit"
	};


	// �p�G�O���ت�function �����@�ӷstype 
	Node* MakePrimitiveFuncNode(const string& name) {
	    return new Node(true, Token{name, "PRIMITIVE"});
	}
		
	// �T�w�O���O����Function
	bool isPrimitiveName(const string& name) {
	    return primitiveFuncName.count(name);
	}

	// �L function pointer
	void PrintProcedureName(Node* node) {
    	cout << "#<procedure " << node->atom.name << ">";
	}
	// �L function token
	void PrintProcedureNamefortoken(string& name){
		cout << "#<procedure " << name << ">";
	}
	
	// �ˬd�B�⪺�ѼƦ��S��dot pair 
	bool IsProperList(Node* node) {
	    Node* cur = node;
	    while (cur) {
	        if (cur->isAtom) {
	            // ���O NIL �N�O dotted pair
	            if (cur->atom.type != "NIL") return false;
	            return true;
	        }
	        cur = cur->right;
	    }
	    return true;
	}
	
	// �ƴX�ӰѼ� 
	int CountListArgs(Node* args) {
	    int count = 0;
	    Node* cur = args;
	    while (cur && !cur->isAtom) {
	        ++count;
	        cur = cur->right;
	    }
	    if (cur && cur->isAtom && cur->atom.type == "NIL") return count;
	    if (cur) ++count;
	    return count;
	}



		bool IsEqual(Node* a, Node* b) {
		    if (!a || !b) return false;
		    if (a->isAtom && b->isAtom) {
		        // int float ���i�H��
		        if ((a->atom.type == "INT" || a->atom.type == "FLOAT") &&
		            (b->atom.type == "INT" || b->atom.type == "FLOAT")) {
		            return stod(a->atom.name) == stod(b->atom.name);
		        }
		        // ���L�� 
		        if ((a->atom.type == "T" && b->atom.type == "T") ||
		            (a->atom.type == "NIL" && b->atom.type == "NIL")) {
		            return true;
		        }
		        // �r��
		        if (a->atom.type == "String" && b->atom.type == "String") {
		            return a->atom.name == b->atom.name;
		        }
		        // symbol/function ���O�M���e���n�@��
		        return a->atom.type == b->atom.type && a->atom.name == b->atom.name;
		    }
		    
		    // list: ���c�@�P�~ true
		    if (!a->isAtom && !b->isAtom) {
		        return IsEqual(a->left, b->left) && IsEqual(a->right, b->right);
		    }
		    // ��L����
		    return false;
		}
		
		Node* EvalEqual(Node* node) {
		    if (!IsProperList(node)) {
		        cout << "ERROR (non-list) : ";
		        PrintError(node);
		        return nullptr;
		    }
		    // ����u��ū��� 
		    if (!node->right || !node->right->left || !node->right->right || !node->right->right->left) {
		        cout << "ERROR (incorrect number of arguments) : equal?" << endl;
		        return nullptr;
		    }

		    if (CountListArgs(node->right) != 2) {
		        cout << "ERROR (incorrect number of arguments) : equal?" << endl;
		        return nullptr;
		    }
		    Node* arg1 = Eval(node->right->left, false);
		    if (!arg1) return nullptr;
		    Node* arg2 = Eval(node->right->right->left, false);
		    if (!arg2) { FreeTree(arg1); return nullptr; }

		
		    bool eq = false;
		    try {
		        eq = IsEqual(arg1, arg2);
		    } 
			catch (...) {
		        eq = false;
		    }
		    FreeTree(arg1);
		    FreeTree(arg2);
		    return new Node(true, eq ? Token{"#t", "T"} : Token{"nil", "NIL"});
		}

	
		Node* EvalEqv(Node* node) {
		    if (!IsProperList(node)) {
		        cout << "ERROR (non-list) : ";
		        PrintError(node);
		        return nullptr;
		    }
		    // �������ū��� 
		    if (!node->right || !node->right->left || !node->right->right || !node->right->right->left) {
		        cout << "ERROR (incorrect number of arguments) : eqv?" << endl;
		        return nullptr;
		    }
		    if (CountListArgs(node->right) != 2) {
		        cout << "ERROR (incorrect number of arguments) : eqv?" << endl;
		        return nullptr;
		    }
		
		    Node* raw1 = node->right->left;
		    Node* raw2 = node->right->right->left;
		
		    Node* arg1 = nullptr;
		    Node* arg2 = nullptr;
		    try {
		        arg1 = LookupPointer(raw1);
		        arg2 = LookupPointer(raw2);
		    } 
			catch (...) {
		        cout << "ERROR (eqv? internal) : runtime error" << endl;
		        return nullptr;
		    }
		
		    // �ˬd null
		    if (!arg1 || !arg2) return nullptr;
		
		    // pointer �@��
		    if (arg1 == arg2) return new Node(true, Token{"#t", "T"});
		
		    // int/float/boolean�u�n�Ȥ@�ˤ]��
		    if (arg1->isAtom && arg2->isAtom) {
		        // �Ʀr
		        if ((arg1->atom.type == "INT" || arg1->atom.type == "FLOAT") &&
		            (arg2->atom.type == "INT" || arg2->atom.type == "FLOAT")) {
		            if (stod(arg1->atom.name) == stod(arg2->atom.name))
		                return new Node(true, Token{"#t", "T"});
		        }
		        // ���L�� 
		        if ((arg1->atom.type == "T" && arg2->atom.type == "T") ||
		            (arg1->atom.type == "NIL" && arg2->atom.type == "NIL")) {
		            return new Node(true, Token{"#t", "T"});
		        }
		        // ��L�F��u�n pointer���P�N nil
		        return new Node(true, Token{"nil", "NIL"});
		    }
		    // ��L�F��u�n pointer���P�N nil
		    return new Node(true, Token{"nil", "NIL"});
		}
    
	// if/cond�� 
	bool IsFalse(Node* node) {  
	    return (node && node->isAtom && (node->atom.type == "NIL" ));
	}

	Node* Eval(Node* node,bool isTopLevel) {
	    if (!node) return nullptr;
	    // scheme => sexp & atom
	    // atom
	    if (node->isAtom) {
	        // symbol���ܬd�� 
	        if (node->atom.type == "SYMBOL" || node->atom.type == "QUOTE") {
	            // �b�w�q��table�� 
	            if (env.count(node->atom.name)) {
	                return CopyNode(env[node->atom.name]);
	            }
	            
	            // �O�w�q��function 
	            if (isPrimitiveName(node->atom.name)) {
	                return new Node(true, Token{node->atom.name, "PRIMITIVE"});
	            }
	            
	            // �����O 
	            cout << "ERROR (unbound symbol) : " << node->atom.name << endl;
	            return nullptr;
	        }
	        // ��L�^�Ǧۤv
	        return new Node(true, node->atom);
	    }
	
	    // sexp
	    if (!node || !node->left) return nullptr;
	    Node* opNode = node->left;
	    Node* func = Eval(opNode,true);
	    
	    if (!IsProperList(node) && func->atom.name != "quote") {
        	cout << "ERROR (non-list) : ";
        	PrintError(node);
        	return nullptr;
    	}
	    
		if (!func){
		  return nullptr;
		} 
	    
	    if (!func || !func->isAtom || func->atom.type != "PRIMITIVE") {
	        cout << "ERROR (attempt to apply non-function) : ";
	        if (opNode->isAtom) {
	            if (opNode->atom.type == "NIL") cout << "nil" << endl;
	            else if (opNode->atom.type == "T") cout << "#t" << endl;
	            else if ( opNode->atom.type == "SYMBOL"){
	            	if (env.count(opNode->atom.name)) {
                		return env[opNode->atom.name];
					}
				}
	            else cout << opNode->atom.name << endl;
	        } 
	        else if (func){
			    PrintError(func);
			    return nullptr;
			}
			else {
	            cout << endl;
	        }
	        return nullptr;
	    } // if
	
	    string op = func->atom.name;
	    
	    // ���ˬdexit ce def ���S���b�Ĥ@�h
		if ((op == "clean-environment" || op == "exit"|| op == "define" ) && !isTopLevel ){
			if ( op == "clean-environment"){
				cout << "ERROR (level of CLEAN-ENVIRONMENT)"<< endl;
			}
			else if ( op == "exit"){
				cout << "ERROR (level of EXIT)"<< endl;
			}
			else if( op == "define"){
				cout << "ERROR (level of DEFINE)"<< endl;
			}
			return nullptr;
		}  
	
	    // �Φ�pair 
	    if (op == "cons") {
	    	if (!IsProperList(node)) {
			    cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
			}
	        Node* arg1 = node->right ? node->right->left : nullptr; 
	        Node* arg2 = (node->right && node->right->right) ? node->right->right->left : nullptr;
	        Node* more = (node->right && node->right->right) ? node->right->right->right : nullptr; // �u�n���h�������A�N�� 
	        if (!arg1 || !arg2 || (more && (more->left || more->right))) {
	            cout << "ERROR (incorrect number of arguments) : cons" << endl;
	            return nullptr;
	        }
		    if (arg2 && !arg2->isAtom && !IsProperList(arg2)) {
		        cout << "ERROR (non-list) : ";
		        PrintError(arg2);
		        return nullptr; 
		    }
	        
	        Node* left = Eval(arg1,false);
	        if (!left) return nullptr;
	        Node* right = Eval(arg2,false);
	        if (!right) { FreeTree(left); return nullptr; }
	        return new Node(false, Token{}, left, right);
	    }
	    // �Φ�list 
	    if (op == "list") {
	    	if (!IsProperList(node)) {
			    cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
			}
	        Node* args = node->right;
	        Node* dummy = new Node(false, {}, nullptr, nullptr);
	        Node* cur = dummy;
	        while (args && args->left) {
	            Node* evaled = Eval(args->left,false);
	            if (!evaled) {
	            	return nullptr; 
				} 
				if (evaled == args->left && !evaled->isAtom) {
			        return evaled; // non-list
			    }
	            cur->right = new Node(false, {}, evaled, nullptr);
	            cur = cur->right;
	            args = args->right;
	        }
	        cur->right = new Node(true, Token{"nil", "NIL"});
	        return dummy->right;
	    }
	    
	    // ���䪺�F��ɯ� 
	    if (op == "quote") {
	        Node* quoted = node->right ? node->right->left : nullptr;

	        if (!quoted) {
	            cout << "ERROR (incorrect number of arguments) : quote" << endl;
	            return nullptr;
	        }
	        return CopyNode(quoted);
	    }
	    // �w�q 
		if (op == "define") {
			// like only define 
		    if (!node->right || !node->right->left || !node->right->right) {
		        cout << "ERROR (DEFINE format) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    //  like (define a . 5 ) 
		    if (node->right->right->isAtom && node->right->right->atom.type != "NIL") {
		        cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    //  like (define a 10 20)
		    if (node->right->right->right && !(node->right->right->right->isAtom && node->right->right->right->atom.type == "NIL")) {
		        cout << "ERROR (DEFINE format) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    // like ( define a )
		    if (!node->right->right->left) {
		        cout << "ERROR (DEFINE format) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    
		    
		    Node* sym = node->right->left;
		    Node* val = node->right->right->left;

		    if (!sym->isAtom || sym->atom.type != "SYMBOL") {
		        cout << "ERROR (DEFINE format) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    if (isPrimitiveName(sym->atom.name)) {
		        cout << "ERROR (DEFINE format) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    // ���S���~�F �����w�q 
			Node* eval_val = nullptr;
			if (val->isAtom && isPrimitiveName(val->atom.name)) {
			    eval_val = new Node(true, Token{val->atom.name, "PRIMITIVE"});
			}
			else if (val->isAtom && val->atom.type == "SYMBOL" && env.count(val->atom.name)) {
			    eval_val = env[val->atom.name]; // (define b a) 
			    // cout << "test1 : " << env[val->atom.name] << " test2 : " << eval_val;
			}
			else {
			    eval_val = Eval(val,false);
			    if (!eval_val){
			    	return nullptr;
				} 
			}
			env[sym->atom.name] = eval_val;
			cout << sym->atom.name << " defined" << endl;
			return nullptr;
		}

	    // ���k�� 
	    if (op == "car") {
		    if (!IsProperList(node)) {
		    	cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr; 
		    }
		    int argc = CountListArgs(node->right);
		    if (argc != 1) {
		        cout << "ERROR (incorrect number of arguments) : car" << endl;
		        return nullptr;
		    }
	        Node* arg = Eval(node->right->left,false);
	        if (!arg){
	        	return nullptr;
			}
	        if (!arg || arg->isAtom || !arg->left) {
	            cout << "ERROR (car with incorrect argument type) : ";
			    PrintError(arg);
			    return nullptr;
	        }
	        return CopyNode(arg->left);
	    }
	    
		// ��car�H��ѤU�� 
	    if (op == "cdr") {
		    if (!IsProperList(node)) {
		    	cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr; 
		    }
		    int argc = CountListArgs(node->right);
		    if (argc != 1) {
		        cout << "ERROR (incorrect number of arguments) : cdr" << endl;
		        return nullptr;
		    }
	        Node* arg = Eval(node->right->left,false);
	        if (!arg){
	        	return nullptr;
			}
	        if (!arg || arg->isAtom || !arg->right) {
	            cout << "ERROR (cdr with incorrect argument type) : ";
			    PrintError(arg);
			    return nullptr;
	        }
	        return CopyNode(arg->right);
	    }

	    if (op == "atom?") {
		    if (!IsProperList(node)) {
		    	cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }	    	
		    Node* args = node->right;
		    if (!args || !args->left || (args->right && args->right->left)) {
		        cout << "ERROR (incorrect number of arguments) : atom?" << endl;
		        return nullptr;
		    }
		    Node* arg = Eval(args->left,false);
		    if (!arg) return nullptr;
			if (arg == args->left && !arg->isAtom) {
			    return arg;
			}
		    Token result = (arg->isAtom ? Token{"#t", "T"} : Token{"nil", "NIL"});
		    FreeTree(arg);
		    return new Node(true, result);
		}
		
		if (op == "pair?") {
		    if (!IsProperList(node)) {
		    	cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    Node* args = node->right;
		    if (!args || !args->left || (args->right && args->right->left)) {
		        cout << "ERROR (incorrect number of arguments) : pair?" << endl;
		        return nullptr;
		    }
		    Node* arg = Eval(args->left,false);
		    if (!arg) return nullptr;
		    if (arg == args->left && !arg->isAtom) {
		        return arg;
		    }
		    Token result = (!arg->isAtom ? Token{"#t", "T"} : Token{"nil", "NIL"});
		    FreeTree(arg);
		    return new Node(true, result);
		}
		
		// ���Y�Ԫ�pair 
		if (op == "list?") {
		    if (!IsProperList(node)) {
		    	cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    Node* args = node->right;
		    if (!args || !args->left || (args->right && args->right->left)) {
		        cout << "ERROR (incorrect number of arguments) : list?" << endl;
		        return nullptr;
		    }
		    Node* arg = Eval(args->left,false);
		    if (!arg) return nullptr;
		    if (arg == args->left && !arg->isAtom) {
		        return arg;
		    }
		    bool isList = false;
		    if (arg->isAtom && arg->atom.type == "NIL") isList = true;
		    else if (!arg->isAtom) {
		        Node* cur = arg;
		        while (!cur->isAtom && cur->right) {
		            cur = cur->right;
		        }
		        if (cur && cur->isAtom && cur->atom.type == "NIL") isList = true;
		    }
		    Token result = (isList ? Token{"#t", "T"} : Token{"nil", "NIL"});
		    FreeTree(arg);
		    return new Node(true, result);
		}
		
		if (op == "null?") {
		    if (!IsProperList(node)) {
		    	cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr; 
		    }
		    Node* args = node->right;
		    if (!args || !args->left || (args->right && args->right->left)) {
		        cout << "ERROR (incorrect number of arguments) : null?" << endl;
		        return nullptr;
		    }
		    Node* arg = Eval(args->left,false);
		    if (!arg) return nullptr;
			if (arg == args->left && !arg->isAtom) {
			    return arg;
			}
		    Token result = (arg->isAtom && arg->atom.type == "NIL" ? Token{"#t", "T"} : Token{"nil", "NIL"});
		    FreeTree(arg);
		    return new Node(true, result);
		}
		
		if (op == "integer?") {
		    if (!IsProperList(node)) {
		    	cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    Node* args = node->right;
		    if (!args || !args->left || (args->right && args->right->left)) {
		        cout << "ERROR (incorrect number of arguments) : integer?" << endl;
		        return nullptr;
		    }
		    Node* arg = Eval(args->left,false);
		    if (!arg) return nullptr;
			if (arg == args->left && !arg->isAtom) {
			    return arg;
			}
		    Token result = (arg->isAtom && arg->atom.type == "INT" ? Token{"#t", "T"} : Token{"nil", "NIL"});
		    FreeTree(arg);
		    return new Node(true, result);
		}
		
		if (op == "real?" || op == "number?") {
		    if (!IsProperList(node)) {
		    	cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    Node* args = node->right;
		    if (!args || !args->left || (args->right && args->right->left)) {
		        cout << "ERROR (incorrect number of arguments) : " << op << endl;
		        return nullptr;
		    }
		    Node* arg = Eval(args->left,false);
		    if (!arg) return nullptr;
			if (arg == args->left && !arg->isAtom) {
			    return arg;
			}
		    Token result = (arg->isAtom && (arg->atom.type == "INT" || arg->atom.type == "FLOAT")
		                    ? Token{"#t", "T"} : Token{"nil", "NIL"});
		    FreeTree(arg);
		    return new Node(true, result);
		}
		
		if (op == "string?") {
		    if (!IsProperList(node)) {
		    	cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    Node* args = node->right;
		    if (!args || !args->left || (args->right && args->right->left)) {
		        cout << "ERROR (incorrect number of arguments) : string?" << endl;
		        return nullptr;
		    }
		    Node* arg = Eval(args->left,false);
		    if (!arg) return nullptr;
			if (arg == args->left && !arg->isAtom) {
			    return arg;
			}
		    Token result = (arg->isAtom && arg->atom.type == "String" ? Token{"#t", "T"} : Token{"nil", "NIL"});
		    FreeTree(arg);
		    return new Node(true, result);
		}
		
		if (op == "boolean?") {
		    if (!IsProperList(node)) {
		    	cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    Node* args = node->right;
		    if (!args || !args->left || (args->right && args->right->left)) {
		        cout << "ERROR (incorrect number of arguments) : boolean?" << endl;
		        return nullptr;
		    }
		    Node* arg = Eval(args->left,false);
		    if (!arg) return nullptr;
			if (arg == args->left && !arg->isAtom) {
			    return arg;
			}
		    Token result = (arg->isAtom && (arg->atom.type == "T" || arg->atom.type == "NIL")
		                    ? Token{"#t", "T"} : Token{"nil", "NIL"});
		    FreeTree(arg);
		    return new Node(true, result);
		}
		
		if (op == "symbol?") {
		    if (!IsProperList(node)) {
		    	cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    Node* args = node->right;
		    if (!args || !args->left || (args->right && args->right->left)) {
		        cout << "ERROR (incorrect number of arguments) : symbol?" << endl;
		        return nullptr;
		    }
		    Node* arg = Eval(args->left,false);
		    if (!arg) return nullptr;
			if (arg == args->left && !arg->isAtom) {
			    return arg;
			}
		    Token result = (arg->isAtom && arg->atom.type == "SYMBOL" ? Token{"#t", "T"} : Token{"nil", "NIL"});
		    FreeTree(arg);
		    return new Node(true, result);
		}
		
		// not �u�� ( not nil ) �~��#t ��L��nil 
		if (op == "not") {
		    if (!IsProperList(node)) {
		    	cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr; 
		    }
		    Node* args = node->right;
		    Node* arg1 = args ? args->left : nullptr;
		    Node* more = (args && args->right) ? args->right->left : nullptr; // �u�঳�@�ӰѼ� 
		    if (!arg1 || more) {
		        cout << "ERROR (incorrect number of arguments) : not" << endl;
		        return nullptr;
		    }
		    Node* arg = Eval(arg1,false);
		    if (!arg){
		    	return nullptr;
			}
		    if (arg == arg1 && !arg->isAtom) {
		        return arg;
		    }
		    Token t = (arg && arg->isAtom && arg->atom.type == "NIL") ? Token{"#t", "T"} : Token{"nil", "NIL"};
		    return new Node(true, t);
		}
		
		// and �p�G�S��nil �N�^�ǳ̫�@�ӰѼ� 
		if (op == "and") {
		    if (!IsProperList(node)) {
		    	cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr; 
		    }
		    Node* args = node->right;
		    int count = 0;
		    Node* last = nullptr;
		    while (args && args->left) {
		        last = Eval(args->left,false);
		        if (!last){
		    		return nullptr;
				}
		        if (last == args->left && !last->isAtom) {
		            return last; 
		        }
		        count++;
		        if (!last || (last->isAtom && last->atom.type == "NIL")) return new Node(true, Token{"nil", "NIL"});
		        args = args->right;
		    }
		    if (count < 2) {
		        cout << "ERROR (incorrect number of arguments) : and" << endl;
		        return nullptr;
		    }
		    return last ? CopyNode(last) : new Node(true, Token{"#t", "T"});
		}
		
		// or �^�ǲĤ@�Ӥ��Onil���Ѽ� 
		if (op == "or") {
		    if (!IsProperList(node)) {
		    	cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    Node* args = node->right;
		    int count = 0;
		    while (args && args->left) {
		        Node* res = Eval(args->left,false);
		        if (!res){
		    		return nullptr;
				}
		        if (res == args->left && !res->isAtom) {
		            return res;
		        }
		        count++;
		        if (res && (!res->isAtom || res->atom.type != "NIL")) return CopyNode(res);
		        args = args->right;
		    }
		    if (count < 2) {
		        cout << "ERROR (incorrect number of arguments) : or" << endl;
		        return nullptr;
		    }
		    return new Node(true, Token{"nil", "NIL"});
		}
		
		if (op == "+") {
		    if (!IsProperList(node)) {
		        cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    double sum = 0;
		    bool isFloat = false;
		    int count = 0;
		    Node* args = node->right;
		    while (args && args->left) {
		        Node* arg = Eval(args->left,false);
		        if (!arg) { // nullptr �N�����w�qsymbol 
		        	return nullptr;
				}
				/*
				if ( !arg->isAtom && !IsProperList(arg)) {
			        // cout << "ERROR (non-list) :";
			        return arg;
			    }
			    */
			    if (arg == args->left && !arg->isAtom) {
		            return arg; 
		        }
		        if (!arg || !arg->isAtom || (arg->atom.type != "INT" && arg->atom.type != "FLOAT")) {
		            cout << "ERROR (+ with incorrect argument type) : ";
				    PrintError(arg);
				    return nullptr;
		        }
		        if (arg->atom.type == "FLOAT") isFloat = true;
		        sum += stod(arg->atom.name);
		        count++;
		        args = args->right;
		    }
		    if (count < 2) {
		        cout << "ERROR (incorrect number of arguments) : +" << endl;
		        return nullptr;
		    }
		    if (isFloat)
		        return new Node(true, Token{to_string(sum), "FLOAT"});
		    else
		        return new Node(true, Token{to_string(int(sum)), "INT"});
		}
		
		if (op == "-") {
		    Node* args = node->right;
		    if (!IsProperList(args)) {
		        cout << "ERROR (non-list) :";
			    PrintError(node);
			    return nullptr;
		    }
		    vector<Node*> argVec;
		    while (args && args->left) {
		        Node* arg = Eval(args->left,false);
		        if (!arg) { // nullptr �N�����w�qsymbol 
		        	return nullptr;
				}
			    if (arg == args->left && !arg->isAtom) {
		            return arg;
		        }
		        if (!arg || !arg->isAtom || (arg->atom.type != "INT" && arg->atom.type != "FLOAT")) {
		            cout << "ERROR (- with incorrect argument type) : ";
				    PrintError(arg);
				    return nullptr;
		        }
		        argVec.push_back(arg);
		        args = args->right;
		    }
		    if (argVec.size() < 2) {
		        cout << "ERROR (incorrect number of arguments) : -" << endl;
		        return nullptr;
		    }
		    double result = stod(argVec[0]->atom.name);
		    bool isFloat = (argVec[0]->atom.type == "FLOAT");
		    for (size_t i = 1; i < argVec.size(); ++i) {
		        if (argVec[i]->atom.type == "FLOAT") isFloat = true;
		        result -= stod(argVec[i]->atom.name);
		    }
		    if (isFloat)
		        return new Node(true, Token{to_string(result), "FLOAT"});
		    else
		        return new Node(true, Token{to_string(int(result)), "INT"});
		}
		
		if (op == "*") {
		    Node* args = node->right;
		    if (!IsProperList(args)) {
		        cout << "ERROR (non-list) :";
			    PrintError(node);
			    return nullptr;
		    }
		    double product = 1;
		    bool isFloat = false;
		    int count = 0;
		    args = node->right;
		    while (args && args->left) {
		        Node* arg = Eval(args->left,false);
		        if (!arg) { // nullptr �N�����w�qsymbol 
		        	return nullptr;
				}
			    if (arg == args->left && !arg->isAtom) {
		            return arg;
		        }
		        if (!arg || !arg->isAtom || (arg->atom.type != "INT" && arg->atom.type != "FLOAT")) {
		            cout << "ERROR (* with incorrect argument type) : ";
				    PrintError(arg);
				    return nullptr;
		        }
		        if (arg->atom.type == "FLOAT") isFloat = true;
		        product *= stod(arg->atom.name);
		        count++;
		        args = args->right;
		    }
		    if (count < 2) {
		        cout << "ERROR (incorrect number of arguments) : *" << endl;
		        return nullptr;
		    }
		    if (isFloat)
		        return new Node(true, Token{to_string(product), "FLOAT"});
		    else
		        return new Node(true, Token{to_string(int(product)), "INT"});
		}
		
		if (op == "/") {
		    Node* args = node->right;
		    if (!IsProperList(args)) {
		        cout << "ERROR (non-list) :";
			    PrintError(node);
			    return nullptr;
		    }
		    vector<Node*> argVec;
		    while (args && args->left) {
		        Node* arg = Eval(args->left,false);
		        if (!arg) { // nullptr �N�����w�qsymbol 
		        	return nullptr;
				}
			    if (arg == args->left && !arg->isAtom) {
		            return arg;
		        }
		        if (!arg || !arg->isAtom || (arg->atom.type != "INT" && arg->atom.type != "FLOAT")) {
		            cout << "ERROR (/ with incorrect argument type) : ";
				    PrintError(arg);
				    return nullptr;
		        }
		        argVec.push_back(arg);
		        args = args->right;
		    }
		    if (argVec.size() < 2) {
		        cout << "ERROR (incorrect number of arguments) : /" << endl;
		        return nullptr;
		    }
		    double result = stod(argVec[0]->atom.name);
		    bool isFloat = (argVec[0]->atom.type == "FLOAT");
		    for (size_t i = 1; i < argVec.size(); ++i) {
		        if (argVec[i]->atom.type == "FLOAT") isFloat = true;
		        double divisor = stod(argVec[i]->atom.name);
		        if (divisor == 0) {
		            cout << "ERROR (division by zero) : /" << endl;
		            return nullptr;
		        }
		        result /= divisor;
		    }
		    if (isFloat)
		        return new Node(true, Token{to_string(result), "FLOAT"});
		    else
		        return new Node(true, Token{to_string(int(result)), "INT"});
		}
		
		if (op == ">") {
		    if (!IsProperList(node->right)) {
		        cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    vector<double> vals;
		    Node* args = node->right;
		    int cnt = 0;
		    while (args && args->left) {
		        Node* evaled = Eval(args->left,false);
		        if (!evaled) { // nullptr �N�����w�qsymbol 
		        	return nullptr;
				}
			    if (evaled == args->left && !evaled->isAtom) {
		            return evaled;
		        }
		        if (!evaled || !evaled->isAtom || (evaled->atom.type != "INT" && evaled->atom.type != "FLOAT")) {
		            cout << "ERROR (> with incorrect argument type) : ";
				    PrintError(evaled);
				    return nullptr;
		        }
		        vals.push_back(stod(evaled->atom.name));
		        args = args->right;
		        cnt++;
		    }
		    if (cnt < 2) {
		        cout << "ERROR (incorrect number of arguments) : >" << endl;
		        return nullptr;
		    }
		    bool result = true;
		    for (size_t i = 1; i < vals.size(); ++i) {
		        if (!(vals[i - 1] > vals[i])) {
		            result = false;
		            break;
		        }
		    }
		    return new Node(true, Token{result ? "#t" : "nil", result ? "T" : "NIL"});
		}
		
		if (op == ">=") {
		    if (!IsProperList(node->right)) {
		        cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    vector<double> vals;
		    Node* args = node->right;
		    int cnt = 0;
		    while (args && args->left) {
		        Node* evaled = Eval(args->left,false);
		        if (!evaled) { // nullptr �N�����w�qsymbol 
		        	return nullptr;
				}
			    if (evaled == args->left && !evaled->isAtom) {
		            return evaled;
		        }
		        if (!evaled || !evaled->isAtom || (evaled->atom.type != "INT" && evaled->atom.type != "FLOAT")) {
		            cout << "ERROR (>= with incorrect argument type) : ";
				    PrintError(evaled);
				    return nullptr;
		        }
		        vals.push_back(stod(evaled->atom.name));
		        args = args->right;
		        cnt++;
		    }
		    if (cnt < 2) {
		        cout << "ERROR (incorrect number of arguments) : >=" << endl;
		        return nullptr;
		    }
		    bool result = true;
		    for (size_t i = 1; i < vals.size(); ++i) {
		        if (!(vals[i - 1] >= vals[i])) {
		            result = false;
		            break;
		        }
		    }
		    return new Node(true, Token{result ? "#t" : "nil", result ? "T" : "NIL"});
		}

		if (op == "<") {
		    if (!IsProperList(node->right)) {
		        cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    vector<double> vals;
		    Node* args = node->right;
		    int cnt = 0;
		    while (args && args->left) {
		        Node* evaled = Eval(args->left,false);
		        if (!evaled) { // nullptr �N�����w�qsymbol 
		        	return nullptr;
				}
			    if (evaled == args->left && !evaled->isAtom) {
		            return evaled;
		        }
		        if (!evaled || !evaled->isAtom || (evaled->atom.type != "INT" && evaled->atom.type != "FLOAT")) {
		            cout << "ERROR (< with incorrect argument type) : ";
				    PrintError(evaled);
				    return nullptr;
		        }
		        vals.push_back(stod(evaled->atom.name));
		        args = args->right;
		        cnt++;
		    }
		    if (cnt < 2) {
		        cout << "ERROR (incorrect number of arguments) : <" << endl;
		        return nullptr;
		    }
		    bool result = true;
		    for (size_t i = 1; i < vals.size(); ++i) {
		        if (!(vals[i - 1] < vals[i])) {
		            result = false;
		            break;
		        }
		    }
		    return new Node(true, Token{result ? "#t" : "nil", result ? "T" : "NIL"});
		}
		
		if (op == "<=") {
		    if (!IsProperList(node->right)) {
		        cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    vector<double> vals;
		    Node* args = node->right;
		    int cnt = 0;
		    while (args && args->left) {
		        Node* evaled = Eval(args->left,false);
		        if (!evaled) { // nullptr �N�����w�qsymbol 
		        	return nullptr;
				}
			    if (evaled == args->left && !evaled->isAtom) {
		            return evaled;
		        }
		        if (!evaled || !evaled->isAtom || (evaled->atom.type != "INT" && evaled->atom.type != "FLOAT")) {
		            cout << "ERROR (<= with incorrect argument type) : ";
				    PrintError(evaled);
				    return nullptr;
		        }
		        vals.push_back(stod(evaled->atom.name));
		        args = args->right;
		        cnt++;
		    }
		    if (cnt < 2) {
		        cout << "ERROR (incorrect number of arguments) : <=" << endl;
		        return nullptr;
		    }
		    bool result = true;
		    for (size_t i = 1; i < vals.size(); ++i) {
		        if (!(vals[i - 1] <= vals[i])) {
		            result = false;
		            break;
		        }
		    }
		    return new Node(true, Token{result ? "#t" : "nil", result ? "T" : "NIL"});
		}
		
		if (op == "=") {
		    if (!IsProperList(node->right)) {
		        cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    vector<double> vals;
		    Node* args = node->right;
		    int cnt = 0;
		    while (args && args->left) {
		        Node* evaled = Eval(args->left,false);
		        if (!evaled) { // nullptr �N�����w�qsymbol 
		        	return nullptr;
				}
			    if (evaled == args->left && !evaled->isAtom) {
		            return evaled;
		        }
		        if (!evaled || !evaled->isAtom || (evaled->atom.type != "INT" && evaled->atom.type != "FLOAT")) {
		            cout << "ERROR (= with incorrect argument type) : ";
				    PrintError(evaled);
				    return nullptr;
		        }
		        vals.push_back(stod(evaled->atom.name));
		        args = args->right;
		        cnt++;
		    }
		    if (cnt < 2) {
		        cout << "ERROR (incorrect number of arguments) : =" << endl;
		        return nullptr;
		    }
		    bool result = true;
		    for (size_t i = 1; i < vals.size(); ++i) {
		        if (vals[i - 1] != vals[i]) {
		            result = false;
		            break;
		        }
		    }
		    return new Node(true, Token{result ? "#t" : "nil", result ? "T" : "NIL"});
		}
		
		if (op == "string-append") {
		    if (!IsProperList(node->right)) {
		        cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    string result = "";
		    Node* args = node->right;
		    int cnt = 0;
		    while (args && args->left) {
		        Node* evaled = Eval(args->left,false);
		        if (!evaled) { // nullptr �N�����w�qsymbol 
		        	return nullptr;
				}
			    if (evaled == args->left && !evaled->isAtom) {
		            return evaled;
		        }
		        if (!evaled || !evaled->isAtom || evaled->atom.type != "String") {
		            cout << "ERROR (string-append with incorrect argument type) : ";
				    PrintError(evaled);
				    return nullptr;
		        }
		        result += evaled->atom.name.substr(1, evaled->atom.name.length() - 2); // �h�����޸�
		        args = args->right;
		        cnt++;
		    }
		    if (cnt < 2) {
		        cout << "ERROR (incorrect number of arguments) : string-append" << endl;
		        return nullptr;
		    }
		    // ���s�W���޸�
		    return new Node(true, Token{"\"" + result + "\"", "String"});
		}

		if (op == "string>?") {
		    if (!IsProperList(node->right)) {
		        cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    vector<string> vals;
		    Node* args = node->right;
		    int cnt = 0;
		    while (args && args->left) {
		        Node* evaled = Eval(args->left,false);
		        if (!evaled) { // nullptr �N�����w�qsymbol 
		        	return nullptr;
				}
			    if (evaled == args->left && !evaled->isAtom) {
		            return evaled;
		        }
		        if (!evaled || !evaled->isAtom || evaled->atom.type != "String") {
		            cout << "ERROR (string>? with incorrect argument type) : ";
				    PrintError(evaled);
				    return nullptr;
		        }
		        // �h���e�����޸�
		        vals.push_back(evaled->atom.name.substr(1, evaled->atom.name.length() - 2));
		        args = args->right;
		        cnt++;
		    }
		    if (cnt < 2) {
		        cout << "ERROR (incorrect number of arguments) : string>?" << endl;
		        return nullptr;
		    }
		    bool result = true;
		    for (size_t i = 1; i < vals.size(); ++i) {
		        if (!(vals[i - 1] > vals[i])) {
		            result = false;
		            break;
		        }
		    }
		    return new Node(true, Token{result ? "#t" : "nil", result ? "T" : "NIL"});
		}

		if (op == "string<?") {
		    if (!IsProperList(node->right)) {
		        cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    vector<string> vals;
		    Node* args = node->right;
		    int cnt = 0;
		    while (args && args->left) {
		        Node* evaled = Eval(args->left,false);
		        if (!evaled) { // nullptr �N�����w�qsymbol 
		        	return nullptr;
				}
			    if (evaled == args->left && !evaled->isAtom) {
		            return evaled;
		        }
		        if (!evaled || !evaled->isAtom || evaled->atom.type != "String") {
		            cout << "ERROR (string<? with incorrect argument type) : ";
				    PrintError(evaled);
				    return nullptr;
		        }
		        vals.push_back(evaled->atom.name.substr(1, evaled->atom.name.length() - 2));
		        args = args->right;
		        cnt++;
		    }
		    if (cnt < 2) {
		        cout << "ERROR (incorrect number of arguments) : string<?" << endl;
		        return nullptr;
		    }
		    bool result = true;
		    for (size_t i = 1; i < vals.size(); ++i) {
		        if (!(vals[i - 1] < vals[i])) {
		            result = false;
		            break;
		        }
		    }
		    return new Node(true, Token{result ? "#t" : "nil", result ? "T" : "NIL"});
		}
		
		if (op == "string=?") {
		    if (!IsProperList(node->right)) {
		        cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    vector<string> vals;
		    Node* args = node->right;
		    int cnt = 0;
		    while (args && args->left) {
		        Node* evaled = Eval(args->left,false);
		        if (!evaled) { // nullptr �N�����w�qsymbol 
		        	return nullptr;
				}
			    if (evaled == args->left && !evaled->isAtom) {
		            return evaled;
		        }
		        if (!evaled || !evaled->isAtom || evaled->atom.type != "String") {
		            cout << "ERROR (string=? with incorrect argument type) : ";
				    PrintError(evaled);
				    return nullptr;
		        }
		        vals.push_back(evaled->atom.name.substr(1, evaled->atom.name.length() - 2));
		        args = args->right;
		        cnt++;
		    }
		    if (cnt < 2) {
		        cout << "ERROR (incorrect number of arguments) : string=?" << endl;
		        return nullptr;
		    }
		    bool result = true;
		    for (size_t i = 1; i < vals.size(); ++i) {
		        if (vals[i - 1] != vals[i]) {
		            result = false;
		            break;
		        }
		    }
		    return new Node(true, Token{result ? "#t" : "nil", result ? "T" : "NIL"});
		}
		
		if (op == "eqv?") {
		    return EvalEqv(node);
		}
		if (op == "equal?") {
		    return EvalEqual(node);
		}

		
		if (op == "begin") {
		    if (!IsProperList(node)) {
		        cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    int argc = CountListArgs(node->right);
		    if (argc < 1) {
		        cout << "ERROR (incorrect number of arguments) : begin" << endl;
		        return nullptr;
		    }
		    Node* args = node->right;
		    Node* last = nullptr;
		    while (args && args->left) {
		        if (last) FreeTree(last);
		        last = Eval(args->left,false);
		        if (!last) return nullptr; // ������ѼƥX���N�� 
		        args = args->right;
		    }
		    return last ? last : new Node(true, Token{"nil", "NIL"});
		}
		
		if (op == "if") {
		    if (!IsProperList(node)) {
		        cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
		    }
		    int argc = CountListArgs(node->right);
		    if (argc != 2 && argc != 3) {
		        cout << "ERROR (incorrect number of arguments) : if" << endl;
		        return nullptr;
		    }
		    // test, then, else
		    Node* testNode = node->right ? node->right->left : nullptr;
		    Node* thenNode = (node->right && node->right->right) ? node->right->right->left : nullptr;
		    Node* elseNode = (node->right && node->right->right && node->right->right->right)
		                        ? node->right->right->right->left : nullptr;
		
		    Node* cond = Eval(testNode,false);
		    if (!cond) return nullptr; // test�����X��
		
		    // �u�n���Onil���Otrue 
		    if (!IsFalse(cond)) {
		        FreeTree(cond);
		        if (thenNode) return Eval(thenNode,false);
		        cout << "ERROR (no return value) : ";
			    PrintError(node);
			    return nullptr;
		    } 
			else {
		        FreeTree(cond);
		        if (argc == 3 && elseNode) return Eval(elseNode,false);
		        cout << "ERROR (no return value) : ";
			    PrintError(node);
			    return nullptr;
		    }
		}



		if (op == "cond") {
		    if (!IsProperList(node)) {
		        cout << "ERROR (non-list) : ";
		        PrintError(node);
		        return nullptr;
		    }
		    Node* args = node->right;
		    // �u�� (cond) 
		    if (!args || (args->isAtom && args->atom.type == "NIL")) {
		        cout << "ERROR (COND format) : ";
		        PrintError(node);
		        return nullptr;
		    }
		
		    // �C�ӱ��󳣭n���Ѽ� 
		    Node* check = args;
		    while (check && check->left) {
		        Node* clause = check->left;
		        if (!clause || clause->isAtom || !IsProperList(clause) || CountListArgs(clause) < 2) {
		            cout << "ERROR (COND format) : ";
		            PrintError(node);
		            return nullptr;
		        }
		        check = check->right;
		    }
		    // �����n�Onil 
		    if (check && (!check->isAtom || check->atom.type != "NIL")) {
		        cout << "ERROR (COND format) : ";
		        PrintError(node);
		        return nullptr;
		    }
		
		    // �i�J evaluation
		    args = node->right;
		    while (args && args->left) {
		        Node* clause = args->left;
		        int argc = CountListArgs(clause);
		        Node* testExpr = clause->left;
		        Node* exprList = clause->right;
		        bool isLastClause = !(args->right && args->right->left);
		
		        // else �u�b�̫᭱���S��N�q 
		        if (testExpr->isAtom && testExpr->atom.type == "SYMBOL" && testExpr->atom.name == "else") {
		            if (!isLastClause) {
		                // else ���q symbol
		                Node* elseVal = nullptr;
		                if (env.count("else")) {
		                    elseVal = env["else"];
		                    if (IsFalse(elseVal)) {
		                        args = args->right;
		                        continue;
		                    }
		                } 
						else {
		                    cout << "ERROR (unbound symbol) : else" << endl;
		                    return nullptr;
		                }
		            }
		            // else �l�y�A�^�ǳ̫�@�� expr ����
		            Node* lastExpr = nullptr;
		            while (exprList && exprList->left) {
		                if (lastExpr) FreeTree(lastExpr);
		                lastExpr = Eval(exprList->left, false);
		                if (!lastExpr) return nullptr;
		                exprList = exprList->right;
		            }
		            return lastExpr ? lastExpr : new Node(true, Token{"nil", "NIL"});
		        }
		
		        // ���` clause
		        Node* condRes = Eval(testExpr, false);
		        if (!condRes) return nullptr;
		        if (!IsFalse(condRes)) {
		            FreeTree(condRes);
		            // �S�� expr-list�A�u�^�� testExpr
		            if (!exprList || (exprList->isAtom && exprList->atom.type == "NIL"))
		                return CopyNode(testExpr);
		            // ���h�� body�A�^�ǳ̫�@�� expr
		            Node* lastExpr = nullptr;
		            while (exprList && exprList->left) {
		                if (lastExpr) FreeTree(lastExpr);
		                lastExpr = Eval(exprList->left, false);
		                if (!lastExpr) return nullptr;
		                exprList = exprList->right;
		            }
		            return lastExpr ? lastExpr : new Node(true, Token{"nil", "NIL"});
		        }
		        FreeTree(condRes);
		
		        args = args->right;
		    }
		    // �S�R������@����A�S�� else
		    cout << "ERROR (no return value) : ";
		    PrintError(node);
		    return nullptr;
		}



		if (op == "clean-environment" ) {
		    if (CountListArgs(node->right) != 0) {
		        cout << "ERROR (incorrect number of arguments) : clean-environment" << endl;
		        return nullptr;
		    }
		    return nullptr;
		}
		
		if (op == "exit") {
		    if (CountListArgs(node->right) != 0) {
		        cout << "ERROR (incorrect number of arguments) : exit" << endl;
		        return nullptr;
		    }
            return nullptr; 
		}



	
	    // ���Ӥ��|�_���ۦ� ���٬O�nretrun�F�� 
	    cout << " What happened??? " << op << endl;
	    return nullptr;
	}

	Node* LookupPointer(Node* raw) {
	    if (!raw) return nullptr;
	    if (raw->isAtom && raw->atom.type == "SYMBOL" && env.count(raw->atom.name)) {
	        return env[raw->atom.name];
	    }
	    // ��L���A�N eval
	    return Eval(raw,false);
	}


	
	void PrintSymbolTable() {
	    cout << "---- Symbol Table ----" << endl;
	    for (const auto& pair : env) {
	        cout << pair.first << " : ";
	        if (pair.second->isAtom)
	            cout << pair.second->atom.name;
	        else
	            cout << "(cons-cell)";
	        cout << endl;
	    }
	    cout << "----------------------" << endl;
	}
	
	void ClearSymbolTable() {
	    set<Node*> visited;
	    for (auto& pair : env) {
	        if (pair.second && visited.find(pair.second) == visited.end()) {
	            FreeTree(pair.second);
	            visited.insert(pair.second);
	        }
	    }
	    env.clear();
	}
	



};

int main() {
	Tokenizer tokenizer;
	Parser parser;
	Executor executor;
	TokenList tokenlist;
	Node* root = nullptr;
	Token token;
	int testnum;
	cin >> testnum;
	/*
	if (testnum == 2){
		cout << "\n debug time >< \n";
	}
	*/
    cout << "Welcome to OurScheme!\n";
    //initialize();
    glineProcessed = true;
	while(true){
		token = tokenizer.gettoken(cin);
		tokenlist.push_back(token);
		// ��comment �qlist���R�� 
		tokenizer.RemoveComments(tokenlist);
		// cout << "Now at Line "<< gline <<" Column " << gcol << "\n" ; 
		// cout << token.name << "/" << token.type << endl;
		
		if ( tokenizer.Isexit(tokenlist) || token.type == "EOF" ) {
			cout << "\n> ";
			if ( token.type == "EOF" )
			  cout << "ERROR (no more input) : END-OF-FILE encountered";
			break;
		} // ����
		
		if ( token.type == "Comment" ){ 
            if (glineProcessed){
            	glineProcessed = false;
            	gline =1;
            	gcol = 0;
			}
			continue;
		} // �Ocomment�N�~��Ū
		
		if ( tokenizer.Isclean_environment(tokenlist)){
			tokenizer.CleanAlltoken(tokenlist);
			executor.ClearSymbolTable();
			cout <<"\n> environment cleaned\n";
			continue; 
		}
		
		// �u�n���s�F��i�ӤF 
		glineProcessed = false; 
		
		if ( token.name == "NCQ" ){
			cout << "\n> ";
			cout << "ERROR (no closing quote) : END-OF-LINE encountered at Line " << gline << " Column " << gcol << endl;
			tokenizer.CleanAlltoken(tokenlist);
		    gline = 1;
		    gcol = 0;
			continue; 
		} // �᭱���κ�
		 
		if (token.name == "test"){
			tokenizer.ShowAlltoken(tokenlist);
		}  // ���ե� 
		
		if (token.name == "\'"){
			continue;
		} // ���줣�O'���F��i�ӦA�B�z 
		
		if (parser.OnlyAtom(tokenlist) && token.name != "NCQ") {
		    cout << "\n> ";
		    // �u���@�� atom
		    if (tokenlist[0].type == "SYMBOL") {
		    	// ���T�w�O�_�btable�� 
		        if (env.count(tokenlist[0].name)) {
		            TokenList envTokens;
		            executor.TreeToTokens(env[tokenlist[0].name], envTokens);
		            if (envTokens.size() == 1) { // a ��¬O 3 , "string" 
		                parser.printOnlyAtom(envTokens);
		                cout << "\n";
		            } 
					else { // a �O sexp 
		                parser.printTokens(envTokens);
		            }
		        } 
		        // �O����fun 
		        else if( executor.isPrimitiveName(tokenlist[0].name)){
		        	executor.PrintProcedureNamefortoken(tokenlist[0].name);
		        	cout << "\n";
				}
				else {
		            cout << "ERROR (unbound symbol) : " << tokenlist[0].name;
		            cout << "\n";
		        }
		    } 
			else {
		        // ���Osymbol�A�����L
		        parser.printOnlyAtom(tokenlist);
		        cout << "\n";
		    }
		    tokenizer.CleanAlltoken(tokenlist);
		    glineProcessed = true;
		    gline = 1;
		    gcol = 0;
		} // �S���A�������L 
		
		else{
		  //cout << "come?\n";
		  //tokenizer.ShowAlltoken(tokenlist);
		  //cout << token.name << "/" << token.type << endl; 
		  bool parsercheck = false;
		  parser.paser_check(tokenlist,&root,parsercheck);
		  // parsercheck �u���b��k���T�B�A����٫�~�|�Otrue 
		  if (parsercheck){
		  	// executor.gettree(root); // �T�w�𦳨S���ئn
			cout << "\n> "; 
		  	Node* result = executor.Eval(root,true); // �B�� 
		  	TokenList tree2token;
		  	executor.TreeToTokens(result,tree2token); // �N���ܦ^vector �����Χڭ쥻���}�G�L 
		  	// tokenizer.ShowAlltoken(tree2token);
			parser.printTokens(tree2token);  
			tokenizer.CleanAlltoken(tree2token);  
		  	executor.FreeTree(root); // ��AST�屼 
		  	//executor.PrintSymbolTable(); // ���ӽT�w�ܼƪ��F��O�_���T 
		  	root = nullptr;
		  	// result = nullptr;
		  } // ����k��check���A����
		  else{
		  	executor.FreeTree(root);
		  	root = nullptr;		  	
		  } // �����屼�H�K��memory 

		}

	}
	
	
	cout << "\nThanks for using OurScheme!\n";
    return 0;
	
	

}

