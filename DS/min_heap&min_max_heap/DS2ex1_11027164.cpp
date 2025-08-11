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
        int gNum; // ���~�ǥͼƭn��j�p�s���Ʀr����A�X9
        string pName; // �a�I10
        string s; // ��t11
};

struct infolist{
	vector<info> item;
};

class Minheap {
	public:
		vector<infolist> h;
		int parent(int i) {
			i = (i-1)/2;
			return i;
		}
		
		int leftchild(int i) {
			i = i*2 + 1;
			return i; 
		}

        int rightchild(int i) {
        	i = i*2 + 2;
        	return i;
		}
		
		
		void adjust(int i){
			infolist temp;
        	if (i != 0  && h[parent(i)].item[0].gNum > h[i].item[0].gNum) { // ����0�A�S��parent�F 
        	    temp = h[parent(i)];
        		h[parent(i)] = h[i];
        		h[i] = temp; 
                adjust(parent(i)); // �~�򩹤W�T�{�A�������i��O�[�F�@�ӳ̤p��
			} 
		}
		
		
		void add(info a){
			infolist temp;
			temp.item.push_back(a);
			h.push_back(temp);
			int index = h.size() - 1;
        	adjust(index);
		}
		
		int leftbottom(int i){
			i =0;
			while(1){
				i = i*2+1;
				if ( i > h.size()-1) // �|�]�t�̥���n�O�̫�@�� 
				  break;
			}
			
			i = (i-1)/2;
			return i;	  
		}
};

class Min_maxheap{
	public:
		vector<infolist> h;
		int current = 0;
		bool countlevel(int i){
			int count = 0;
			int n =0;
			while(1){
				n = n*2+1;
				count++;
				if ( n > i) // �|�]�t�̥���n�O�̫�@�� 
				  break;
			}
			
			if (count%2 != 0 ){ // �_�Ƽh�Amin 
				return false;
			}
            else 
			    return true;  
		}
		
		int parent(int i) {
			i = (i-1)/2;
			return i;
		}
		
		int grandparent(int i) {
			i = parent(parent(i)); // ������`�I�A���`�I�����`�I�N�O�����`�I 
        	return i;
    	}
		
		int leftchild(int i) {
			i = i*2 + 1;
			return i; 
		}

        int rightchild(int i) {
        	i = i*2 + 2;
        	return i;
		}
		
		
		void adjust_min(int i){
			infolist temp;
        	if (i >=3  && h[grandparent(i)].item[0].gNum > h[i].item[0].gNum) { //i<3�S��grandparent
        	    temp = h[grandparent(i)];
        		h[grandparent(i)] = h[i];
        		h[i] = temp; 
                adjust_min(grandparent(i)); // �~�򩹤W�T�{
			} 
		}
		
		void adjust_max(int i){
			infolist temp;
        	if (i >=3  && h[grandparent(i)].item[0].gNum < h[i].item[0].gNum) { // i<3�S��grandparent
        	    temp = h[grandparent(i)];
        		h[grandparent(i)] = h[i];
        		h[i] = temp; 
                adjust_max(grandparent(i)); 
			} 
		}
		
		void change_min(int i){
			infolist temp;
        	if (i != 0  && h[parent(i)].item[0].gNum > h[i].item[0].gNum) { // ����0�A�S��parent�F 
        	    temp = h[parent(i)];
        		h[parent(i)] = h[i];
        		h[i] = temp;
        		change_max(parent(i));
			} 
			else {
				current = i;
			}
			
		}
		
		void change_max(int i){
			infolist temp;
        	if (i != 0  && h[parent(i)].item[0].gNum < h[i].item[0].gNum) { // ����0�A�S��parent�F 
        	    temp = h[parent(i)];
        		h[parent(i)] = h[i];
        		h[i] = temp; 
        		change_min(parent(i));
			}
			else {
				current = i;
			}
		}
		
		void add(info a){
			infolist temp;
			temp.item.push_back(a);
			h.push_back(temp);
			int index = h.size() - 1;
			current =0; 
			if (!countlevel(parent(index))){ // �p�G���`�I�O�_�Ƽh 
				change_min(index);
			}
			else {
			    change_max(index);
			}
			
			if (!countlevel(current)){ // �ݬݦۤv�b���h 
				adjust_min(current);
			}
			else{
				adjust_max(current);
			}
			
			
		}
		
		int leftbottom(int i){
			i =0;
			while(1){
				i = i*2+1;
				if ( i > h.size()-1) // �|�]�t�̥���n�O�̫�@�� 
				  break;
			}
			
			i = (i-1)/2;
			return i;	  
		}
	
		
};

void getName(string &realname){
 string inputname;
 realname="input000";
 cout << "input 100,101...[0]Quit:";
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
    
    /*  �ˬd���S�����~�H�ƬۦP�� 
    bool ox = false;
    for (int i =0; i<og.size();i++){
    	for(int j =1; j<og.size();j++){
    		if ( og[i].gNum == og[j].gNum && i != j) {
    		  cout << i << "," << j << endl;
			   ox = true;
    	    }
		}
	}
	
	if ( !ox )
	  cout << "no same!!"<< endl;
    */ 
    
    
    Minheap note;
    bool same = false;
    for (int i = 0; i < og.size() ; i++){ // ��C�ө�iheap�� 
        same = false; 
        /* 
    	for(int j = 0; j< note.h.size(); j++ ){ // ���~�H�Ƥ@�˪�������᭱ 
    		if( og[i].gNum == note.h[j].item[0].gNum ) {
    			note.h[j].item.push_back(og[i]);
    			same = true;
			}
		}
		
		if (!same)// �S���@�˪��A�[�i�h 
		*/
		note.add(og[i]);

	}
    

    cout << "<min heap>" <<endl;
    cout << "root:" << "["<<note.h[0].item[0].num<<"]" << note.h[0].item[0].gNum << endl ;
    cout << "bottom:" << "["<<note.h[note.h.size()-1].item[0].num<<"]" << note.h[note.h.size()-1].item[0].gNum<< endl;
    cout << "leftmost bottom:"<< "["<<note.h[note.leftbottom(0)].item[0].num<<"]" << note.h[note.leftbottom(0)].item[0].gNum<< endl; 
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
    
    Min_maxheap note;
    /*
    bool ox = false;
    for (int i =0; i<og.size();i++){
    	for(int j =1; j<og.size();j++){
    		if ( og[i].gNum == og[j].gNum && i != j) {
    		  cout << i << "," << j << endl;
			   ox = true;
    	    }
		}
	}
	
	if ( !ox )
	  cout << "no same!!"<< endl;
    */
    
    for (int i = 0; i < og.size() ; i++){ // ��C�ө�iheap�� 
		note.add(og[i]);
	}
    cout << "<min-max heap>" <<endl;
    cout << "root:" << "["<<note.h[0].item[0].num<<"]" << note.h[0].item[0].gNum << endl ;
    cout << "bottom:" << "["<<note.h[note.h.size()-1].item[0].num<<"]" << note.h[note.h.size()-1].item[0].gNum<< endl;
    cout << "leftmost bottom:"<< "["<<note.h[note.leftbottom(0)].item[0].num<<"]" << note.h[note.leftbottom(0)].item[0].gNum<< endl; 


    myFile.close();

}


int main(){
	int command = 0; // user command
 	do{
		cout << endl << "*** File Object Manipulator ***";
 		cout << endl << "* 0. QUIT        *";
 		cout << endl << "* 1. min heap    *";
 		cout << endl << "* 2. min-max heap*";
 		cout << endl << "*********************************";
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
