// 11027164 ���ɾ� 
#include <iostream>
#include <fstream> // open, is_open, close, ignore
#include <string> // string, find_last_of, substr
#include <vector> // vector, push_back
#include <cstdlib> // system, atoi
#include <iomanip> // setw
#include <cstring>
#include <sstream>
using namespace std;
struct info{
	    int num; // �s�� 
        string sNum; // �եN1
        string sName; // �զW2
        string dCode; // ��N3
        string dName; // ��W4
        string day; // ��or�i5
        string rate; // ����6
        string stuNum; // �ǥͼ�7
        string tNum; // �Ѯv��8
        int gNum; // ���~�ǥͼ�9�n��j�p�s���Ʀr����A�X
        string pName; // �a�I10
        string s; // ��t11
};

struct infolist{
	vector<info> samename; // �@�˦W�r�������᭱ 
};
struct node{
    vector<infolist> data;//�@���I���u�@�� 
    node* left;
    node* right;
    node* middle;
};
typedef node * nptr;

struct avlnode{
    vector<info> data;
    avlnode* left;
    avlnode* right;
};

typedef avlnode * aptr;

class TTtree{
	public:
		int nodenum = 0;
		nptr root = NULL;
        nptr fourkid = NULL; // ��������j���٭n���|�Ψ� 
  		int height(nptr t){
      		if(t == NULL) return 0;
      		int l = height(t->left);
      		int r = height(t->right);
      		int m = height(t->middle);
            if ( l>=m && l>=r){
            	return l+1;
			}
			else if ( m>=l && m>=r){
				return m+1;
			}
			else
				return r+1;
   	 	}
   	 	
		nptr search(nptr t,info n){
			infolist temp;
			if ( t->data.size() == 1){
				if( t->data[0].samename[0].sName == n.sName){ // �W�r�ۦP
                    t->data[0].samename.push_back(n);
				}
				else if( n.sName < t->data[0].samename[0].sName){ // ��L�p
                	if( t -> left == NULL){ //�츭�l�F�����[ 
                		temp.samename.push_back(n);
						t -> data.push_back(temp);
						temp = t ->data[0];
						t ->data[0] = t ->data[1];
						t ->data[1] = temp;	     		
					}
					else{
						search(t->left,n);
					}
				}
				else{ // ��L�j 
                	if( t -> right == NULL){ //�츭�l�F�����[ 
                		temp.samename.push_back(n);
						t -> data.push_back(temp); 
					}
					else{
						search(t->right,n);
					}	
				}
			}
			else if( t->data.size() == 2){
				if( t->data[0].samename[0].sName == n.sName){ // �W�r�ۦP
                    t->data[0].samename.push_back(n);
				}
				else if( t->data[1].samename[0].sName == n.sName){ // �W�r�ۦP
                    t->data[1].samename.push_back(n);
				}
				else if( n.sName < t->data[0].samename[0].sName){ // ��L�p
                	if( t -> left == NULL){ //�츭�l�F�����[ 
                	    temp.samename.push_back(n);
						t -> data.push_back(temp);
						temp = t ->data[2];
						t ->data[2] = t ->data[1];
						t ->data[1] = t ->data[0];
						t ->data[0] = temp;
						splitnode(t);
					}
					else{
						search(t->left,n);
					}
				}
				else if ( n.sName > t->data[0].samename[0].sName && n.sName < t->data[1].samename[0].sName){ // ���󤤶� 
					if(t -> middle == NULL){
						temp.samename.push_back(n);
						t -> data.push_back(temp);
						temp = t ->data[2];
						t ->data[2] = t ->data[1];
				        t ->data[1] = temp;
				        splitnode(t);
					}
					else{
						search(t -> middle,n);
					}
				}
				
				else if (n.sName > t->data[1].samename[0].sName){ // ����I���j
				    if( t -> right == NULL){ //�츭�l�F�����[ 
				        temp.samename.push_back(n);
                		t -> data.push_back(temp);
                		splitnode(t);
					}
					else{
						search(t->right,n);
					}	
				}
			}
			
			return t;
		}
		
		nptr findparent(nptr goal) { // �䪨��! 
			nptr p = root ;
			while ( p != NULL ) {
				if ( p -> left  == goal	|| p -> middle  == goal || p -> right  == goal ) {
					break ; // ���F 
				}  
				else if ( goal -> data[0].samename[0].sName < p -> data[0].samename[0].sName ) {// ��̥���p 
					p = p -> left ;
				}
			 	else if ( p->data.size() == 1 || goal -> data[0].samename[0].sName > p -> data[p -> data.size()-1].samename[0].sName ) {//��̥k��j�ήڥ��u���@��key
					p =  p -> right ;
				} 
				else{ 
					p = p -> middle ;
			    } 
			}
			return p ;
		}
		
		void assignkid( nptr & n, nptr & l, nptr & r ) {
			if ( fourkid -> data[0].samename[0].sName < n -> middle -> data[0].samename[0].sName ) { // ���X�Ӫ��p�Ĥ񤤶��p�A���̥��䪺�k�� 
				l -> left = n -> left ;
				l -> right = fourkid ;
				r -> left = n -> middle ;
				r -> right = n -> right ;
			}  
			else if ( fourkid -> data[0].samename[0].sName < r -> data[0].samename[0].sName ) { // ���X�Ӫ��p�Ĥ�̥k��p�A���̦��k�䪺���� 
				l -> left = n -> left ;
				l -> right = n -> middle ;
				r -> left = fourkid;
				r -> right = n -> right ;
			}
			else if ( fourkid -> data[0].samename[0].sName > r -> data[0].samename[0].sName ){ // ���X�Ӫ��p�Ĥ�̥k��j�A���̦��k�䪺�k��  
				l -> left = n -> left ;
				l -> right = n -> middle ;
				r -> left = n-> right ;
				r -> right = fourkid ;
			}
			else{
				l -> left = fourkid ;
				l -> right = n -> left ;
				r -> left = n-> middle ;
				r -> right = n -> right ;		
			}
			fourkid = NULL ; // �o�ӶW���n�A�����p�ĭn�M�� 
		}
		
		
		void splitnode(nptr & t ) {
			infolist temp;
			nptr t1 = NULL;
			nptr t2 = NULL;
			nptr t3 = NULL;
			if ( t == root ) {  
				t1 = new node();
				t2 = new node();
				t3 = new node();
				nodenum +=3;
				t1 -> data.push_back(t -> data[0]);
				t2 -> data.push_back(t -> data[1]);
				t3 -> data.push_back(t -> data[2]);
				t2 -> left = t1 ; // ���䱵�p�� 
				t2 -> right = t3 ; // �k�䱵�j�� 
				root = t2 ; 
				
				if ( fourkid != NULL ) // ���U�����]���� 
					assignkid( t, t1, t3 ) ;			
				t -> data.clear();
				delete t ;
				t = NULL ;
				nodenum--;
			}
			else {
				nptr p = findparent( t ) ; // �䪨�� 
				if ( p->data.size() == 1 ) {  
					p -> data.push_back(t -> data[1]);//������������ 
					t1 = new node();
					t2 = new node();
					nodenum+=2;
					t1 -> data.push_back(t -> data[0]);
					t2 -> data.push_back(t -> data[2]);
					
					if ( fourkid != NULL )
						assignkid( t, t1, t2 ) ;
						
					if ( p -> data[0].samename[0].sName > p->data[1].samename[0].sName ) { // �W�Ӫ�������p 
						temp = p ->data[0];
						p ->data[0] = p ->data[1];
						p ->data[1] = temp;	
						p -> left = t1 ;//�̥��䪺���쥻�N��ۤv�p�� 
						p -> middle = t2 ;	// ��������ۤv�j���@�w���Ӫ������p��(�]���쥻�N�b���������� 
					}
					else {
						p -> middle = t1 ; // ��������ۤv�p���@�w���Ӫ������j��(�]���쥻�N�b�k�� 
						p -> right = t2 ;// �̥k�䪺���쥻�N��ۤv�j�� 
					}
					t -> data.clear();
					delete t ;
					t = NULL ;
					nodenum--;
					
				}
				else if ( p->data.size() == 2 ) { // �����]���F 
					p -> data.push_back(t -> data[1]);//������������ 
					t1 = new node();
					t2 = new node();
					nodenum+=2;
					t1 -> data.push_back(t -> data[0]);
					t2 -> data.push_back(t -> data[2]);
					if ( fourkid != NULL )
						assignkid( t, t1, t2 ) ;
					if ( p -> data[0].samename[0].sName > p->data[2].samename[0].sName ) { // �W�Ӫ��O�̤p�� 
						temp = p ->data[0]; 
						p ->data[0] = p ->data[2];
						p ->data[2] = p ->data[1];
						p ->data[1] = temp ;	
						p -> left = t1 ;
					}
					else if ( p -> data[1].samename[0].sName < p->data[2].samename[0].sName ) { //�W�Ӫ��O�̤j�� 
						p -> right = t1 ;
					}
					else {  // �L�b���� 
						temp = p ->data[1];
						p ->data[1] = p ->data[2];
						p ->data[2] = temp;	
						p -> middle = t1 ;
					}
					
					fourkid = t2 ;  
					t -> data.clear();
					delete t ;
					t = NULL ;
					nodenum--;
					splitnode( p ) ; //���W�ˬd 
				}
			}
		}

		
	
		
};

class AVLtree{
  public:
  	int nodenum = 0;
  	
    int height(aptr a){
      if(a == NULL) 
	  	return 0;
      int l = height(a->left);
      int r = height(a->right);
      if(l>=r) 
	  	return l+1;
      return r+1;
    }

    int getbf(aptr t){ // �T�w��O���O���Ū� 
      if(t == NULL) return -1;
      return (height(t->left)-height(t->right));
    }

    aptr rightrotate(aptr y){ 
      aptr x = y->left;
      y->left = x->right;
      x->right = y;
      return x;
    }

    aptr leftrotate(aptr x){
      aptr y = x->right;
      x->right = y->left;
      y->left = x;
      return y;
    }
    
    aptr insert(aptr t, info n){
      if(t==NULL){ 
  			t = new avlnode();
			t -> data.push_back(n);
			t -> left = NULL;
			t -> right = NULL;
			nodenum++;
			return t;
      }

      if(n.gNum < t ->data[0].gNum) // ����p�A�h����   
        t->left = insert(t->left, n);

      else if(n.gNum > t ->data[0].gNum) // ����j�A�h�k�� 
        t->right = insert(t->right, n);
        
      else if(n.gNum == t ->data[0].gNum){  // �@�ˤj�A�[�᭱ 
	    t -> data.push_back(n);
        return t;
      }

      int bf = getbf(t);
      //4 cases:
    	if (bf > 1) { 
    		if (n.gNum < t->left->data[0].gNum) { // LL
      			return rightrotate(t);
    		} 
			else if (n.gNum > t->left->data[0].gNum) { // LR
      			t->left = leftrotate(t->left);
      			return rightrotate(t);
    		}
  		}
  		else if (bf < -1) { 
    		if (n.gNum > t->right->data[0].gNum) { // RR
      			return leftrotate(t);
    		} 
			else if (n.gNum < t->right->data[0].gNum) { // RL
      			t->right = rightrotate(t->right);
      			return leftrotate(t);
   			}
  		}
  		return t;

    }

}; 
void getName(string &realname){
 string inputname;
 realname="input000";
 cout << "input 201,202...[0]Quit:";
 cin >> inputname;
 if (inputname=="0") {
        realname="0";
        return;
 }
 if ( inputname.length() == realname.length()) {
   realname = inputname+".txt";
    }
 else if  ( inputname.length() < realname.length() && inputname.length() == 3){
   realname = "input"+inputname+".txt";
    }
 else{
   cout <<"NOT RIGHT!,TRY AGAIN"<< endl;
      getName(realname);
    }
}

void split(string s, vector<string>& buf){ // �����ɮ�
 int current = 0; //�̪�� 0 ����m�}�l��
 int next;
 while (1)
 {
    next = s.find_first_of("\t", current);
    string tmp = s.substr(current, next - current);
    buf.push_back(tmp);

  if (next == string::npos) break; // �䧹�N���X�j�� 
  current = next + 1; //�U���� next + 1 ����m�}�l��_�C
 }
}

void printrootfortttree(nptr root){
	vector<info> plist;
	for(int k = 0; k < root->data[0].samename.size() ;k++) {
		plist.push_back(root->data[0].samename[k]);
    }
    if ( root->data.size() == 2){
    	for(int k = 0; k < root->data[1].samename.size() ;k++) {
			plist.push_back(root->data[1].samename[k]);
  		}
    }
    info t;
    for(int i = plist.size() ; i> 0 ;i--){
        for(int j = 0; j < i - 1 ;j++){
            if(plist[j+1].num<plist[j].num){
            	t=plist[j];
              	plist[j]=plist[j+1];
                plist[j+1]=t;
            }

        }

    }
    for(int k = 0; k < plist.size() ;k++) {
        cout <<k+1 <<":"<< "["<<plist[k].num<<"] "<<plist[k].sName<<",";
		cout << plist[k].dName<<","<<plist[k].day<<",";
        cout << plist[k].rate<<"," << plist[k].gNum << endl;
    }
    

    
    
}

void case1(){
  string line;
  string dontneed;
  string realname;
  getName(realname);
  int k =1;

  if(realname=="0") return;
  ifstream myFile;
  myFile.open(realname.c_str());
  if(!myFile.is_open()){
    cout << "File does not exist!";
    return;
  }

  string s ;
  vector<string> buf;
  vector<info> og;

    for (int i =0;i<3;i++){
        getline(myFile, dontneed);
        // cout << dontneed;
    }
    while(getline(myFile, s)){
      split(s, buf);
    }
    info temp;
    for(int i = 0; i < buf.size() ;i++ ){
        if (i%11 == 0)
            temp.sNum = buf[i];
        if (i%11 == 1)
            temp.sName = buf[i];
        if (i%11 == 2)
            temp.dCode = buf[i];
        if (i%11 == 3)
            temp.dName = buf[i];
        if (i%11 == 4)
            temp.day = buf[i];
        if (i%11 == 5)
            temp.rate = buf[i];
        if (i%11 == 6)
            temp.stuNum = buf[i];
        if (i%11 == 7)
            temp.tNum = buf[i];
        if (i%11 == 8)
            temp.gNum = stoi(buf[i]);
        if (i%11 == 9)
            temp.pName = buf[i];
        if ( i%11 == 10 ) { // ���F
            temp.s = buf[i];
            temp.num = k;
            og.push_back(temp);
            k++;
        } // if
    }
    
    TTtree tree;
    tree.nodenum = 0;
    nptr root = NULL;
    for (int i = 0; i < og.size() ; i++){ // ��C�ө�itree�� 
		if (tree.root == NULL){ // �ڥ��٨S���F�� 
			root = new node();
			tree.nodenum++;
			infolist temp;
			temp.samename.push_back(og[i]);
			root -> data.push_back(temp);
			root -> left = NULL;
			root -> middle = NULL;
			root -> right = NULL;
			tree.root = root;
		}
		else{
			root = tree.root;
      		root = tree.search(root,og[i]);
		}
	}
	
	root = tree.root;
	cout << "Tree height = " << tree.height(root) << endl;
	cout << "Number of nodes = " << tree.nodenum << endl;
	int j = 1; 
	root = tree.root;
    printrootfortttree(root);


    myFile.close();

}

void case2(){
	  string line;
  string dontneed;
  string realname;
  getName(realname);

  if(realname=="0") return;
  ifstream myFile;
  myFile.open(realname.c_str());
  if(!myFile.is_open()){
    cout << "File does not exist!";
    return;
  }

  string s ;
  vector<string> buf;
  vector<info> og;

    for (int i =0;i<3;i++){
        getline(myFile, dontneed);
        // cout << dontneed;
    }
    while(getline(myFile, s)){
      split(s, buf);
    }
    info temp;
    int k =1;
    for(int i = 0; i < buf.size() ;i++ ){
        if (i%11 == 0)
            temp.sNum = buf[i];
        if (i%11 == 1)
            temp.sName = buf[i];
        if (i%11 == 2)
            temp.dCode = buf[i];
        if (i%11 == 3)
            temp.dName = buf[i];
        if (i%11 == 4)
            temp.day = buf[i];
        if (i%11 == 5)
            temp.rate = buf[i];
        if (i%11 == 6)
            temp.stuNum = buf[i];
        if (i%11 == 7)
            temp.tNum = buf[i];
        if (i%11 == 8)
            temp.gNum = stoi(buf[i]);
        if (i%11 == 9)
            temp.pName = buf[i];
        if ( i%11 == 10 ) { // ���F
            temp.s = buf[i];
            temp.num = k;
            og.push_back(temp);
            k++; 
        } // if
    }
    
        
    AVLtree avl;
    avl.nodenum = 0;
    aptr root = NULL;
    for (int i = 0; i < og.size() ; i++){ // ��C�ө�itree�� 
      root = avl.insert(root, og[i]);
	}
	
	cout << "Tree height = " << avl.height(root) << endl;
	cout << "Number of nodes = " << avl.nodenum << endl;
	for(int k = 0; k < root->data.size() ;k++) {
        cout <<k+1 <<":"<< "["<<root->data[k].num<<"] "<<root -> data[k].sName<<","<<root -> data[k].dName<<","<<root -> data[k].day<<",";
        cout << root -> data[k].rate<<"," << root -> data[k].gNum << endl;
    }

    myFile.close();
}
int main(){
	int command = 0; // user command
 	do{
		cout << endl << "** Search Tree Utilities  **";
 		cout << endl << "* 0. QUIT                  *";
 		cout << endl << "* 1. Build 2-3 tree        *";
 		cout << endl << "* 2. Build AVL tree        *";
 		cout << endl << "****************************";
 		cout << endl << "Input a choice(0, 1, 2): ";
 		cin >> command; // get a command
 		switch (command){
			case 0: break;
 			case 1:
 				case1();
 				break;
 			case 2:
 				case2();
 				break;
 			default:
				cout << endl << "Command does not exist!" << endl;
 		} // end switch
 	} while (command != 0); // '0': stop the program
 	system("pause"); // pause the display
 	return 0;
} // end main
