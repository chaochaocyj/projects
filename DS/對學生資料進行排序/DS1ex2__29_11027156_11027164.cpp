#include <iostream> // cout, endl
#include <fstream> // open, is_open, close, ignore
#include <string> // string, find_last_of, substr
#include <vector> // vector, push_back
#include <cstdlib> // system, atoi
#include <iomanip> // setw
#include <bits/stdc++.h>
#include <stdlib.h>

using namespace std;

class info{

    public:
        string sNum; // 校代
        string sName; // 校名
        string dCode; // 科代
        string dName; // 科名
        string day; // 日or進
        string rate; // 等級
        string stuNum; // 學生數
        string tNum; // 老師數
        string gNum; // 畢業數
        string pName; // 地點
        string s; // 體系
};
bool cmp(string a,string b){ // 比大小
  if ( a.length() == b.length()){
   if( a[0] > b[0] ){
     return true;
 }
 else if ( a[0] == b[0]){
   if ( a[1] > b[1] && a.length()>=2){
    return true;
   }
   else if ( a[1] == b[1]){
    if ( a[2] > b[2] && a.length()>=3){
      return true;
  }
  else if ( a[2] == b[2] ){
   if ( a[3] > b[3] && a.length()>=4){
     return true;
   }
   else if ( a[3] == b[3] ){
    if (a[4] > b[4] && a.length()>=5){
      return true;
    }
   }
  }
   }
 }
  }


  else if ( a.length() > b.length())
    return true;
  return false;
}
void getNameforCase1(string &realname, string &newname){
 string inputname;
 realname="input000";
 newname;
 int i = 0;
 cout << "input 201,202...[0]Quit:";
 cin >> inputname;
 if (inputname=="0") {
        realname="0";
        return;
 }
 if ( inputname.length() == realname.length()) {
   realname = inputname+".txt";
   newname = "copy"+ realname.substr(realname.length()-7,realname.length());
    }
 else if  ( inputname.length() < realname.length() && inputname.length() == 3){
   realname = "input"+inputname+".txt";
   newname = "copy"+ realname.substr(realname.length()-7,realname.length());
    }
 else{
   cout <<"NOT RIGHT!,TRY AGAIN"<< endl;
      getNameforCase1(realname,newname);
    }


}
void getNameforCase2(string &realname){
 string inputname;
 realname="copy000";
 int i = 0;
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
   realname = "copy"+inputname+".txt";
    }
 else{
   cout <<"NOT RIGHT!,TRY AGAIN"<< endl;
      getNameforCase2(realname);
    }
}

void getNameforCase3(string &realname){
 string inputname;
 realname="copy000";
 int i = 0;
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
   realname = "copy"+inputname+".txt";
    }
 else{
   cout <<"NOT RIGHT!,TRY AGAIN"<< endl;
      getNameforCase3(realname);
    }
}



void Compare( vector<info> &note, vector<info> &temp){
 string stunum ;
 string gnum ;
 int j = 0;
    cout << "Threshold on number of students: ";
    cin >> stunum;
 while (stunum[0] == '-' ) {
   cout << "please enter a valid number!";
   cin >> stunum;
 }
 cout << "Threshold number of graduates:";
 cin >> gnum;
 while (gnum[0] == '-' ) {
   cout << "please enter a valid number!:";
   cin >> gnum;
 }
    for (int i = 0 ; i < note.size() ; i++){

       if ( cmp(note[i].stuNum,stunum) && cmp(note[i].gNum, gnum)) {
      temp.push_back(info());
      temp[j] = note[i];
      j++;
       }


 }


}


void split(string s, vector<string>& buf)
{
 int current = 0; //最初由 0 的位置開始找
 int next;
 while (1)
 {
  next = s.find_first_of("\t", current);
  if (next != current)
  {
   string tmp = s.substr(current, next - current);
   if (tmp.size() != 0) //忽略空字串
    buf.push_back(tmp);
  }
  if (next == string::npos) break;
  current = next + 1; //下次由 next + 1 的位置開始找起。
 }
}



void case1(){
 string line;
 string dontneed;
 string realname;
 string newname;
 int record=0;
 getNameforCase1(realname,newname);
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
    for (int i =0;i<3;i++){
     getline(myFile, dontneed);
 }
    // Print file content
    while (getline(myFile, line)) {
        newFile << line << endl;
        record++;
    }

    cout << "Total number of records: " << record << endl;
    // Close file
    myFile.close();
    newFile.close();
    // cout << "End!";
}

void case2(){
 vector<info> note;
 vector<info> temp;
 int record=0;
 //ifstream myFile;
 string realname;
 getNameforCase2(realname);
 if(realname=="0") return;
 //myFile.open(realname.c_str());
 ifstream myFile;
 myFile.open(realname.c_str());
 if(!myFile.is_open()){
        cout << "File does not exist!";
        return;
 }

 string s ;
 vector<string> buf;
 while(getline(myFile, s)){
  split(s, buf);
 }
 int j = 0;
 note.push_back(info());
 for(int i = 0; i < buf.size() ;i++ ){
  if (i%11 == 0)
       note[j].sNum = buf[i];
     if (i%11 == 1)
       note[j].sName = buf[i];
  if (i%11 == 2)
       note[j].dCode = buf[i];
  if (i%11 == 3)
       note[j].dName = buf[i];
  if (i%11 == 4)
       note[j].day = buf[i];
  if (i%11 == 5)
       note[j].rate = buf[i];
  if (i%11 == 6)
       note[j].stuNum = buf[i];
  if (i%11 == 7)
       note[j].tNum = buf[i];
  if (i%11 == 8)
       note[j].gNum = buf[i];
  if (i%11 == 9)
       note[j].pName = buf[i];
  if ( i%11 == 10 ) { // 滿了
   note[j].s = buf[i];
   note.push_back(info());
   j++;
     } // if
 }

 Compare(note, temp);
 remove(realname.c_str());
 ofstream newFile;
 newFile.open(realname.c_str());


 for(int t = 0; t < temp.size() ;t++) {
  newFile << temp[t].sNum << "\t" <<temp[t].sName << "\t" << temp[t].dCode << "\t" << temp[t].dName << "\t" << temp[t].day << "\t" << temp[t].rate << "\t";
        newFile << temp[t].stuNum << "\t" << temp[t].tNum << "\t" << temp[t].gNum << "\t" << temp[t].pName << "\t" << temp[t].s << endl;
        record++;
 }

 cout << "Total number of records: " << record << endl;
 myFile.close();
    newFile.close();
}

void case3(){
 string f1,f2;
 string newname;
 int record=0;
 //cout << "Input first file:";
 getNameforCase3(f1);

 //cout << "Input second file:";
 getNameforCase3(f2);


 newname="output";
 for(int i =4;i<7;i++){
  newname.push_back(f1[i]);
 }
 newname+="_";
 for(int i =4;i<7;i++){
  newname.push_back(f2[i]);
 }
 newname+=".txt";


 ifstream file1;
 file1.open(f1.c_str());
 if(!file1.is_open()){
        cout << "File does not exist!";
        return;
    }



 ifstream file2;
file2.open(f2.c_str());
  if(!file2.is_open()){
        cout << "File does not exist!";
        return;
    }


ofstream newFile(newname.c_str(),ios::out);
 vector<info> note1;
  string s ;
  string s2;
 vector<string> buf;

  while(getline(file1, s)){
   split(s, buf);

  }

 while(getline(file2, s2)){
   split(s2, buf);

  }

 int j = 0;
  note1.push_back(info());
  for(int i = 0; i < buf.size() ;i++ ){
   if (i%11 == 0)
        note1[j].sNum = buf[i];
     if (i%11 == 1)
        note1[j].sName = buf[i];
    if (i%11 == 2)
        note1[j].dCode = buf[i];
   if (i%11 == 3)
        note1[j].dName = buf[i];
   if (i%11 == 4)
        note1[j].day = buf[i];
   if (i%11 == 5)
        note1[j].rate = buf[i];
   if (i%11 == 6)
        note1[j].stuNum = buf[i];
   if (i%11 == 7)
        note1[j].tNum = buf[i];
  if (i%11 == 8)
        note1[j].gNum = buf[i];
   if (i%11 == 9)
        note1[j].pName = buf[i];
   if ( i%11 == 10 ) { // 皛蹂?
    note1[j].s = buf[i];
    note1.push_back(info());
    j++;
    } // if
 }

/*
        string sNum; // 學校代碼
        string sName; // 學校名稱
        string dCode; // 科系代碼
        string dName; // 科系名稱
        string day; // 日間∕進修別
        string rate; // 等級別
        string stuNum; // 學生數
        string tNum; // 教師數
        string gNum; // 上學年度畢業生數
        string pName; // 縣市名稱
        string s; // 體系別
        */

 sort(note1.begin(),note1.end(),[](info first,info second ){
  return first.sNum<second.sNum ;
   });




    for(int t = 0;t<note1.size();t++){
        newFile.open(newname.c_str(),ofstream::out | ofstream::app|ofstream::ate);
        newFile << note1[t].sNum << "\t" << note1[t].sName<< note1[t].dCode << "\t" << note1[t].dName << "\t" << note1[t].day << "\t" << note1[t].rate << "\t";
        newFile << note1[t].stuNum << "\t" << note1[t].tNum << "\t" << note1[t].gNum << "\t" << note1[t].pName << "\t" << note1[t].s << endl;
        newFile.close();
        record++;

 }

   cout << "Total number of records: " << record-1 << endl;
}


void case4(){
 string f1,f2,f3;
 string newname;
 int record=0;
 //cout << "Input first file:";
 getNameforCase3(f1);
if(f1=="0") return;
 //cout << "Input second file:";
 getNameforCase3(f2);
if(f2=="0") return;
 getNameforCase3(f3);
if(f3=="0") return;

 newname="output";
 for(int i =4;i<7;i++){
  newname.push_back(f1[i]);
 }
 newname+="_";
 for(int i =4;i<7;i++){
  newname.push_back(f2[i]);
 }

  newname+="_";
 for(int i =4;i<7;i++){
  newname.push_back(f3[i]);
 }

 newname+=".txt";



 ifstream file1;
 file1.open(f1.c_str());
 if(!file1.good()){
        cout << "File does not exist!";
        return;
    }



 ifstream file2;
file2.open(f2.c_str());
  if(!file2.good()){
        cout << "File does not exist!";
        return;
    }

 ifstream file3;
file3.open(f3.c_str());
  if(!file3.good()){
        cout << "File does not exist!";
        return;
    }


ofstream newFile(newname.c_str(),ios::out);
 vector<info> note1;
  string s ;
  string s2;
  string s3;
 vector<string> buf;

  while(getline(file1, s)){
   split(s, buf);

  }

 while(getline(file2, s2)){
   split(s2, buf);

  }

  while(getline(file3, s3)){
   split(s3, buf);

  }

 int j = 0;
  note1.push_back(info());
  for(int i = 0; i < buf.size() ;i++ ){
   if (i%11 == 0)
        note1[j].sNum = buf[i];
     if (i%11 == 1)
        note1[j].sName = buf[i];
    if (i%11 == 2)
        note1[j].dCode = buf[i];
   if (i%11 == 3)
        note1[j].dName = buf[i];
   if (i%11 == 4)
        note1[j].day = buf[i];
   if (i%11 == 5)
        note1[j].rate = buf[i];
   if (i%11 == 6)
        note1[j].stuNum = buf[i];
   if (i%11 == 7)
        note1[j].tNum = buf[i];
  if (i%11 == 8)
        note1[j].gNum = buf[i];
   if (i%11 == 9)
        note1[j].pName = buf[i];
   if ( i%11 == 10 ) { // 皛蹂?
    note1[j].s = buf[i];
    note1.push_back(info());
    j++;
    } // if
 }


 sort(note1.begin(),note1.end(),[](info first,info second ){
  return first.sNum<second.sNum ;
   });




    for(int t = 0;t<note1.size();t++){
        newFile.open(newname.c_str(),ofstream::out | ofstream::app|ofstream::ate);
        newFile << note1[t].sNum << "\t" << note1[t].sName<< note1[t].dCode << "\t" << note1[t].dName << "\t" << note1[t].day << "\t" << note1[t].rate << "\t";
        newFile << note1[t].stuNum << "\t" << note1[t].tNum << "\t" << note1[t].gNum << "\t" << note1[t].pName << "\t" << note1[t].s << endl;
        newFile.close();
        record++;

 }

   cout << "Total number of records: " << record-1 << endl;
}


int main()
{ int command = 0; // user command
 do
 {
cout << endl << "*** File Object Manipulator ***";
 cout << endl << "* 0. QUIT *";
 cout << endl << "* 1. COPY (Read & Save a file) *";
 cout << endl << "* 2. FILTER (Reduce a file) *";
 cout << endl << "* 3. MERGE (Join two files) *";
 cout << endl << "*********************************";
 cout << endl << "Input a choice(0, 1, 2, 3, 4): ";
 cin >> command; // get a command
 switch (command)
 { case 0: break;
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
 break;
 default: cout << endl << "Command does not exist!" << endl;
 } // end switch
 } while (command != 0); // '0': stop the program
 system("pause"); // pause the display
 return 0;
} // end main
