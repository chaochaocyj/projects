// 11027164 趙怡儒 11027155 李昇亮
#include <iostream>
#include <fstream> // open, is_open, close, ignore
#include <string> // string, find_last_of, substr
#include <vector> // vector, push_back
#include <cstdlib> // system, atoi
#include <iomanip> // setw
#include <cstring>
#include <sstream>
#include <queue>

using namespace std;

struct Data{
    char putID[10] = {'\0'};
    char getID[10] = {'\0'};
    float weight;
};


struct minor {
    char id[10] = {'\0'};
    float weight;
};

struct major {
    char id[10] = {'\0'};
    vector<minor> sublist;
    int visited = 0; // 標記是否已經被訪問過
    int cnt = 0; // 該主節點的連通數
};



class Graph{
	public:
	vector<major> g;
	
	void clear(){ // 清空 
		g.clear();
	}
	
	bool empty(){
		return g.empty();
	}
	
	void creatadj(vector<Data> &data){
		major temp1;
		minor temp2;
		strcpy(temp1.id,data[0].putID);
		g.push_back(temp1);
		int j = 0;
		for(int i = 0; i < data.size(); i++){
			if( strcmp(g[j].id,data[i].putID) == 0){
				strcpy(temp2.id,data[i].getID);
				temp2.weight = data[i].weight;
				g[j].sublist.push_back(temp2);
			}
			else{ // 不一樣了開新的
				strcpy(temp1.id,data[i].putID);
				g.push_back(temp1);
				j++;	
				strcpy(temp2.id,data[i].getID);
				temp2.weight = data[i].weight;
				g[j].sublist.push_back(temp2); 
			}
		}
	}
	
	void sortadj(){
		minor temp;
		for(int i = 0; i < g.size(); i++){
			for(int k = g[i].sublist.size() ; k> 0 ;k--){
       			for(int j = 0; j < k - 1 ;j++){
           			if(strcmp(g[i].sublist[j+1].id,g[i].sublist[j].id)<0){
              			temp=g[i].sublist[j];
               			g[i].sublist[j]=g[i].sublist[j+1];
               			g[i].sublist[j+1]=temp;
           			}
        		}
			}
		}	
		
	}
	
	void sortmajor(){
		major temp;
		for(int i = 0; i < g.size(); i++){
       		for(int j = 0; j < g.size() - i - 1 ;j++){
           		if(strcmp(g[j+1].id, g[j].id)<0){
              		temp=g[j];
              		g[j]=g[j+1];
               		g[j+1]=temp;
           		}
        	}
		}
		
	}
	

	void printadj(string newname){
		sortadj();
		sortmajor();
		int i =0;
		int node = 0;
		ofstream myFile;
		myFile.open(newname.c_str());
		myFile << "<<< There are "<<g.size()<<" IDs in total. >>>"<<endl;
		for(i = 0; i < g.size(); i++){
			myFile << "["<< i+1 <<"] " << g[i].id <<":\n";
			for(int j = 0; j < g[i].sublist.size(); j++){
				myFile << "\t("<< j+1 <<") " <<g[i].sublist[j].id <<"\t"<< g[i].sublist[j].weight;
				node++;
			}
			myFile << endl;
		}
		
		myFile << "<<< There are "<< node <<" nodes in total. >>>"<< endl;
		myFile.close();
		cout <<"<<< There are "<<i<<" IDs in total. >>>\n"<<endl;
		cout << "<<< There are "<< node <<" nodes in total. >>>"<< endl;
			
	}
	
	void sortcnt(){
		major temp;
		for(int i = 0; i < g.size(); i++){
       		for(int j = 0; j < g.size() - i - 1 ;j++){
           		if(g[j].cnt < g[j+1].cnt ){
              		temp=g[j];
              		g[j]=g[j+1];
               		g[j+1]=temp;
           		}
        	}
		}
	}
	
	void merge(int h, int m, int t){
            major temp;
            int n1 = m - h + 1;
            int n2 = t - (m+1)+1;
            vector<major> a;
            vector<major> b;

            for (int i =0; i< n1; i++){
                a.push_back(temp);
                a[i] = g[h + i];
            }
            for (int i =0; i< n2; i++){
                b.push_back(temp);
                b[i] = g[m + 1 + i];
            }

            int i=0;
            int j=0;
            int k=h;
            while (i < n1 && j < n2) {
                if (a[i].cnt >= b[j].cnt) {
                    g[k] = a[i];
                    i++;
                }

                else {
                    g[k] = b[j];
                    j++;
                }

                k++;
            }

            while (i < n1) {
                g[k] = a[i];
                i++;
                k++;
            }

            while (j < n2) {
                g[k] = b[j];
                j++;
                k++;
            }
        }

        void mergesort(int h, int t) {
            if (h < t) {
                int mid = (h+t) / 2;

                mergesort(h, mid);//0到中間
                mergesort( mid+1 , t);//中間+1到尾端
                merge(h, mid, t);
            }
        }		
	void bfs(int start, int &cnt){
    	queue<int> q;
    	q.push(start);
    	g[start].visited = 1;
    	cnt++;
    	vector<minor> nsublist;
    	while (!q.empty()) {
        	int curr = q.front();
        	q.pop();
        	for (auto& sub : g[curr].sublist) {
            	int next = -1;
            	for (int i = 0; i < g.size(); i++) {
                	if (strcmp(g[i].id, sub.id) == 0) { // 在主陣列中找是否有跟相鄰陣列連接的點 
                    	next = i;
                    	break;
                	}
            	}
            	if (next != -1 && !g[next].visited) { // 有此點且此點還沒探訪過 
                	q.push(next);
                	g[next].visited = 1;
                	cnt++;
                	nsublist.push_back(sub); // 把此點放進新的陣列裡 
            	}
        	}
    	}
    	
    	g[start].sublist.clear();//把舊的清空 
    	for(int i = 0; i < nsublist.size() ;i++){
    		g[start].sublist.push_back(nsublist[i]);
		}
	}
	
	void printcnt(string newname){
		sortcnt();
		sortadj();
		ofstream myFile;
		myFile.open(newname.c_str());
		cout << "<<< There are "<<g.size()<<" IDs in total. >>>"<<endl;
		myFile << "<<< There are "<<g.size()<<" IDs in total. >>>"<<endl;
		for(int i = 0; i < g.size(); i++){
			myFile << "["<< i+1 <<"] " << g[i].id <<"("<< g[i].cnt <<")"<<":\n";
			for(int j = 0; j < g[i].sublist.size(); j++){
				myFile << "\t("<< j+1 <<") " <<g[i].sublist[j].id;
			}
			myFile << endl;
		}
		
		myFile.close();
		
	}
};

void getName1(string &realname,string &newname){
 	string inputname;
 	realname="pairs000";
 	cout << "pairs 401,402...[0]Quit:";
 	cin >> inputname;
 	if (inputname=="0") {
        realname="0";
        return;
 	}
 	if ( inputname.length() == realname.length()) {
 		newname = inputname+".adj";
   		realname = inputname+".bin";
    }
 	else if  ( inputname.length() < realname.length() && inputname.length() == 3){
 		newname = "pairs"+inputname+".adj";
   		realname = "pairs"+inputname+".bin";
    }
 	else{
   		cout <<"NOT RIGHT!,TRY AGAIN"<< endl;
      	getName1(realname,newname);
    }
}

void getName2(string realname,string &newname){
	newname = realname.substr(0,8)+".cnt";
}

void write2data(string realname, vector<Data> &data){
    Data d;
    ifstream myFile;
    myFile.open(realname.c_str(), fstream::in | fstream::binary);
    if(!myFile.is_open()){
        // cout << "File does not exist!";
        return;
    }
    while (myFile.read((char *)&d, sizeof(d))){
        data.push_back(d);
    } // for
	myFile.close();
	/*
	for(int i = 0; i < data.size(); i++){
		// cout << data[i].putID <<"/"<< data[i].getID<<"/"<<data[i].weight << endl;
	}
	*/
} 

void case1(string &realname, vector<Data> &data,Graph &g){
	string newname;
	data.clear();
	getName1(realname,newname);
	if ( realname =="0")
		return;	
	write2data(realname,data);
	if (data.empty()){
        cout << "File does not exist!";
        return;
    }
	g.clear();
	g.creatadj(data);
	g.printadj(newname);
}

void case2(string &realname,Graph &g){
	string newname;
	getName2(realname,newname);
	if (g.empty()){
		cout << "There is no graph~\n";
		return;
	}
	else
		// cout << "case2 " << newname << endl; 
	if ( realname =="0")
		return;
    for (int i = 0; i < g.g.size(); i++) {
        int cnt = 0;
        g.bfs(i, cnt);
        g.g[i].cnt = cnt - 1;
        for (auto& sub : g.g) {
            sub.visited = 0;
        }
    }
    g.printcnt(newname);
}

int main(){
  	int command = 0; // user command
  	vector<Data> data;
  	string realname;
  	Graph g;
	do{
		cout << endl << "**** Graph data manipulation ****";
		cout << endl << "* 0. QUIT                       *";
		cout << endl << "* 1. Build adjacency lists      *";
 		cout << endl << "* 2. Compute connection counts  *";
 		cout << endl << "*************************";
 		cout << endl << "Input a command(0, 1, 2): ";
 		cin >> command; // get the command
 		switch (command){
			case 0: break;
 			case 1:
 				case1(realname,data,g);
 				break;
		 	case 2:
            	if(data.empty()){
            		cout << "### There is no graph and choose 1 first. ###";
					break;
				}
				case2(realname,g);
            	break;
 			default: cout << endl << "Command does not exist!" << endl;
 		} // end switch
 	} while (command != 0); // '0': stop the program

} // main()
