#include <bits/stdc++.h>
using namespace std;

int gline = 1;
int gcol = 0;
bool glineProcessed = false; // 上一個東西處理完則為true

struct Token{
	string name;
	string type; 
};

// 文法樹使用
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
// 專門切token 
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
        	token = "\""+token+ "\""; //補雙引號給他 
            return true; // 結束字串
        }
        if (ch == '\n') {
            return false; // 跨行錯誤
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
  	
  // 單純以separators分token不分類 
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
        // 跳過空白
        if (is_whitespace(ch)) continue;
        // 處理註解
        if (ch == ';') {
            while (input_stream.peek() != '\n' && input_stream.peek() != EOF) {
                input_stream.get();
                gcol++;
            }
            ch = input_stream.get();//把換行讀進來
            if (ch == '\n') {
              gline++;
              gcol = 0;
            }
            Token.name = "";
            Token.type = "Comment";
            return Token;
        }
        // 處理字串
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
        // 處理() 
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
        // 一般 token，讀到遇到分隔符號為止
        t += ch;
        while (input_stream.get(ch) && !is_separator(ch) && !is_whitespace(ch)) {
            gcol++;
            t += ch;
        }
        if (!is_separator(ch)) {
            // 若是空白，回退一個字元
            input_stream.unget();
        } else {
            // 若是分隔符，留下來給下一個 token 用
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
  
  // 分類token 
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

// 專門判斷文法，pj2以後的種樹 
class Parser {
public:
  void ShowAlltoken(vector<Token>& tokens) {
    for (int i = 0 ; i < tokens.size(); i++) {
      cout << "NO." << i << " token is " << tokens[i].name <<" type is " <<  tokens[i].type << endl;
    }
  }
  
  // 有錯後面都跳過 
  void dealwitherror(){
  	  // 配合gettoken裡面的換行判斷，把\n留給他做成新的開始 
      while ( cin.peek() != '\n' && cin.peek() != EOF ) {
        cin.get() ;      
        cin.peek() ;
        
      } // while()
		
  } // dealwitherror()
  
  // 只有atom，不用判斷文法直接印出來	
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
      else if ( tokens[0].type == "DOT" ) { //單獨" . "就直接印錯 
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
	    // 把nil 換成() 
	    for (const auto& token : tokens) {
	        if (token.type == "NIL") {
	            processed.emplace_back(Token{"(", "LEFT-PAREN"});
	            processed.emplace_back(Token{")", "RIGHT-PAREN"});
	        } else {
	            processed.push_back(token);
	        }
	    }
	    // 找 .( 刪掉 ) 
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
	                // 沒有找到對稱的 )，但應該不會，我先判斷完文法沒錯才進來的 
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


	// 只要遇到quote，就重新呼叫 
	vector<Token> processQuote(const vector<Token>& tokens, int& idx) {
	    if (idx >= tokens.size()) return {};
	
	    // 找到 quote 就變成 ( quote (...) )
	    if (tokens[idx].name == "'") {
	        idx++; // 從'的下一個開始處理 
	        vector<Token> quoted = processQuote(tokens, idx);
	        vector<Token> result;
	        result.push_back(Token{"(", "LEFT-PAREN"});
	        result.push_back(Token{"quote", "QUOTE"});
	        result.insert(result.end(), quoted.begin(), quoted.end());
	        result.push_back(Token{")", "RIGHT-PAREN"});
	
	        return result;
	    }
	
	    // 遇到 ()，處理()裡的東西 
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
	        return result; // 找到完整的()回傳 
	    }
	
	    // 遇到atom直接回傳 
	    return { tokens[idx++] };
	}
	
	// 用遞迴處理quote 
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
		// 先把對稱的.(與)刪掉 
		vector<Token> result = processTokens(tokens); 
		for (size_t i = 0; i < result.size(); ++i) {
		    const auto& token = result[i];
		    // 檢查() 
		    if (token.name == "(" && i + 1 < result.size() && result[i + 1].name == ")") {
		    	if (paren) { // 上一次印的是 "("
		            cout << " ";
	                cout << "nil"<< endl;
		            paren = false;
		            ++i; // 跳過下一個 ")"
		            continue;
		        }
		        else{
		    	  PrintSpace(num_space);
		          cout << "nil" << endl;
		          ++i; // 跳過下一個 ")"
		          continue;
		        }
		    }
		
		    if (token.name == "(") {
		    	if (paren) { // 上一次印的是 "("
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
		        if (paren) { // 上一次印的是 "("
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
// 用遞迴確定是否一組 sexp 裡面是否只有一個點 
    int parenparser_check(vector<Token>& tokens,int index,bool &error_msg){
    	bool dot = false;
    	bool newsexp = false;
    	for (int i = index; i < tokens.size()&&!error_msg; i++){
    	  if( tokens[i].name == "." ){ // (. .. .) 都錯 
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
          	// 找到)，跳出這次遞迴 
            return i ;
          } 
          else if ( newsexp ) {
            newsexp = false ;
            if ( dot && i+1 < tokens.size() && tokens[i + 1].name != ")" ) { // Not((...).(...))
				cout << "\n> ";
                cout << "ERROR (unexpected token) : ')' expected when token at Line "<< gline <<" Column " <<gcol <<" is >>"<<tokens[i+1].name<< "<<\n" ;
                dealwitherror();
                glineProcessed = true;
                //cout << "error3 : ((...).(...)然後還不是)\n"; 
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
          //剛遞迴完 
          newsexp = true ;
          i--;
        } // if()
      
    } // for()
    
    return tokens.size() ;
}

    void paser_check(vector<Token>& tokens,Node** root,bool & check){
    	int num = 0; // (++ )--
    	bool quote = false;
    	bool quote_atom = false; // 只有atom 
		bool paren = false;
		bool error_msg = false; // 再檢查文法的途中有錯就直接結束 
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
		   if ( num < 0 ) { // )比(多 
	      	cout << "\n> ";
	      	cout << "ERROR (unexpected token) : atom or '(' expected when token at Line "<< gline <<" Column " <<gcol <<" is >>"<<tokens[i].name<< "<<\n" ;
	      	dealwitherror();
	      	glineProcessed = true;;
	      	tokens.clear();
	      	return;
		  } 
      }
      
      //先確定文法 
      parenparser_check(tokens,0,error_msg);
      
      // 文法正確且無error_msg，再處理quote
      if ( quote && num == 0 && !error_msg){ 
      	tokens = recurciveQuote(tokens);
  	  }
      //pj1只是把他印出來
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
	  //pj2 種樹 
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
    // 建樹 
    int now = 0;
	Node* BuildTree(const vector<Token>& tokens) {
	    if (now >= tokens.size()) return nullptr;
	
	    Token tok = tokens[now];
	
	    if (tok.type == "LEFT-PAREN") {
	        now++; // 跳過 '('
	
	        if (tokens[now].type == "RIGHT-PAREN") {
	            now++; // 跳過 ')'
	            return new Node(true, Token{"nil", "NIL"});
	        }
	
	        Node* leftNode = BuildTree(tokens); // 遇到(就種新的樹 
	        Node* root = new Node(false, {}, leftNode, nullptr);
	        Node* cur = root;
	
	        while (now < tokens.size()) {
	            Token next = tokens[now];
	
	            if (next.type == "DOT") {
	                now++; // 跳過 '.'
	                Node* rightNode = BuildTree(tokens);  // 遇到.就種右邊的樹 
	                cur->right = rightNode;
	
	                if (tokens[now].type != "RIGHT-PAREN") {
	                    cout << "ERROR: Expected ')' after dotted pair\n";
	                    return nullptr;
	                } // 應該不會，判斷完文法才進來種樹 
	
	                now++; // 跳過 ')'
	                return root;
	            }
	            else if (next.type == "RIGHT-PAREN") {
	                now++; // 跳過 ')'
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
	        now++; // 跳過 atom
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

// 專門執行 
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
		    // 把nil 換成() 
		    for (const auto& token : tokens) {
		        if (token.type == "NIL") {
		            processed.emplace_back(Token{"(", "LEFT-PAREN"});
		            processed.emplace_back(Token{")", "RIGHT-PAREN"});
		        } else {
		            processed.push_back(token);
		        }
		    }
		    // 找 .( 刪掉 ) 
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
		                // 沒有找到對稱的 )，但應該不會，我先判斷完文法沒錯才進來的 
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
			// 先把對稱的.(與)刪掉 
			vector<Token> result = processTokens(tokens); 
			for (size_t i = 0; i < result.size(); ++i) {
			    const auto& token = result[i];
			    // 檢查() 
			    if (token.name == "(" && i + 1 < result.size() && result[i + 1].name == ")") {
			    	if (paren) { // 上一次印的是 "("
			            cout << " ";
		                cout << "nil"<< endl;
			            paren = false;
			            ++i; // 跳過下一個 ")"
			            continue;
			        }
			        else{
			    	  PrintSpace(num_space);
			          cout << "nil" << endl;
			          ++i; // 跳過下一個 ")"
			          continue;
			        }
			    }
			
			    if (token.name == "(") {
			    	if (paren) { // 上一次印的是 "("
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
			        if (paren) { // 上一次印的是 "("
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
			TreeToTokens(node,tree2token); // 將樹變回vector 直接用我原本的漂亮印 
			printTokens(tree2token);  
			CleanAlltoken(tree2token);  
		}
	
	// 把樹轉回vector 為了直接用pp 
	void TreeToTokens(Node* node, vector<Token>& tokens) {
	    if (!node) return;
	    if (node->isAtom) {
	        tokens.push_back(node->atom);
	        return;
	    }
	    // 開頭補( 
	    tokens.push_back(Token{"(", "LEFT-PAREN"});
	    Node* cur = node;
	    while (cur && !cur->isAtom) {
	        // 左子樹
	        TreeToTokens(cur->left, tokens);
	
	        // cdr 判斷
	        if (!cur->right) break;
	        if (cur->right->isAtom && cur->right->atom.type != "NIL") {
	            // dot pair
	            tokens.push_back(Token{".", "DOT"});
	            TreeToTokens(cur->right, tokens);
	            break;
	        }
	        else if (cur->right->isAtom && cur->right->atom.type == "NIL") {
	            // 到尾巴了
	            break;
	        }
	        // 還有下一個
	        cur = cur->right;
	    }
	    tokens.push_back(Token{")", "RIGHT-PAREN"});
	}



	// 把parser建出來的樹拿來用
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
	
	// 單純用來測試樹建出來了沒 
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
    
    // 把樹砍掉 
    void FreeTree(Node* node) {
	    if (node == nullptr) return;
	
	    if (!node->isAtom) {
	        FreeTree(node->left);
	        FreeTree(node->right);
	    }
	
	    delete node;
	}
	
  	// 複製 node避免改到 
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


	// 如果是內建的function 給它一個新type 
	Node* MakePrimitiveFuncNode(const string& name) {
	    return new Node(true, Token{name, "PRIMITIVE"});
	}
		
	// 確定是不是內建Function
	bool isPrimitiveName(const string& name) {
	    return primitiveFuncName.count(name);
	}

	// 印 function pointer
	void PrintProcedureName(Node* node) {
    	cout << "#<procedure " << node->atom.name << ">";
	}
	// 印 function token
	void PrintProcedureNamefortoken(string& name){
		cout << "#<procedure " << name << ">";
	}
	
	// 檢查運算的參數有沒有dot pair 
	bool IsProperList(Node* node) {
	    Node* cur = node;
	    while (cur) {
	        if (cur->isAtom) {
	            // 不是 NIL 就是 dotted pair
	            if (cur->atom.type != "NIL") return false;
	            return true;
	        }
	        cur = cur->right;
	    }
	    return true;
	}
	
	// 數幾個參數 
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
		        // int float 都可以比
		        if ((a->atom.type == "INT" || a->atom.type == "FLOAT") &&
		            (b->atom.type == "INT" || b->atom.type == "FLOAT")) {
		            return stod(a->atom.name) == stod(b->atom.name);
		        }
		        // 布林值 
		        if ((a->atom.type == "T" && b->atom.type == "T") ||
		            (a->atom.type == "NIL" && b->atom.type == "NIL")) {
		            return true;
		        }
		        // 字串
		        if (a->atom.type == "String" && b->atom.type == "String") {
		            return a->atom.name == b->atom.name;
		        }
		        // symbol/function 型別和內容都要一樣
		        return a->atom.type == b->atom.type && a->atom.name == b->atom.name;
		    }
		    
		    // list: 結構一致才 true
		    if (!a->isAtom && !b->isAtom) {
		        return IsEqual(a->left, b->left) && IsEqual(a->right, b->right);
		    }
		    // 其他情形
		    return false;
		}
		
		Node* EvalEqual(Node* node) {
		    if (!IsProperList(node)) {
		        cout << "ERROR (non-list) : ";
		        PrintError(node);
		        return nullptr;
		    }
		    // 防止只到空指標 
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
		    // 防止指到空指標 
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
		
		    // 檢查 null
		    if (!arg1 || !arg2) return nullptr;
		
		    // pointer 一樣
		    if (arg1 == arg2) return new Node(true, Token{"#t", "T"});
		
		    // int/float/boolean只要值一樣也算
		    if (arg1->isAtom && arg2->isAtom) {
		        // 數字
		        if ((arg1->atom.type == "INT" || arg1->atom.type == "FLOAT") &&
		            (arg2->atom.type == "INT" || arg2->atom.type == "FLOAT")) {
		            if (stod(arg1->atom.name) == stod(arg2->atom.name))
		                return new Node(true, Token{"#t", "T"});
		        }
		        // 布林值 
		        if ((arg1->atom.type == "T" && arg2->atom.type == "T") ||
		            (arg1->atom.type == "NIL" && arg2->atom.type == "NIL")) {
		            return new Node(true, Token{"#t", "T"});
		        }
		        // 其他東西只要 pointer不同就 nil
		        return new Node(true, Token{"nil", "NIL"});
		    }
		    // 其他東西只要 pointer不同就 nil
		    return new Node(true, Token{"nil", "NIL"});
		}
    
	// if/cond用 
	bool IsFalse(Node* node) {  
	    return (node && node->isAtom && (node->atom.type == "NIL" ));
	}

	Node* Eval(Node* node,bool isTopLevel) {
	    if (!node) return nullptr;
	    // scheme => sexp & atom
	    // atom
	    if (node->isAtom) {
	        // symbol的話查表 
	        if (node->atom.type == "SYMBOL" || node->atom.type == "QUOTE") {
	            // 在定義的table裡 
	            if (env.count(node->atom.name)) {
	                return CopyNode(env[node->atom.name]);
	            }
	            
	            // 是定義的function 
	            if (isPrimitiveName(node->atom.name)) {
	                return new Node(true, Token{node->atom.name, "PRIMITIVE"});
	            }
	            
	            // 都不是 
	            cout << "ERROR (unbound symbol) : " << node->atom.name << endl;
	            return nullptr;
	        }
	        // 其他回傳自己
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
	    
	    // 先檢查exit ce def 有沒有在第一層
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
	
	    // 形成pair 
	    if (op == "cons") {
	    	if (!IsProperList(node)) {
			    cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr;
			}
	        Node* arg1 = node->right ? node->right->left : nullptr; 
	        Node* arg2 = (node->right && node->right->right) ? node->right->right->left : nullptr;
	        Node* more = (node->right && node->right->right) ? node->right->right->right : nullptr; // 只要有多的部分，就錯 
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
	    // 形成list 
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
	    
	    // 旁邊的東西升級 
	    if (op == "quote") {
	        Node* quoted = node->right ? node->right->left : nullptr;

	        if (!quoted) {
	            cout << "ERROR (incorrect number of arguments) : quote" << endl;
	            return nullptr;
	        }
	        return CopyNode(quoted);
	    }
	    // 定義 
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
		    // 都沒錯誤了 正式定義 
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

	    // 拿右邊 
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
	    
		// 拿car以後剩下的 
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
		
		// 更嚴謹的pair 
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
		
		// not 只有 ( not nil ) 才能#t 其他都nil 
		if (op == "not") {
		    if (!IsProperList(node)) {
		    	cout << "ERROR (non-list) : ";
			    PrintError(node);
			    return nullptr; 
		    }
		    Node* args = node->right;
		    Node* arg1 = args ? args->left : nullptr;
		    Node* more = (args && args->right) ? args->right->left : nullptr; // 只能有一個參數 
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
		
		// and 如果沒有nil 就回傳最後一個參數 
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
		
		// or 回傳第一個不是nil的參數 
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
		        if (!arg) { // nullptr 代表有未定義symbol 
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
		        if (!arg) { // nullptr 代表有未定義symbol 
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
		        if (!arg) { // nullptr 代表有未定義symbol 
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
		        if (!arg) { // nullptr 代表有未定義symbol 
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
		        if (!evaled) { // nullptr 代表有未定義symbol 
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
		        if (!evaled) { // nullptr 代表有未定義symbol 
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
		        if (!evaled) { // nullptr 代表有未定義symbol 
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
		        if (!evaled) { // nullptr 代表有未定義symbol 
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
		        if (!evaled) { // nullptr 代表有未定義symbol 
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
		        if (!evaled) { // nullptr 代表有未定義symbol 
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
		        result += evaled->atom.name.substr(1, evaled->atom.name.length() - 2); // 去掉雙引號
		        args = args->right;
		        cnt++;
		    }
		    if (cnt < 2) {
		        cout << "ERROR (incorrect number of arguments) : string-append" << endl;
		        return nullptr;
		    }
		    // 重新上雙引號
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
		        if (!evaled) { // nullptr 代表有未定義symbol 
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
		        // 去掉前後雙引號
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
		        if (!evaled) { // nullptr 代表有未定義symbol 
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
		        if (!evaled) { // nullptr 代表有未定義symbol 
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
		        if (!last) return nullptr; // 有任何參數出錯就停 
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
		    if (!cond) return nullptr; // test本身出錯
		
		    // 只要不是nil都是true 
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
		    // 只有 (cond) 
		    if (!args || (args->isAtom && args->atom.type == "NIL")) {
		        cout << "ERROR (COND format) : ";
		        PrintError(node);
		        return nullptr;
		    }
		
		    // 每個條件都要有參數 
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
		    // 結尾要是nil 
		    if (check && (!check->isAtom || check->atom.type != "NIL")) {
		        cout << "ERROR (COND format) : ";
		        PrintError(node);
		        return nullptr;
		    }
		
		    // 進入 evaluation
		    args = node->right;
		    while (args && args->left) {
		        Node* clause = args->left;
		        int argc = CountListArgs(clause);
		        Node* testExpr = clause->left;
		        Node* exprList = clause->right;
		        bool isLastClause = !(args->right && args->right->left);
		
		        // else 只在最後面有特殊意義 
		        if (testExpr->isAtom && testExpr->atom.type == "SYMBOL" && testExpr->atom.name == "else") {
		            if (!isLastClause) {
		                // else 當普通 symbol
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
		            // else 子句，回傳最後一個 expr 的值
		            Node* lastExpr = nullptr;
		            while (exprList && exprList->left) {
		                if (lastExpr) FreeTree(lastExpr);
		                lastExpr = Eval(exprList->left, false);
		                if (!lastExpr) return nullptr;
		                exprList = exprList->right;
		            }
		            return lastExpr ? lastExpr : new Node(true, Token{"nil", "NIL"});
		        }
		
		        // 正常 clause
		        Node* condRes = Eval(testExpr, false);
		        if (!condRes) return nullptr;
		        if (!IsFalse(condRes)) {
		            FreeTree(condRes);
		            // 沒有 expr-list，只回傳 testExpr
		            if (!exprList || (exprList->isAtom && exprList->atom.type == "NIL"))
		                return CopyNode(testExpr);
		            // 有多個 body，回傳最後一個 expr
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
		    // 沒命中任何一條件，沒有 else
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



	
	    // 應該不會淪落自此 但還是要retrun東西 
	    cout << " What happened??? " << op << endl;
	    return nullptr;
	}

	Node* LookupPointer(Node* raw) {
	    if (!raw) return nullptr;
	    if (raw->isAtom && raw->atom.type == "SYMBOL" && env.count(raw->atom.name)) {
	        return env[raw->atom.name];
	    }
	    // 其他型態就 eval
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
		// 把comment 從list中刪除 
		tokenizer.RemoveComments(tokenlist);
		// cout << "Now at Line "<< gline <<" Column " << gcol << "\n" ; 
		// cout << token.name << "/" << token.type << endl;
		
		if ( tokenizer.Isexit(tokenlist) || token.type == "EOF" ) {
			cout << "\n> ";
			if ( token.type == "EOF" )
			  cout << "ERROR (no more input) : END-OF-FILE encountered";
			break;
		} // 結束
		
		if ( token.type == "Comment" ){ 
            if (glineProcessed){
            	glineProcessed = false;
            	gline =1;
            	gcol = 0;
			}
			continue;
		} // 是comment就繼續讀
		
		if ( tokenizer.Isclean_environment(tokenlist)){
			tokenizer.CleanAlltoken(tokenlist);
			executor.ClearSymbolTable();
			cout <<"\n> environment cleaned\n";
			continue; 
		}
		
		// 只要有新東西進來了 
		glineProcessed = false; 
		
		if ( token.name == "NCQ" ){
			cout << "\n> ";
			cout << "ERROR (no closing quote) : END-OF-LINE encountered at Line " << gline << " Column " << gcol << endl;
			tokenizer.CleanAlltoken(tokenlist);
		    gline = 1;
		    gcol = 0;
			continue; 
		} // 後面不用管
		 
		if (token.name == "test"){
			tokenizer.ShowAlltoken(tokenlist);
		}  // 測試用 
		
		if (token.name == "\'"){
			continue;
		} // 等到不是'的東西進來再處理 
		
		if (parser.OnlyAtom(tokenlist) && token.name != "NCQ") {
		    cout << "\n> ";
		    // 只有一個 atom
		    if (tokenlist[0].type == "SYMBOL") {
		    	// 先確定是否在table裡 
		        if (env.count(tokenlist[0].name)) {
		            TokenList envTokens;
		            executor.TreeToTokens(env[tokenlist[0].name], envTokens);
		            if (envTokens.size() == 1) { // a 單純是 3 , "string" 
		                parser.printOnlyAtom(envTokens);
		                cout << "\n";
		            } 
					else { // a 是 sexp 
		                parser.printTokens(envTokens);
		            }
		        } 
		        // 是內建fun 
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
		        // 不是symbol，直接印
		        parser.printOnlyAtom(tokenlist);
		        cout << "\n";
		    }
		    tokenizer.CleanAlltoken(tokenlist);
		    glineProcessed = true;
		    gline = 1;
		    gcol = 0;
		} // 沒有括號直接印 
		
		else{
		  //cout << "come?\n";
		  //tokenizer.ShowAlltoken(tokenlist);
		  //cout << token.name << "/" << token.type << endl; 
		  bool parsercheck = false;
		  parser.paser_check(tokenlist,&root,parsercheck);
		  // parsercheck 只有在文法正確且括號對稱後才會是true 
		  if (parsercheck){
		  	// executor.gettree(root); // 確定樹有沒有種好
			cout << "\n> "; 
		  	Node* result = executor.Eval(root,true); // 運算 
		  	TokenList tree2token;
		  	executor.TreeToTokens(result,tree2token); // 將樹變回vector 直接用我原本的漂亮印 
		  	// tokenizer.ShowAlltoken(tree2token);
			parser.printTokens(tree2token);  
			tokenizer.CleanAlltoken(tree2token);  
		  	executor.FreeTree(root); // 把AST砍掉 
		  	//executor.PrintSymbolTable(); // 拿來確定變數表的東西是否正確 
		  	root = nullptr;
		  	// result = nullptr;
		  } // 整串文法都check完再執行
		  else{
		  	executor.FreeTree(root);
		  	root = nullptr;		  	
		  } // 先把樹砍掉以免佔memory 

		}

	}
	
	
	cout << "\nThanks for using OurScheme!\n";
    return 0;
	
	

}

