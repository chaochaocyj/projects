// 11027164 趙怡儒 11027155 李昇亮
#include <iostream>
#include <fstream> // open, is_open, close, ignore
#include <string> // string, find_last_of, substr
#include <vector> // vector, push_back
#include <cstdlib> // system, atoi
#include <iomanip> // setw
#include <cstring>
#include <sstream>

using namespace std;

struct sinfo{
	char sid[10];
	char sname[10];
	unsigned char score[6];
	float average;
};

struct pinfo{
	char sid[10];
	char sname[10];
	unsigned char score[6];
	float average;
	int num; // 雜湊數 
};

void split(string s, vector<string>& buf){ // 分割檔案
	int current = 0; //最初由 0 的位置開始找
 	int next;
 	while (1) {
    	next = s.find_first_of("\t", current);
    	string tmp = s.substr(current, next - current);
    	buf.push_back(tmp);
		if (next == string::npos) 
			break; // 找完就跳出迴圈
  		current = next + 1; //下次由 next + 1 的位置開始找起。
 	}
}

void tobinary(string realname,string newname){
	string s ;
  	vector<string> buf;
  	vector<sinfo> og;
  	fstream myFile;
  	myFile.open(newname.c_str());
  	if ( myFile.is_open()){ // 已有二進為檔
	    cout <<"***"<< newname <<" is exist!***" << endl;
  		return;
	}
	cout <<"***"<< newname <<" is not exist!***" << endl;
	myFile.open(realname.c_str());
    fstream newFile;
    newFile.open(newname.c_str(),fstream::out|fstream::binary);
    while(getline(myFile, s)){
      split(s, buf);
    }
    
    sinfo temp;
    for(int i = 0; i < buf.size() ;i++ ){
        if (i%9 == 0) {
        	strcpy(temp.sid,buf[i].c_str());
    	}
        if (i%9 == 1)
            strcpy(temp.sname,buf[i].c_str());
        if (i%9 == 2)
            temp.score[0] = atoi(buf[i].c_str());
        if (i%9 == 3)
            temp.score[1] = atoi(buf[i].c_str());
        if (i%9 == 4)
            temp.score[2] = atoi(buf[i].c_str());
        if (i%9 == 5)
            temp.score[3] = atoi(buf[i].c_str());
        if (i%9 == 6)
            temp.score[4] = atoi(buf[i].c_str());
        if (i%9 == 7)
            temp.score[5] = atoi(buf[i].c_str());
        if (i%9 == 8) {
            temp.average = atof(buf[i].c_str());
            newFile.write((char*)&temp,sizeof(temp));
        } // if
    } // for
    
    newFile.close();
    myFile.close();
}

void getName(string &realname,string &newname){
 	string inputname;
 	realname="input000";
 	cout << "input 301,302...[0]Quit:";
 	cin >> inputname;
 	if (inputname=="0") {
        realname="0";
        return;
 	}
 	if ( inputname.length() == realname.length()) {
 		newname = inputname+".bin";
   		realname = inputname+".txt";
    }
 	else if  ( inputname.length() < realname.length() && inputname.length() == 3){
 		newname = "input"+inputname+".bin";
   		realname = "input"+inputname+".txt";
    }
 	else{
   		cout <<"NOT RIGHT!,TRY AGAIN"<< endl;
      	getName(realname,newname);
    }
}

void getoutputName(string &inputname,string newname, int casenum ){
    if (casenum == 1)
    	inputname = "quadratic" + newname.substr(5,3)+".txt";
    if (casenum == 2)
    	inputname = "double" + newname.substr(5,3)+".txt";
}


int prime(int x){
    if (x >= 1) {
        if (x == 1)
            return x;
        if (x == 2)
            return x;
        for (int i = 2; i < x; i++){
            if (x % i == 0)
                return 0;
        }
        return x;
    }
    else
        return 0;
}

int maxprime(float x){
    int max = 0;
    int i = x+1;
    bool find = false;
    while( !find ){
    	max = prime(i);
    	if( max != 0 )
    		find = true;
    	i++;	
	}
    return max;
}

bool check_space(vector<int> check_list,int hashnum){
	for ( int i = 0; i < check_list.size(); i++ ){
		if ( hashnum == check_list[i])
			return false; // 有東西了 
	}
	
	return true; // 空的 
}

void bubble(vector<int> &n){
    for(int i = n.size() ; i> 0 ;i--){
       	for(int j = 0; j < i - 1 ;j++){
           	if(n[j+1]<n[j]){
              	int t=n[j];
               	n[j]=n[j+1];
               	n[j+1]=t;
           	}
        }
	}
} // bubble

float uss(vector<int> n, int size){
	int ans = 0;
	int j =1;
	int num = 0;
	for(int i = 0; i<size;i++){
		j = 1;
		num = i;
		if ( check_space(n,i) ){ // 第一個就是空 
        	ans++;
		} 
		else{
			while(!(check_space(n,num))){ // 還是有東西 
				num = (i+j*j) % size;
            	j++;		
			}
			ans = ans + j;
		}	
	}
	
	return (float)ans/size - 1;
}
/*
float ss(vector<int> n, int size,int k){
	int ans = 0;
	int j =1;
	int num =0;
    for(int i =0; i < n.size(); i++){
    	num = n[i];
    	j =1;
		while(!(check_space(n,num))){ // 還是有東西 
			num = (n[i]+j*j) % size;
            j++;
			ans++;		
		}
	}
	
	return (float)ans/k - 1;
}
*/

void hashcase1(string newname, pinfo list[],int size){
	sinfo s;
	vector<sinfo> overflow;
	int num = 1;
	int i = 0;
	int hashnum = 0;
	bool find = true;
	float sn = 0;
	vector<int> check_list;
	ifstream myFile(newname.c_str(),fstream::in|fstream::binary);
	int k = 1;
  	while(myFile.read((char *)&s, sizeof(s))) { //一直讀到檔案結束
  		// cout << "第" << k << "筆資料"; 
  		num = 1;
  		i=0;
  		find = true;
  		while (s.sid[i] != '\0' ){
  			num = num * s.sid[i]; // hash1()
  			num = num % size;
			i++;
		}
		
		i = 1;
		hashnum = num;
		sn++;
		// cout << hashnum << " <雜湊值 ";
		if ( check_space(check_list,hashnum) ){
			strcpy(list[num].sid,s.sid);
			strcpy(list[num].sname, s.sname);
			list[num].average = s.average;
			list[num].num = hashnum;
			check_list.push_back(hashnum);
			k++;
			
		} 
		
		else{
			while(!(check_space(check_list,num))){ // 有東西了 
				num = (hashnum+i*i) % size;
				// cout << "+"<< i*i << "=" << num << endl;
				if ( i == 100 ) {  // 以防無窮  
					find = false;
					overflow.push_back(s);
					break;	
				}
            	i++;
				sn++;		
			}
			
		//	cout << num<< " <可以放的位置 \n";
		    if (find){
				strcpy(list[num].sid,s.sid);
				strcpy(list[num].sname, s.sname);
				list[num].average = s.average;
				list[num].num = hashnum;
				check_list.push_back(num);
				k++;
			}
		}
		
		
    }
    
    k--;
    myFile.close();
    bubble(check_list);
    string temp;
    getoutputName(temp,newname,1);
    ofstream newFile;
    float usn = uss(check_list, size);
    newFile.open(temp.c_str());
    newFile << " --- Hash table created by Quadratic probing ---\n";
    for(int i =0; i<size; i++){
    	if (check_space(check_list,i))
    		newFile << "["<<i<<"]\n";
    	else	
			newFile << "["<<i<<"]\t"<< list[i].num << ",\t "<< list[i].sid << ",\t "<<list[i].sname<< ",\t "<<list[i].average<<"\n";
	}
	cout <<"\nHash table has been successfully created by Quadratic probing\n";
	newFile << " ----------------------------------------------------- ";
	printf("\nunsuccessful search: %.4f comparisons on average\n", usn);
	if ( k != 0 )
		printf("\nsuccessful search: %.4f comparisons on average\n", sn/k);
	else 
		cout << "there is no data in file\n";
	newFile.close();
	
}

void hashcase2(string newname, pinfo list[],int size,int maxstep){
	sinfo s;
	vector<sinfo> overflow;
	int num = 1;
	int i = 0;
	int step = 1;
	int hashnum = 0;
	bool find = true;
	float sn = 0;
	vector<int> check_list;
	ifstream myFile(newname.c_str(),fstream::in|fstream::binary);
	int k = 1;
  	while(myFile.read((char *)&s, sizeof(s))) { //一直讀到檔案結束
  		// 初始化; 
  		num = 1;
  		step = 1;
  		i=0;
  		find = true;
  		while (s.sid[i] != '\0' ){
  			num = num * s.sid[i]; // hash1()
  			num = num % size;
  			step = step * s.sid[i]; // hash2()
  			step = step % maxstep;
			i++;
		}
					
		step = maxstep - step;

		hashnum = num;
		sn++;
		// cout << hashnum << " <雜湊值 ";
		if ( check_space(check_list,hashnum) ){
			strcpy(list[num].sid,s.sid);
			strcpy(list[num].sname, s.sname);
			list[num].average = s.average;
			list[num].num = hashnum;
			check_list.push_back(hashnum);
			k++;
			
		} 
		
		else{
			while(!(check_space(check_list,num))){ // 有東西了 
			    num = num + step;
			    if ( num >= size ){
			    	num = num % size;
				}
				if ( i == 100 ) { // 以防無窮 
					find = false;
					overflow.push_back(s);
					break;	
				}
				i++;
				sn++;		
			}
			
		//	cout << num<< " <可以放的位置 \n";
		    if (find){
				strcpy(list[num].sid,s.sid);
				strcpy(list[num].sname, s.sname);
				list[num].average = s.average;
				list[num].num = hashnum;
				check_list.push_back(num);
				k++;
			}
		}
		
		
    }
    
    k--;
    myFile.close();
    bubble(check_list);
    string temp ;
    getoutputName(temp,newname,2);
    ofstream newFile;
    newFile.open(temp.c_str());
    newFile << " --- Hash table created by Quadratic probing ---\n";
    for(int i =0; i<size; i++){
    	if (check_space(check_list,i)) // 是空的 
    		newFile << "["<<i<<"]\n";
    	else	
			newFile << "["<<i<<"]\t"<< list[i].num << ",\t "<< list[i].sid << ",\t "<<list[i].sname<< ",\t "<<list[i].average<<"\n";
	}
	newFile << "  ----------------------------------------------------- ";
	cout <<"\nHash table has been successfully created by Double hashing\n";
	if ( k != 0 )
		printf("\nsuccessful search: %.4f comparisons on average\n", sn/k);
	else 
		cout << "there is no data in file\n";
	newFile.close();
	
}
int sizeoffile(string newname){
	sinfo s;
	float num = 0.0;
	ifstream myFile(newname.c_str(),fstream::in|fstream::binary);
  	while(myFile.read((char *)&s, sizeof(s))) { //一直讀到檔案結束
  	    // cout << s.sid << "\t "<<s.sname << "\t "<< s.average << endl;
  		num++;
    }
    num = num * 1.2;
    int size = maxprime(num);
    myFile.close();
    return size;
}

int maxofstep(string newname){
	sinfo s;
	float num = 0.0;
	ifstream myFile(newname.c_str(),fstream::in|fstream::binary);
  	while(myFile.read((char *)&s, sizeof(s))) { //一直讀到檔案結束
  	    // cout << s.sid << "\t "<<s.sname << "\t "<< s.average << endl;
  		num++;
    }
    num = num / 3;
    int size = maxprime(num);
    myFile.close();
    return size;
}

void case0(string &realname,string &newname){
	getName(realname,newname);
	if (realname=="0") {
		cout << "***QUIT,BACK TO SELECT***";
        return;
 	}
    tobinary(realname,newname);
}

void case1(){
	string realname;
	string newname;
	case0(realname,newname);
	int size = sizeoffile(newname);
	pinfo list[size];
	hashcase1(newname,list,size);
}

void case2(){
	string realname;
	string newname;
	case0(realname,newname);
	int size = sizeoffile(newname);
	int maxstep = maxofstep(newname);
	pinfo list[size];
	hashcase2(newname,list,size,maxstep);
}

int main(){
  	int command = 0; // user command
	do{
		cout << endl << "******* Hash Table ******";
		cout << endl << "* 0. Quit               *";
		cout << endl << "* 1. Quadratic probing  *";
 		cout << endl << "* 2. Double hashing     *";
 		cout << endl << "*************************";
 		cout << endl << "Input a command(0, 1, 2): ";
 		cin >> command; // get the command
 		switch (command){
			case 0: break;
 			case 1:
 				case1();
 				break;
		 	case 2:
		 		case2();
 				break;
 			default: cout << endl << "Command does not exist!" << endl;
 		} // end switch
 	} while (command != 0); // '0': stop the program

} // main()
