#include <iostream> // cin, cout, endl
#include <iomanip> // setw
#include <new> // new, delete
#include <string> // c_str, length
#include <cstdlib> // strtoul, system
#include <ctime> // clock_t, clock, CLOCKS_PER_SEC
using namespace std;
int case1_num = 1; // ��case1�p�� 
int case2_num = 1; // ��case2�p�� 
int case3_num = 1; // ��case3�p�� 
int cT = 0;
void F_forC1(int num[], int now, int total) {  // now:�{�b����,total:�`�@�n�h��  
    int i, j, temp; // �ܼƤj�x 

    if(now < total) { // �٤���ҨD 
        for(i = now; i <= total; i++) { 
            temp = num[i]; // �ȩ� 
            for(j = i; j > now; j--) 
                num[j] = num[j-1]; 
                
            num[now] = temp; 

            F_forC1(num, now+1, total); // ���j�~��� 

            
            for(j = now; j < i; j++) 
                num[j] = num[j+1]; 
                
            num[i] = temp; // �٦^�h 
        } 
    } 
    else {  // ���׵���ݨD�N�L�X�� 
        cout << "["<< case1_num <<"]"; 
        case1_num++;
        for(i = 1; i <= total; i++){
            cout <<" "<< num[i];

        }
        cout << endl;
    } 
}
void F_forC2(int num[], int now, int total) {  // now:�{�b����,total:�`�@�n�h��  
    int i, j, temp; // �ܼƤj�x 

    if(now < total) { // �٤���ҨD 
        for(i = now; i <= total; i++) { 
            temp = num[i]; // �ȩ� 
            for(j = i; j > now; j--) 
                num[j] = num[j-1]; 
                
            num[now] = temp; 

            F_forC2(num, now+1, total); // ���j�~��� 

            
            for(j = now; j < i; j++) 
                num[j] = num[j+1]; 
                
            num[i] = temp; // �٦^�h 
        } 
    } 
    else {  // ���׵���ݨD�N�L�X�� 
        cout << "["<< case2_num <<"]"; 
        case2_num++;
        for(i = 1; i <= total; i++){
            cout <<" "<< num[i];

        }
        cout << endl;
    } 
}
void F_forC3(string list , string answer, int n)
{
    if(answer.length() == n) // ���ת��׵���ҨD 
    {
        cout<<"["<<case3_num<<"]";
        case3_num++;
        cout<<answer<<endl;
        return;
    }
    for(int i=0 ; i<list.length() ; i++)
    {
        char ch = list[i];
        string a = list.substr(0,i);
        string b = list.substr(i+1);
        string temp = a + b;
        
        F_forC3(temp , answer+ch,n);
    }
  
}
int getNO(){ //���b�@ 
	cout <<"Input num:";
	int n = 0;
	cin >> n;
	while( n<1|| n>9 ){
		cout << "###It is not in[1,9]###" << endl;
		cout <<"Input num:";
		cin >> n;
	}
	return n;
}
int getNum(){ //���b�G 
	cout <<"Input num:";
	int m=0;
	cin >> m;
	while( m<2|| m>9 ){
		cout << "###It is not in[2,9]###" << endl;
		cout <<"Input num:";
		cin >> m;
	}
	return m;
}

int getforCase2(int list[],int m){ //���b�T 
	bool exist = false;
	cout <<"Input num:";
	int n=0;
	cin >> n;
	for(int i = 0;i<=m;i++){
		if (list[i] == n)
		  exist = true;
	}
	
	if (exist){
		cout <<"###Exist, still need number!###"<<endl;
		getforCase2(list,m);
	}
	else 
	  return n;
	
	  
}
void case1(){
	int n = getNO();
    case1_num =1; // �^�_���@ 
    int list1[n+1]={0}; //��l�� 
    
    for(int i = 1; i <= n; i++) 
        list1[i] = i; 

    F_forC1(list1, 1, n);
    cout<< "Level:" << n; 
}
void case2(){
	int m = getNum();
	case2_num = 1;// �^�_���@ 
	int list2[m+1]={0}; //��l�� 
	int n = 0;
	for(int i = 1; i <= m; i++) {
	  n=getforCase2(list2,m); 
	  list2[i] = n; 
	}
	
	F_forC2(list2, 1, m);
}
void case3(){
   string list="123456789";
   string answer="";
   case3_num = 1;// �^�_���@ 
   int n=getNO();
   F_forC3(list , answer,n);
} 

  
int main()
{ int command = 0; // user command
 do{ 
 cout << endl << "** Permutation Generator **";
 cout << endl << "* 0. Quit *";
 cout << endl << "* 1. N numbers from 1..N *";
 cout << endl << "* 2. M numbers from input *";
 cout << endl << "* 3. M numbers from 1..9 *";
 cout << endl << "***************************";
 cout << endl << "Input a choice(0, 1, 2, 3): ";
 cin >> command; // get a command
 switch (command)
 { case 0: break;
 case 1: 
 case1();
 break;
 case 2:  // get the permutation length
 cT = clock(); // start timer
 case2();
 cT = clock() - cT; // stop timer
 cout << "T = " << (float)cT * 1000 / CLOCKS_PER_SEC << " ms" << endl;
 break;
 case 3: 
 cT = clock(); // start timer
 case3();
 cT = clock() - cT; // stop timer
 cout << "T = " << (float)cT * 1000 / CLOCKS_PER_SEC << " ms" << endl;
 break;
 default: cout << endl << "Command does not exist!" << endl;
 } // end switch
 } while (command != 0); // '0': stop the program
 system("pause"); // pause the display
 return 0;
} // main
