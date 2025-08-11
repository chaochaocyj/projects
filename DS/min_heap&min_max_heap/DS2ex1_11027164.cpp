// 11027164 趙怡儒 
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
	    int num; // 編號 
        string sNum; // 校代1
        string sName; // 校名2
        string dCode; // 科代3
        string dName; // 科名4
        string day; // 日or進5
        string rate; // 等級6
        string stuNum; // 學生數7
        string tNum; // 老師數8
        int gNum; // 畢業學生數要比大小存成數字比較適合9
        string pName; // 地點10
        string s; // 體系11
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
        	if (i != 0  && h[parent(i)].item[0].gNum > h[i].item[0].gNum) { // 等於0，沒有parent了 
        	    temp = h[parent(i)];
        		h[parent(i)] = h[i];
        		h[i] = temp; 
                adjust(parent(i)); // 繼續往上確認，畢竟有可能是加了一個最小的
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
				if ( i > h.size()-1) // 會包含最左剛好是最後一個 
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
				if ( n > i) // 會包含最左剛好是最後一個 
				  break;
			}
			
			if (count%2 != 0 ){ // 奇數層，min 
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
			i = parent(parent(i)); // 先算父節點，父節點的父節點就是阿公節點 
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
        	if (i >=3  && h[grandparent(i)].item[0].gNum > h[i].item[0].gNum) { //i<3沒有grandparent
        	    temp = h[grandparent(i)];
        		h[grandparent(i)] = h[i];
        		h[i] = temp; 
                adjust_min(grandparent(i)); // 繼續往上確認
			} 
		}
		
		void adjust_max(int i){
			infolist temp;
        	if (i >=3  && h[grandparent(i)].item[0].gNum < h[i].item[0].gNum) { // i<3沒有grandparent
        	    temp = h[grandparent(i)];
        		h[grandparent(i)] = h[i];
        		h[i] = temp; 
                adjust_max(grandparent(i)); 
			} 
		}
		
		void change_min(int i){
			infolist temp;
        	if (i != 0  && h[parent(i)].item[0].gNum > h[i].item[0].gNum) { // 等於0，沒有parent了 
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
        	if (i != 0  && h[parent(i)].item[0].gNum < h[i].item[0].gNum) { // 等於0，沒有parent了 
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
			if (!countlevel(parent(index))){ // 如果父節點是奇數層 
				change_min(index);
			}
			else {
			    change_max(index);
			}
			
			if (!countlevel(current)){ // 看看自己在哪層 
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
				if ( i > h.size()-1) // 會包含最左剛好是最後一個 
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

void split(string s, vector<string>& buf){ // 分割檔案
 int current = 0; //最初由 0 的位置開始找
 int next;
 while (1)
 {
    next = s.find_first_of("\t", current);
    string tmp = s.substr(current, next - current);
    buf.push_back(tmp);

  if (next == string::npos) break; // 找完就跳出迴圈 
  current = next + 1; //下次由 next + 1 的位置開始找起。
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
        if ( i%11 == 10 ) { // 滿了
            temp.s = buf[i];
            temp.num = k;
            og.push_back(temp);
            k++;
        } // if
    }
    
    /*  檢查有沒有畢業人數相同的 
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
    for (int i = 0; i < og.size() ; i++){ // 把每個放進heap裡 
        same = false; 
        /* 
    	for(int j = 0; j< note.h.size(); j++ ){ // 畢業人數一樣的直接放後面 
    		if( og[i].gNum == note.h[j].item[0].gNum ) {
    			note.h[j].item.push_back(og[i]);
    			same = true;
			}
		}
		
		if (!same)// 沒有一樣的，加進去 
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
        if ( i%11 == 10 ) { // 滿了
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
    
    for (int i = 0; i < og.size() ; i++){ // 把每個放進heap裡 
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
