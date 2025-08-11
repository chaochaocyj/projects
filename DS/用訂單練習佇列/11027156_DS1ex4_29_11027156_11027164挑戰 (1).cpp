// 11027156 林芷安 11027164 趙怡儒
#include <iostream>
#include <fstream> // open, is_open, close, ignore
#include <string> // string, find_last_of, substr
#include <vector> // vector, push_back
#include <cstdlib> // system, atoi
#include <iomanip> // setw
#include <cstring>
#include <ctime> // clock_t, clock, CLOCKS_PER_SEC
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace std;

struct list{
    string orderid; // 訂單編號
    int arrival; // 抵達時間
    int duration; // 處理時間
    int timeout; // 限制時間
};

struct a{
    string oid; // 訂單編號
    string cid; // 廚師ID
    int delay; // 逾時
    int abort; // 放棄時間
};

struct t{
    string oid; // 訂單編號
    string cid;
    int delay; // 逾時
    int depart; // 完成
};

vector<a> abt;
vector<t> tout;

class cook{
	public:
		vector<list> queue;
		int time = 0;
		bool isFull(){
			if ( queue.size() >= 3){ // 滿了不能放了
			  return true;
			} // if
			return false;
		} // isFull()
		int size(){
			return queue.size();
		}
		void pop(){
			queue.erase(queue.begin()); // 把第一個拔掉
		} // pop()

		void add(list item){
			queue.push_back(item);
		}

		void putintoabt1(string s){ // 拿出來的時候發現超時:(
			a a;
            a.oid = queue[0].orderid;
            a.cid = s;
            a.delay = time - queue[0].arrival;
            a.abort = time;
            abt.push_back(a);
		}

		void putintoabt2(list item,string s){ // 滿了不要硬塞放棄ㄅ
			a a;
            a.oid = item.orderid;
            a.cid = s;
            a.delay = 0;
            a.abort = item.arrival;
            abt.push_back(a);
		}

		void putintotout(string s){ // 做完才發先超時遺憾
		    t t;
            t.oid = queue[0].orderid;
            t.cid = s;
            t.delay = time - (queue[0].arrival + queue[0].duration);
            t.depart = time;
            tout.push_back(t);
		}
		void oneCook(list item){ // case2
		    while ( time <= item.arrival && queue.size() > 0 ) {  // 佇列有東西
            	if ( time >= queue[0].timeout){ // 喔耨,還沒做就超時了
					putintoabt1("nothing");
            	}

            	else{ // 做完有兩種
              		time = time + queue[0].duration;
              		if ( time > queue[0].timeout){
						putintotout("nothing");
              		}
            	}

            	pop();

          	} // while

       		if ( isFull() ) { // 滿了:(
              putintoabt2(item,"nothing");
        	} // if


        	else{ // 他很閒，馬上移動到下一筆訂單
          		if( time <= item.arrival )
            		time = item.arrival;
          		add(item);
        	} // else
		} // oneCook()

		void twocook( list item, string s){
			while ( time <= item.arrival && queue.size() > 0 ) {  // 佇列有東西
            	if ( time >= queue[0].timeout){ // 喔耨,還沒做就超時了
					putintoabt1(s);
            	}

            	else{ // 做完有兩種
              		time = time + queue[0].duration;
              		if ( time > queue[0].timeout){
						putintotout(s);
              		}
            	}


            	pop();

          	} // while


          if( time <= item.arrival )
              time = item.arrival;

		}


};

int Adddelay(vector<a> a_list, vector<t> t_list){
 int a_delay = 0;
 int t_delay = 0;
 int i = 0;
 for( i = 0; i < a_list.size(); i++ ){
    a_delay =  a_delay + a_list[i].delay;
 }
 for( i = 0; i < t_list.size(); i++ ){
    t_delay =  t_delay + t_list[i].delay;
 }

 return a_delay + t_delay;
}


float Failure(vector<list> list,vector<a> a_list, vector<t> t_list){
 float i  = (float((a_list.size())+(t_list.size()))/float(list.size())*100.0);
 return  i;

}

void getNameforCase1(string &realname, string &newname){
 string inputname;
 realname="input000";
 newname;
 int i = 0;
 cout << "input 401,402...[0]Quit:";
 cin >> inputname;
 if (inputname=="0") {
        realname="0";
        return;
 }
 if ( inputname.length() == realname.length()) {
   realname = inputname+".txt";
   newname = "sort"+ realname.substr(realname.length()-7,realname.length());
    }
 else if  ( inputname.length() < realname.length() && inputname.length() == 3){
   realname = "input"+inputname+".txt";
   newname = "sort"+ realname.substr(realname.length()-7,realname.length());
    }
 else{
   cout <<"NOT RIGHT!,TRY AGAIN"<< endl;
      getNameforCase1(realname,newname);
    }
}

void getNameforCase2(string &realname, string &newname){
 string inputname;
 realname="sort000";
 newname;
 int i = 0;
 cout << "input 401,402...[0]Quit:";
 cin >> inputname;
 if (inputname=="0") {
        realname="0";
        return;
 }
 if ( inputname.length() == realname.length()) {
   realname = inputname+".txt";
   newname = "one"+ realname.substr(realname.length()-7,realname.length());
    }
 else if  ( inputname.length() < realname.length() && inputname.length() == 3){
   realname = "sort"+inputname+".txt";
   newname = "one"+ realname.substr(realname.length()-7,realname.length());
    }
 else{
   cout <<"NOT RIGHT!,TRY AGAIN"<< endl;
      getNameforCase2(realname,newname);
    }
}

void getNameforCase3(string &realname, string &newname){
 string inputname;
 realname="sort000";
 newname;
 int i = 0;
 cout << "input 401,402...[0]Quit:";
 cin >> inputname;
 if (inputname=="0") {
        realname="0";
        return;
 }
 if ( inputname.length() == realname.length()) {
   realname = inputname+".txt";
   newname = "two"+ realname.substr(realname.length()-7,realname.length());
    }
 else if  ( inputname.length() < realname.length() && inputname.length() == 3){
   realname = "sort"+inputname+".txt";
   newname = "two"+ realname.substr(realname.length()-7,realname.length());
    }
 else{
   cout <<"NOT RIGHT!,TRY AGAIN"<< endl;
      getNameforCase2(realname,newname);
    }
}

void getNameforCase4(string &realname, string &newname){
 string inputname;
 realname="sort000";
 newname;
 int i = 0;
 cout << "input 401,402...[0]Quit:";
 cin >> inputname;
 if (inputname=="0") {
        realname="0";
        return;
 }
 if ( inputname.length() == realname.length()) {
   realname = inputname+".txt";
   newname = "more"+ realname.substr(realname.length()-7,realname.length());
    }
 else if  ( inputname.length() < realname.length() && inputname.length() == 3){
   realname = "sort"+inputname+".txt";
   newname = "more"+ realname.substr(realname.length()-7,realname.length());
    }
 else{
   cout <<"NOT RIGHT!,TRY AGAIN"<< endl;
      getNameforCase2(realname,newname);
    }
}

void split(string s, vector<string>& buf){
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

void shellSort(vector<list> &note) {
	int temp;
	list item;
	int a;
	int b;
    for (int gapSize = note.size() / 2; gapSize > 0; gapSize /= 2) {
        for (int index = gapSize; index < note.size(); index++) {
            temp = index;
            list item = note[index];

            while (temp >= gapSize) {
            	a = note[temp - gapSize].arrival;
            	b = item.arrival;
              	if ( a > b ) {
                	note[temp] = note[temp - gapSize];
                	temp -= gapSize;
              	}
              	else if ( a == b) { // 抵達時間一樣，比訂單編號
                	a = stoi(note[temp - gapSize].orderid);
            		b = stoi(item.orderid);
            		if ( a > b ) {
                  		note[temp] = note[temp - gapSize];
                 	 	temp -= gapSize;
                	}
                	else
                  		break;

			  	}
              	else
                	break;

            }


            note[temp] = item;
        }
    }
}

void Dealwithcase3(vector<list> &note, cook no1,cook no2, a item){
  int temp =0;
  for( int i = 0; i < note.size(); i++){
  	no1.twocook(note[i],"1");
    no2.twocook(note[i],"2");
  	if ( no1.isFull() && no2.isFull() ) { // 大家都滿了
      no1.putintoabt2(note[i],"0");
    } // if
    else if ( no1.time <= note[i].arrival  ){ // 可能兩個都沒事，但no1優先
      no1.add(note[i]);
    } // if

    else if ( no2.time <= note[i].arrival ) {  // no2沒事
      no2.add(note[i]);
    } // else if


    else if ( no1.time > note[i].arrival && no2.time > note[i].arrival) { // 兩個都有事比誰比較忙
      if ( no1.size() > no2.size() ) { // no1忙
      	no2.add(note[i]);
      } // if

      else if ( no1.size() == no2.size() && !no1.isFull() ){ // 一樣忙但no1優先
        no1.add(note[i]);
      } // else

      else { // no2忙
      	no1.add(note[i]);
      }
    } // else if

      if ( i == note.size() -1 && no1.size() > 0){ // 讓佇列裡的東東跑完
      	note[i].arrival = note[i].timeout + 10000;
      	no1.twocook(note[i],"1");
	  }
	  if ( i == note.size() -1 && no2.size() > 0){ // 讓佇列裡的東東跑完
      	note[i].arrival = note[i].timeout + 10000;
      	no2.twocook(note[i],"2");
	  }
  }


}

int findsmall( vector<cook> &clist ){  // 挑戰，找最沒事的廚師
	int who_min = 0;
	int min = -1 ;

	min = clist[0].size();
	who_min = 0;

    for(int i =0 ; i < clist.size() ; i++) {
    	if( clist[i].size() < min ) {
    	  who_min  = i;
    	  min = clist[i].size();
        } // if
    }

    return who_min;

}
bool allfull( vector<cook> &clist ) {  // 挑戰，確定廚師是不是都沒空
	for(int i =0 ; i < clist.size() ; i++) {
	  if (!clist[i].isFull()) // 有不是空的
	    return false;
    }

    return true;
}

void doall( list item,vector<cook> &clist ){
	int nocook;
	for(int i =0 ; i < clist.size() ; i++) {
	  nocook = i +1;
	  stringstream ss;
	  ss << nocook;
      clist[i].twocook(item,ss.str());
    }
}

void checkbusy( list item,vector<cook> &clist ){  // 挑戰，確定要把訂單交給誰
  	for(int i =0 ; i < clist.size() ; i++) {
      if ( clist[i].time <= item.arrival ) {
        clist[i].add(item);
        return;
      }
    }

    int i = findsmall( clist );
    clist[i].add(item);
    return;


}

void runall( vector<cook> &clist ){  // 把佇列裡的東西跑完
	list item;
	int nocook;
	for(int i =0 ; i < clist.size() ; i++) {
      if ( clist[i].size() > 0 ) {
      	item.arrival = 100000000;
        nocook = i+1;
      	stringstream ss;
	    ss << nocook;
      	clist[i].twocook(item,ss.str());
	  }

    }
}

void Dealwithcase4(vector<list> &note, vector<cook> &clist){
	int min = 0;
    for( int i = 0; i < note.size(); i++){
      doall( note[i],clist);
  	  if ( allfull( clist ) ) { // 大家都滿了
        clist[0].putintoabt2(note[i],"0");
      } // if

	  else checkbusy( note[i],clist );

      if ( i == note.size() -1 ){ // 讓佇列裡的東東跑完
        runall( clist );
	  }

  }

}
void case1(){
	vector<list> note;
	string realname;
	string newname;
	getNameforCase1(realname,newname);
    ifstream myFile;
    ofstream newFile;
    int rdt = 0;
    int sdt = 0;
    int wdt = 0;
    rdt = clock();
    myFile.open(realname.c_str());
	if (!myFile.good()) {
      cout << "wrong";
      return;
    }
	rdt = clock() - rdt;
    newFile.open(newname.c_str());

	string s ;
	vector<string> buf;
	while(getline(myFile, s)){
		split(s, buf);
	}
	int j = 0;
	newFile << buf[0] << "\t" <<buf[1] << "\t" << buf[2] << "\t" << buf[3] <<  endl;
	cout <<"\t"<<buf[0] << "\t" <<buf[1] << "\t" << buf[2] << "\t" << buf[3] <<  endl;


	list temp;
	for(int i = 4; i < buf.size() ;i++ ){
		if (i%4 == 0)
	      temp.orderid = buf[i];
	    if (i%4 == 1)
	      temp.arrival = stoi(buf[i]);
		if (i%4 == 2)
	      temp.duration = stoi(buf[i]);
		if (i%4 == 3) {
	      	temp.timeout = stoi(buf[i]);
	    	note.push_back(temp);
	    }

	}
    sdt = clock();
    shellSort(note);
    sdt = clock() - sdt;
    wdt = clock();
	for(int t = 0; t < note.size() ;t++) {
			newFile << note[t].orderid << "\t" <<note[t].arrival << "\t" << note[t].duration << "\t" << note[t].timeout <<  endl;
			cout <<"["<< t+1 <<"]\t"<<note[t].orderid << "\t" <<note[t].arrival << "\t" << note[t].duration << "\t" << note[t].timeout <<  endl;
	}
	wdt = clock() - wdt;
	cout << "Reading Data = " << (float)rdt * 1000 / CLOCKS_PER_SEC << " ms" << endl;
	cout << "Sorting Data = " << (float)sdt * 1000 / CLOCKS_PER_SEC << " ms" << endl;
	cout << "Writting Data = " << (float)wdt * 1000 / CLOCKS_PER_SEC << " ms" << endl;
	myFile.close();
    newFile.close();
}

void case2(){
	list temp;
    int totaldelay;
	float failure;
	cook no1;
    vector<list> note;
    string realname;
    string newname;
    getNameforCase2(realname,newname);
    ifstream myFile;
    ofstream newFile;
    myFile.open(realname.c_str());
    if (!myFile.good()) {
      cout << "wrong";
      return;
    }
    newFile.open(newname.c_str());
    string line;
    vector<string> buf;
    while(getline(myFile, line)){
        split(line, buf);
    }

	for(int i = 4; i < buf.size() ;i++ ){
		if (i%4 == 0)
	      temp.orderid = buf[i];
	    if (i%4 == 1)
	      temp.arrival = stoi(buf[i]);
		if (i%4 == 2)
	      temp.duration = stoi(buf[i]);
		if (i%4 == 3) {
	      	temp.timeout = stoi(buf[i]);
	    	note.push_back(temp);
	    }

	}


    cout << "OID\tArrival\tDuration\tTimeout\n";
	for(int i =0;i < note.size();i++)
        cout << note[i].orderid  << "\t"<< note[i].arrival << "\t"<< note[i].duration << "\t"<< note[i].timeout << endl;
    // 開始處理!
    for ( int i = 0 ; i < note.size(); i++ ) {
      no1.oneCook(note[i]);
      if ( i == note.size() -1 && no1.queue.size() > 0){ // 讓佇列裡的東東跑完
      	temp.arrival = temp.timeout + 10000;
      	no1.oneCook(temp);
	  }
    }



    newFile << "\t[Abort List]\n\tOID\tDelay\tAbort\n";
    //cout << "\t[Abort List]\n\tOID\tDelay\tAbort\n";
    for(int i =0;i<abt.size();i++){
        newFile << "[" << i+1 << "]\t"<< abt[i].oid << "\t" << abt[i].delay << "\t" << abt[i].abort << endl;
        //cout << "[" << i+1 << "]\t"<< abt[i].oid << "\t" << abt[i].delay << "\t" << abt[i].abort << endl;
    }

    newFile << "\t[Timeout List]\n\tOID\tDelay\tDeparture\n";
    //cout << "\t[Abort List]\n\tOID\tDelay\tAbort\n";
    for(int i =0;i<tout.size();i++){
        newFile << "[" << i+1 << "]\t" << tout[i].oid << "\t" << tout[i].delay << "\t" << tout[i].depart << endl;
        //cout << "[" << i+1 << "]\t" << tout[i].oid << "\t" << tout[i].delay << "\t" << tout[i].depart << endl;
    }

    totaldelay = Adddelay(abt,tout);
	failure = Failure(note,abt,tout);
	newFile << "[Total Delay]\n" << totaldelay << " min."<< endl;
	if(failure>=10)
        newFile << "[Failure Percentage]\n" << setprecision (4) << failure << " %" ;
    else    newFile << "[Failure Percentage]\n" << setprecision (3) << failure << " %" ;

	//cout << "[Total Delay]\n" << totaldelay << "min.\n"<< "[Failure Percentage]\n" << failure << " %" ;

    myFile.close();
    newFile.close();

}

void case3(){
	list temp;
	a item;
    int totaldelay;
	float failure;
	cook no1;
    cook no2;
    vector<list> note;
    string realname;
    string newname;
    getNameforCase3(realname,newname);
    ifstream myFile;
    ofstream newFile;
    myFile.open(realname.c_str());
    if (!myFile.good()) {
      cout << "wrong";
      return;
    }
    newFile.open(newname.c_str());
    string line;
    vector<string> buf;
    while(getline(myFile, line)){
        split(line, buf);
    }

	for(int i = 4; i < buf.size() ;i++ ){
		if (i%4 == 0)
	      temp.orderid = buf[i];
	    if (i%4 == 1)
	      temp.arrival = stoi(buf[i]);
		if (i%4 == 2)
	      temp.duration = stoi(buf[i]);
		if (i%4 == 3) {
	      	temp.timeout = stoi(buf[i]);
	    	note.push_back(temp);
	    }

	}

	cout << "OID\tArrival\tDuration\tTimeout\n";
	for(int i =0;i < note.size();i++)
        cout << note[i].orderid  << "\t"<< note[i].arrival << "\t"<< note[i].duration << "\t"<< note[i].timeout << endl;

    Dealwithcase3( note, no1, no2, item);

    newFile << "\t[Abort List]\n\tOID\tCID\tDelay\tAbort\n";
    //cout << "\t[Abort List]\n\tOID\tDelay\tAbort\n";
    for(int i =0;i<abt.size();i++){
        newFile << "[" << i+1 << "]\t"<< abt[i].oid << "\t" << abt[i].cid<<"\t" << abt[i].delay << "\t" << abt[i].abort << endl;
        //cout << "[" << i+1 << "]\t"<< abt[i].oid << "\t" << abt[i].cid<< "\t" << abt[i].delay << "\t" << abt[i].abort << endl;
    }

    newFile << "\t[Timeout List]\n\tOID\tCID\tDelay\tDeparture\n";
    //cout << "\t[Abort List]\n\tOID\tDelay\tAbort\n";
    for( int i =0;i<tout.size();i++){
        newFile << "[" << i+1 << "]\t" << tout[i].oid << "\t" << tout[i].cid<< "\t" << tout[i].delay << "\t" << tout[i].depart << endl;
        //cout << "[" << i+1 << "]\t" << tout[i].oid  << "\t" << tout[i].cid<< "\t" << tout[i].delay << "\t" << tout[i].depart << endl;
    }

    totaldelay = Adddelay(abt,tout);
	failure = Failure(note,abt,tout);
	newFile << "[Total Delay]\n" << totaldelay << " min."<< endl;
	if(failure>=10)
        newFile << "[Failure Percentage]\n" << setprecision (4) << failure << " %" ;
    else    newFile << "[Failure Percentage]\n" << setprecision (3) << failure << " %" ;
	//cout << "[Total Delay]\n" << totaldelay << "min.\n"<< "[Failure Percentage]\n" << failure << " %" ;

    myFile.close();
    newFile.close();

}

void case4(){
    list temp;
	a item;
    int totaldelay;
	float failure;
	int cooknum;
    vector<cook> clist;
    vector<list> note;
    string realname;
    string newname;
    getNameforCase4(realname,newname);
    ifstream myFile;
    ofstream newFile;
    myFile.open(realname.c_str());
    if (!myFile.good()) {
      cout << "wrong";
      return;
    }
    newFile.open(newname.c_str());
    string line;
    vector<string> buf;
    while(getline(myFile, line)){
        split(line, buf);
    }

	for(int i = 4; i < buf.size() ;i++ ){
		if (i%4 == 0)
	      temp.orderid = buf[i];
	    if (i%4 == 1)
	      temp.arrival = stoi(buf[i]);
		if (i%4 == 2)
	      temp.duration = stoi(buf[i]);
		if (i%4 == 3) {
	      	temp.timeout = stoi(buf[i]);
	    	note.push_back(temp);
	    }

	}
    cout << "Input the number of cooks = ";
    cin >> cooknum;
    for(int i =0 ; i < cooknum ; i++)
      clist.push_back(cook());

	cout << "OID\tArrival\tDuration\tTimeout\n";
	for(int i =0;i < note.size();i++)
        cout << note[i].orderid  << "\t"<< note[i].arrival << "\t"<< note[i].duration << "\t"<< note[i].timeout << endl;

    Dealwithcase4( note, clist);

    newFile << "\t[Abort List]\n\tOID\tCID\tDelay\tAbort\n";
    //cout << "\t[Abort List]\n\tOID\tDelay\tAbort\n";
    for(int i =0;i<abt.size();i++){
        newFile << "[" << i+1 << "]\t"<< abt[i].oid << "\t" << abt[i].cid<<"\t" << abt[i].delay << "\t" << abt[i].abort << endl;
        //cout << "[" << i+1 << "]\t"<< abt[i].oid << "\t" << abt[i].cid<< "\t" << abt[i].delay << "\t" << abt[i].abort << endl;
    }

    newFile << "\t[Timeout List]\n\tOID\tCID\tDelay\tDeparture\n";
    //cout << "\t[Abort List]\n\tOID\tDelay\tAbort\n";
    for( int i =0;i<tout.size();i++){
        newFile << "[" << i+1 << "]\t" << tout[i].oid << "\t" << tout[i].cid<< "\t" << tout[i].delay << "\t" << tout[i].depart << endl;
        //cout << "[" << i+1 << "]\t" << tout[i].oid  << "\t" << tout[i].cid<< "\t" << tout[i].delay << "\t" << tout[i].depart << endl;
    }

    totaldelay = Adddelay(abt,tout);
	failure = Failure(note,abt,tout);
	newFile << "[Total Delay]\n" << totaldelay << " min."<< endl;
	if(failure>=10)
        newFile << "[Failure Percentage]\n" << setprecision (4) << failure << " %" ;
    else    newFile << "[Failure Percentage]\n" << setprecision (3) << failure << " %" ;
	//cout << "[Total Delay]\n" << totaldelay << "min.\n"<< "[Failure Percentage]\n" << failure << " %" ;

    myFile.close();
    newFile.close();


}

int main(){
  	int command = 0; // user command
	do{
    	abt.clear();
     	tout.clear();  // 要重製不然會出事
		cout << endl << "**** Simulate FIFO Queues by SQF *****";
		cout << endl << "* 0. Quit *";
		cout << endl << "* 1. Sort a file *";
 		cout << endl << "* 2. Simulate one FIFO queue *";
 		cout << endl << "* 3. Simulate two queues by SQF *";
 		cout << endl << "*******************************";
 		cout << endl << "Input a command(0, 1, 2, 3, 4): ";
 		cin >> command; // get the command
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
 				break;
 			default: cout << endl << "Command does not exist!" << endl;
 		} // end switch
 	} while (command != 0); // '0': stop the program

} // main()
