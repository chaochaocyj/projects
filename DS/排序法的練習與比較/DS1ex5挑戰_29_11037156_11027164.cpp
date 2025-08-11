// 11027156 �L��w 11027164 ���ɾ�
#include <iostream>
#include <fstream> // open, is_open, close, ignore
#include <string> // string, find_last_of, substr
#include <vector> // vector, push_back
#include <cstdlib> // system, atoi
#include <iomanip> // setw
#include <cstring>
#include <ctime> // clock_t, clock, CLOCKS_PER_SEC
#include <sstream>

using namespace std;
int c4num = 0 ;

struct info{
        string sNum; // �եN
        string sName; // �զW
        string dCode; // ��N
        string dName; // ��W
        string day; // ��or�i
        string rate; // ����
        string stuNum; // �ǥͼ�
        string tNum; // �Ѯv��
        int gNum; // ���~�Ƥ�j�p�s���Ʀr����A�X
        string pName; // �a�I
        string s; // ��t
};

class Sorts{
	public:
    	vector<info> n;
    	void add(info item){
			n.push_back(item);
		}

		void clear(){
			n.clear();
		}

		int size(){
			return n.size();
		}

		void select(){
			info t;
			int max;
    		for(int i =0;i < size();i++){
    			max = i;
        		for(int j =i+1;j < size();j++){
            		if(n[j].gNum > n[max].gNum){
						max = j; // ���@�Ӥ���j��
           			}
        		}
        		t=n[max];
                n[max]=n[i];
                n[i]=t;

    		}
		} // select

		void bubble(){
			info t;
    		for(int i = size() ; i> 0 ;i--){
        		for(int j = 0; j < i - 1 ;j++){
            		if(n[j+1].gNum>n[j].gNum){
                		t=n[j];
                		n[j]=n[j+1];
                		n[j+1]=t;
            		}

        		}

    		}
		} // bubble

        void merge(int h, int m, int t){
            info temp;
            int n1 = m - h + 1;
            int n2 = t - (m+1)+1;
            vector<info> a;
            vector<info> b;

            for (int i =0; i< n1; i++){
                a.push_back(temp);
                a[i] = n[h + i];
            }
            for (int i =0; i< n2; i++){
                b.push_back(temp);
                b[i] = n[m + 1 + i];
            }

            int i=0;
            int j=0;
            int k=h;
            while (i < n1 && j < n2) {
                if (a[i].gNum >= b[j].gNum) {
                    n[k] = a[i];
                    i++;
                }

                else {
                    n[k] = b[j];
                    j++;
                }

                k++;
            }

            while (i < n1) {
                n[k] = a[i];
                i++;
                k++;
            }

            while (j < n2) {
                n[k] = b[j];
                j++;
                k++;
            }
        }

        void mergesort(int h, int t) {
            if (h < t) {
                int mid = (h+t) / 2;

                mergesort(h, mid);//0�줤��
                mergesort( mid+1 , t);//����+1�����
                merge(h, mid, t);
            }
        }

		void reverse(){
			int last = size() -1;
			int first = 0;
			while( first < last){
				swap(n[first],n[last]);
				first++;
				last--;
			}
		}
		void swap(info &a, info &b){
    		info t = a;
    		a = b;
    		b = t;
		}
		int pivot(int front, int end){
    		int base = n[end].gNum;
    		int i = front -1; // ���i��j�a���� base�p
    		for (int j = front; j < end; j++) {
        		if (n[j].gNum > base) { //�@���o�{���H��base�j
            		i++;
            		swap(n[i], n[j]);//��L���e���h
       	 		}
    		}
    		i++;
    		swap(n[i], n[end]); // ��base���b��L�j���L�p������
    		return i;
		}
		void quick(int front, int end){
    		if (front < end) {
        		int p = pivot(front, end);
        		quick(front, p - 1); // ���b��]��
        		quick(p + 1, end); // �k�b��]��
    		}
		}

		void pre_radix(int place,bool last) { // ���N�O�O�ƱƦC�A���Ӧ�Q��ʦ�
			info output[size()];
  			int count[10]; // 0-9
  			for (int i = 0; i < 10; ++i) // ��l��
    			count[i] = 0;
			for (int i = 0; i < size(); i++)  // �l�Ƥ@�˭p�ƾ��[�[
    			count[(n[i].gNum / place) % 10]++;
			for (int i = 1; i < 10; i++) // ��Ҧ���m���`�M�[�_��
		    	count[i] += count[i - 1];
			for (int i = size()-1; i >= 0; i--) {
   				output[count[(n[i].gNum / place) % 10] - 1] = n[i];
   				count[(n[i].gNum / place) % 10]--;
  			}
  			if ( last ) {
				for (int i = 0; i < size(); i++)
   					n[i] = output[size()-1-i];

   			}
   			else{
   				for (int i = 0; i < size(); i++)
   					n[i] = output[i];
			}

		}





		void radix() {
  			int max = n[0].gNum;
 			for(int i = 0; i < size(); i++){
				if( n[i].gNum > max)
					max = n[i].gNum;
			}

  			for (int place = 1; max / place > 0; place *= 10) { //�̤j���ƬO�X��Ʒ�d�򭭨�
  			    if ( place*10 <= max)
            		pre_radix(place,false);
            	else
            		pre_radix(place,true);
			}

		}
};

// ���W�r�j�x
void getNameforSS_BS(string &realname, string &newname1, string &newname2){  // ��ܸ��w
 string inputname;
 realname="input000";
 int i = 0;
 cout << "input 501,502...[0]Quit:";
 cin >> inputname;
 if (inputname=="0") {
        realname="0";
        return;
 }
 if ( inputname.length() == realname.length()) {
   realname = inputname+".txt";
   newname1 = "select_sort"+ realname.substr(realname.length()-7,realname.length());
   newname2 = "bubble_sort"+ realname.substr(realname.length()-7,realname.length());
    }
 else if  ( inputname.length() < realname.length() && inputname.length() == 3){
   realname = "input"+inputname+".txt";
   newname1 = "select_sort"+ realname.substr(realname.length()-7,realname.length());
   newname2 = "bubble_sort"+ realname.substr(realname.length()-7,realname.length());
    }
 else{
   cout <<"NOT RIGHT!,TRY AGAIN"<< endl;
      getNameforSS_BS(realname,newname1,newname2);
    }


}


void getNameforMS_QS(string &realname, string &newname1, string &newname2){  // �X�ָ�ֳt
 string inputname;
 realname="input000";
 int i = 0;
 cout << "input 501,502...[0]Quit:";
 cin >> inputname;
 if (inputname=="0") {
        realname="0";
        return;
 }
 if ( inputname.length() == realname.length()) {
   realname = inputname+".txt";
   newname1 = "merge_sort"+ realname.substr(realname.length()-7,realname.length());
   newname2 = "quick_sort"+ realname.substr(realname.length()-7,realname.length());
    }
 else if  ( inputname.length() < realname.length() && inputname.length() == 3){
   realname = "input"+inputname+".txt";
   newname1 = "merge_sort"+ realname.substr(realname.length()-7,realname.length());
   newname2 = "quick_sort"+ realname.substr(realname.length()-7,realname.length());
    }
 else{
   cout <<"NOT RIGHT!,TRY AGAIN"<< endl;
      getNameforMS_QS(realname,newname1,newname2);
    }


}


void getNameforRS(string &realname, string &newname){  // ���
 string inputname;
 realname="input000";
 newname;
 int i = 0;
 cout << "input 501,502...[0]Quit:";
 cin >> inputname;
 if (inputname=="0") {
        realname="0";
        return;
 }
 if ( inputname.length() == realname.length()) {
   realname = inputname+".txt";
   newname = "radix_sort"+ realname.substr(realname.length()-7,realname.length());
    }
 else if  ( inputname.length() < realname.length() && inputname.length() == 3){
   realname = "input"+inputname+".txt";
   newname = "radix_sort"+ realname.substr(realname.length()-7,realname.length());
    }
 else{
   cout <<"NOT RIGHT!,TRY AGAIN"<< endl;
      getNameforRS(realname,newname);
    }


}

void getNameforcase4(string &realname,string &stname,string &ogname){  // ���
 string inputname;
 realname="input000";
 ogname;
 int i = 0;
 cout << "input 501,502...[0]Quit:";
 cin >> inputname;
 if (inputname=="0") {
        realname="0";
        return;
 }
 

 if ( inputname.length() == realname.length()) {
	realname = inputname+".txt";
   	ogname = inputname.substr(inputname.length()-3,inputname.length());
   	stname = "sort_time.txt";
    }
 else if  ( inputname.length() < realname.length() && inputname.length() == 3){
 	realname = "input"+inputname+".txt";
    ogname = inputname;
    stname = "sort_time.txt";
    }
 else{
   cout <<"NOT RIGHT!,TRY AGAIN"<< endl;
      getNameforcase4(realname,stname,ogname);
    }


}


void split(string s, vector<string>& buf){ // �����ɮ�
 int current = 0; //�̪�� 0 ����m�}�l��
 int next;
 while (1)
 {
  next = s.find_first_of("\t", current);
  if (next != current)
  {
   string tmp = s.substr(current, next - current);
   if (tmp.size() != 0) //�����Ŧr��
    buf.push_back(tmp);
  }
  if (next == string::npos) break;
  current = next + 1; //�U���� next + 1 ����m�}�l��_�C
 }
}

void case1(){
 string line;
 string dontneed;
 string realname;
 string newname1;
 string newname2;

 int record=0;
 getNameforSS_BS(realname,newname1,newname2);

 if(realname=="0") return;
 // cout << realname << newname;
    ifstream myFile;
    myFile.open(realname.c_str());
    ofstream newFile1;
    ofstream newFile2;
    if(!myFile.is_open()){
        cout << "File does not exist!";
        return;
    }

    newFile1.open(newname1.c_str());
    newFile2.open(newname2.c_str());
    string s ;
    vector<string> buf;
    vector<info> og;
    Sorts note;

    for (int i =0;i<3;i++){
        getline(myFile, dontneed);
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
            og.push_back(temp);
        } // if
    }

    note.n = og;
    int sst = 0;
    int bst = 0;
    sst=clock();
    note.select();
    sst=clock() - sst;

    for(int t = 0; t < note.size() ;t++) {
        newFile1 << note.n[t].sNum << "\t" << note.n[t].sName << "\t" << note.n[t].dCode << "\t" << note.n[t].dName << "\t" << note.n[t].day << "\t" << note.n[t].rate << "\t";
        newFile1 << note.n[t].stuNum << "\t" << note.n[t].tNum << "\t" << note.n[t].gNum << "\t" << note.n[t].pName << "\t" << note.n[t].s << endl;
    }



    note.clear();
    note.n = og;
    bst=clock();
    note.bubble();
    bst=clock() - bst;

    for(int t = 0; t < note.size() ;t++) {
        newFile2 << note.n[t].sNum << "\t" << note.n[t].sName << "\t" << note.n[t].dCode << "\t" << note.n[t].dName << "\t" << note.n[t].day << "\t" << note.n[t].rate << "\t";
        newFile2 << note.n[t].stuNum << "\t" << note.n[t].tNum << "\t" << note.n[t].gNum << "\t" << note.n[t].pName << "\t" << note.n[t].s << endl;
    }




    cout << "Selection sort: " << sst << "ms\n\n";
    cout << "Bubble sort: " << bst << "ms\n";

    myFile.close();
    newFile1.close();
    newFile2.close();

}

void case2(){
 string line;
 string dontneed;
 string realname;
 string newname1;
 string newname2;
 getNameforMS_QS(realname,newname1,newname2);

 if(realname=="0") return;
 // cout << realname << newname;
    ifstream myFile;
    myFile.open(realname.c_str());
    ofstream newFile1;
    ofstream newFile2;
    if(!myFile.is_open()){
        cout << "File does not exist!";
        return;
    }

    newFile1.open(newname1.c_str());
    newFile2.open(newname2.c_str());
    string s ;
    vector<string> buf;
    vector<info> og;
    Sorts note;

    for (int i =0;i<3;i++){
        getline(myFile, dontneed);
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
            og.push_back(temp);
        } // if
    }

    note.n = og;
    int mst = 0;
    int qst = 0;
    mst=clock();
    note.mergesort(0,note.size()-1);
    mst=clock() - mst;
    for(int t = 0; t < note.size() ;t++) {
        newFile1 << note.n[t].sNum << "\t" << note.n[t].sName << "\t" << note.n[t].dCode << "\t" << note.n[t].dName << "\t" << note.n[t].day << "\t" << note.n[t].rate << "\t";
        newFile1 << note.n[t].stuNum << "\t" << note.n[t].tNum << "\t" << note.n[t].gNum << "\t" << note.n[t].pName << "\t" << note.n[t].s << endl;
    }



    note.clear();
    note.n = og;
    qst=clock();
    note.quick(0,note.size()-1);
    qst=clock() - qst;

    for(int t = 0; t < note.size() ;t++) {
        newFile2 << note.n[t].sNum << "\t" << note.n[t].sName << "\t" << note.n[t].dCode << "\t" << note.n[t].dName << "\t" << note.n[t].day << "\t" << note.n[t].rate << "\t";
        newFile2 << note.n[t].stuNum << "\t" << note.n[t].tNum << "\t" << note.n[t].gNum << "\t" << note.n[t].pName << "\t" << note.n[t].s << endl;
    }




    cout << "Merge sort: " << mst << "ms\n\n";
    cout << "Quick sort: " << qst << "ms\n";

    myFile.close();
    newFile1.close();
    newFile2.close();

}

void case3(){
 string line;
 string dontneed;
 string realname;
 string newname;

 getNameforRS(realname,newname);

 if(realname=="0") return;
 // cout << realname << newname;
    ifstream myFile;
    myFile.open(realname.c_str());
    ofstream newFile;
    if(!myFile.is_open()){
        cout << "File does not exist!";
        return;
    }

    newFile.open(newname.c_str());
    string s ;
    vector<string> buf;
    vector<info> og;
    Sorts note;

    for (int i =0;i<3;i++){
        getline(myFile, dontneed);
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
            og.push_back(temp);
        } // if
    }

    note.n = og;
    int rst = 0;
    rst=clock();
    note.radix();
    rst=clock() - rst;

    for(int t = 0; t < note.size() ;t++) {
        newFile << note.n[t].sNum << "\t" << note.n[t].sName << "\t" << note.n[t].dCode << "\t" << note.n[t].dName << "\t" << note.n[t].day << "\t" << note.n[t].rate << "\t";
        newFile << note.n[t].stuNum << "\t" << note.n[t].tNum << "\t" << note.n[t].gNum << "\t" << note.n[t].pName << "\t" << note.n[t].s << endl;
    }


    cout << "Radix sort: " << rst << "ms\n";

    myFile.close();
    newFile.close();
}


void case4(){
 string line;
 string dontneed;
 string realname;
 string ogname;
 string stname;
 int num;
 bool exist = true;
 
 getNameforcase4(realname, stname, ogname);

 if(realname=="0") return;
 ifstream myFile1;
 myFile1.open(realname.c_str());
 
 if(!myFile1.is_open()){
    cout << "File does not exist!";
    return;
 }
 fstream myFile2;
 ifstream f(stname.c_str());
 
 if( !f.good()){
 	myFile2.open(stname.c_str(), ios::app);
    myFile2 << "\t��ܱƧ�\t��w�Ƨ�\t�X�ֱƦC\t�ֳt�ƦC\t��ƱƦC\n";
 }
 else
   myFile2.open(stname.c_str(), ios::app);
 

    string s ;
    vector<string> buf;
    vector<info> og;
    Sorts note;

    for (int i =0;i<3;i++){
        getline(myFile1, dontneed);
    }
    while(getline(myFile1, s)){
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
            og.push_back(temp);
        } // if
    }
	s ="";
    note.n = og;
    int rst = 0;
    int mst = 0;
    int qst = 0;
    int sst = 0;
    int bst = 0;
    stringstream ss;
    sst=clock();
    note.select();
    sst=clock() - sst;
    ss << sst;
    s = s+ogname+"\t"+ss.str()+"\t";
    note.clear(); //�M�� 
    note.n = og;
    bst=clock();
    note.bubble();
    bst=clock() - bst;
    ss << bst;
    s = s + ss.str()+"\t";
    note.clear(); // �M�� 
    note.n = og;
    mst=clock();
    note.mergesort(0,note.size()-1);
    mst=clock() - mst;
    ss << mst;
    s = s + ss.str()+"\t";
    note.clear(); // �M�� 
    note.n = og;
    qst=clock();
    note.quick(0,note.size()-1);
    qst=clock() - qst;
    ss << qst;
    s = s+ ss.str()+"\t";
    note.clear(); // �M�� 
    note.n = og;
    rst=clock();
    note.radix();
    rst=clock() - rst;

    myFile2 << ogname << "\t"<< sst << "\t" << bst << "\t" << mst << "\t" << qst << "\t" << rst << "\n";
    
    myFile1.close();
    myFile2.close();
    
    
}
int main(){
	int command = 0; // user command
 	do{
		cout << endl << "*** File Object Manipulator ***";
 		cout << endl << "* 0. QUIT *";
 		cout << endl << "* 1. Selection sort vs Bubble sort*";
 		cout << endl << "* 2. Merge sort vs. Quick sort *";
 		cout << endl << "* 3. Radix sort *";
 		cout << endl << "* 4. Comparison on five methods *";
 		cout << endl << "*********************************";
 		cout << endl << "Input a choice(0, 1, 2, 3, 4): ";
 		cin >> command; // get a command
 		switch (command){
			case 0: break;
 			case 1:
 				case1();
 				break;
 			case 2:
 				case2();
 				break;
 			case 3:
 				case3();
 				break;
            case 4:
            	case4();
            	c4num++;
            	break;
 			default:
				cout << endl << "Command does not exist!" << endl;
 		} // end switch
 	} while (command != 0); // '0': stop the program
 	system("pause"); // pause the display
 	return 0;
} // end main
