// 11027156 林芷安 11027164 趙怡儒
#include <iostream>
#include <fstream> // open, is_open, close, ignore
#include <string> // string, find_last_of, substr
#include <vector> // vector, push_back
#include <cstdlib> // system, atoi
#include <iomanip> // setw
#include <cstring>
#include <sstream>
using namespace std;

struct pokemon{
    string num;
    string name;
    string t1;
    string t2;
    string total;
    int hp;
    string atk;
    string def;
    string spatk;
    string spdef;
    string speed;
    string gen;
    string legen;

};

struct bst{
    vector<pokemon> data;
    bst* left;
    bst* right;

};
typedef bst * bstptr;

void getNameforCase1(string &realname){
 string inputname;
 realname="input000";
 cout << "input 601,602...[0]Quit:";
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
      getNameforCase1(realname);
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

  if (next == string::npos) break;
  current = next + 1; //下次由 next + 1 的位置開始找起。
 }
}

void bubble(vector<pokemon>& p){
    pokemon t;
    for(int i = p.size() ; i> 0 ;i--){
        for(int j = 0; j < i - 1 ;j++){
            if(p[j+1].hp>p[j].hp){
                t=p[j];
                p[j]=p[j+1];
                p[j+1]=t;
            }

            else if(p[j+1].hp==p[j].hp&& stoi(p[j+1].num)<stoi(p[j].num)){
                t=p[j];
                p[j]=p[j+1];
                p[j+1]=t;
            }

        }
    }
} // bubble

bstptr Insert(bstptr t,pokemon p){
	if (t == NULL){
		t = new bst;
		t -> data.push_back(p);
		t -> left = NULL;
		t -> right = NULL;
	}
	else if (p.hp < t ->data[0].hp)
		t -> left = Insert(t -> left, p);
	else if (p.hp > t ->data[0].hp)
		t -> right = Insert(t -> right, p);
	else // 等於就直接加上去
		t -> data.push_back(p);
	return t;
}


int level(bstptr t){
    if(t==NULL)
		return 0;
    else{
        int l=level(t -> left);
        int r=level(t -> right);

        if(l>r)
            return l+1;
        else return r+1;
    }

}


void seek( bstptr t, vector<pokemon> &p,int target, int & time){
	if( t == NULL )
    	return;
	if( t -> data[0].hp > target ) { // 這個點比目標大，兩邊都要確定
      for (int i = 0; i < t ->data.size(); i++){
      	p.push_back(t -> data[i]);
      }
    	time++;

    	seek(t -> left,p,target,time);
    	seek(t -> right,p,target,time);
  	}


  	else { // 這個點跟目標一樣或較小，往右邊找就好
    	if( t -> data[0].hp == target ) {
      		for (int i = 0; i < t ->data.size(); i++){
        		p.push_back(t -> data[i]);
      		}
    	}
    	time++;
    	seek(t -> right,p,target,time);
  	}
}

bstptr getMax(bstptr t,bstptr &p){
	while( t -> right != NULL ) {
		p = t;
		t = t -> right;
    }
    return t;

}

bstptr getMin(bstptr t,bstptr &p){
	while( t -> left != NULL ) {
		p = t;
		t = t -> left;
    }
    return t;
}



void case1(bstptr &tree){
 string line;
 string dontneed;
 string realname;
 bstptr head = NULL;
 vector<pokemon> note; 
 getNameforCase1(realname);

 if(realname=="0") return;
 // cout << realname << newname;
    ifstream myFile;
    myFile.open(realname.c_str());
    if(!myFile.is_open()){
        cout << "File does not exist!";
        return;
    }

    string s ;
    vector<string> buf;
    //vector<pokemon> note;

    getline(myFile, dontneed);//第一行

    while(getline(myFile, s)){
      split(s, buf);
    }
    pokemon temp;
    for(int i = 0; i < buf.size() ;i++ ){
        if (i%13 == 0)
            temp.num = buf[i];
        if (i%13 == 1)
            temp.name = buf[i];
        if (i%13 == 2)
            temp.t1 = buf[i];
        if (i%13 == 3 )
            temp.t2 = buf[i];
        if (i%13 == 4)
            temp.total = buf[i];
        if (i%13 == 5)
            temp.hp = stoi(buf[i]);
        if (i%13 == 6)
            temp.atk = buf[i];
        if (i%13 == 7)
            temp.def = buf[i];
        if (i%13 == 8)
            temp.spatk = buf[i];
        if (i%13 == 9)
            temp.spdef = buf[i];
        if (i%13 == 10)
            temp.speed = buf[i];
        if (i%13 == 11)
            temp.gen = buf[i];
        if (i%13 == 12 ) { // 滿了
            temp.legen = buf[i];
            note.push_back(temp);
        } // if
    }



    for(int i =0;i < note.size() ;i++){
      tree = head;
      tree = Insert(tree, note[i]);
      if ( i == 0)
          head = tree; // 記住第一個位置
    }

    /*
    for(int t = 0; t < note.size() ;t++) {
        cout <<"[" <<t+1<<"]"<<note[t].num << "\t" << note[t].name << "\t" << note[t].t1 << "\t" << note[t].t2 << "\t" << note[t].total << "\t" << note[t].hp << "\t"<< note[t].atk << "\t"<< note[t].def << "\t";
        cout << note[t].spatk << "\t" << note[t].spdef << "\t" << note[t].speed << "\t" << note[t].gen << "\t" << note[t].legen << endl;
    }
    cout <<"_________________________________check\n";
    for(int k =0; k < note.size();k++)
      cout << note[k].hp << endl;
    cout <<"_________________________________check\n";
    */

    tree = head; // 指回開頭
    int l=level(tree);
    cout << "\t#\tName\t\tType 1\tHP\tAttack\tDefense\n";
    for(int t = 0; t < note.size() ;t++) {
        cout <<"[" <<t+1<<"]\t"<<note[t].num << "\t" << note[t].name << "\t" << note[t].t1 << "\t"<< note[t].hp << "\t"<< note[t].atk << "\t"<< note[t].def << endl;
    }
    tree = head; // 指回開頭
    cout << "HP tree height = " << l << endl;
    head = NULL ;
    myFile.close();
}

void case2(bstptr &tree){
	bstptr head = tree;
	vector<pokemon> note;
	int target = 0;
	int time = 0;
	cout << "Threshold (a positive integer):";
	cin >> target;
	while(target<0){
        cout << "Threshold (a positive integer):";
    	cin >> target;
	}
	cout << endl;
	seek(head,note,target,time);
    bubble(note);
	cout << "\t#\tName\t\tType 1\tTotal\tHP\tAttack\tDefense\n";
	for(int t = 0; t < note.size() ;t++) {
        cout <<"[" <<t+1<<"]\t"<<note[t].num << "\t" << note[t].name << "\t" << note[t].t1 <<"\t"<<note[t].total<< "\t"<< note[t].hp << "\t"<< note[t].atk << "\t"<< note[t].def << endl;
    }
	cout << "Number of visited nodes = " << time << endl;
	head = NULL;
}

void case3(bstptr &tree){
	bstptr head = tree;
	bstptr parent = NULL;
	bstptr target = getMax(tree, parent);
	//
	//cout << target -> data[0].hp << "t_debug~\n";
	pokemon op;
	int index =0;
	// cout <<"!!!" <<target -> data[0].name << "!!!";
	if ( target -> data.size() > 1){ //此最大值hp有兩筆以上的資料
	   // cout <<"264";
	   op = target -> data[0];
	   target -> data.erase(target ->data.begin());
	}
	else{
		if ( target -> left == NULL && target -> right == NULL ){ // 沒有子節點
		    // cout << "~one~";
			if (target != head ){
            	if (parent->left == target) {
               		parent->left = NULL;
            	}
            	else {
                	parent->right = NULL;
            	}
        	}
        	else {
            	head = NULL;
        	}

        	op = target -> data[0];
        	target -> data.clear();
            delete target;
		}
		else if( target -> left != NULL && target -> right != NULL){ // 有兩個子節點
		    // cout << "~two~";
			bstptr successor = getMin(target,parent); // 找後繼
			// cout << successor -> data[0].hp << "s_debug~\n";
			op = target -> data[0];
			target -> data.clear();
			for (int i=0; i<successor->data.size(); i++) //把資料移到target去
              target->data.push_back(successor->data[i]);
            parent -> left = NULL; // 後繼的parent->left就不用指東西了
            delete successor;
            successor = NULL;
		}
		else{ // 只有一個子節點
		    // cout << "~three~";
        	bstptr child = NULL;
        	if ( target -> left != NULL ){
        		child = target -> left;
        		// cout << "leftchild\n";
            }
        	else{
        		child = target -> right;
        		// cout << "rightchild\n";
        	}
        	if (target != head){
            	if (target == parent->left) {
                	parent->left = child;
            	}
            	else {
                	parent->right = child;
            	}
       		}

        	else {
            	head = child;
        	}

        	op = target -> data[0];
        	target -> data.clear();
            delete target;

		}

	}

	cout <<"#       Name    Type 1  Type 2  Total   HP      Attack  Defense Sp. Atk Sp. Def Speed   Generation      Legendary\n";
	cout <<op.num<<"\t"<<op.name<<"\t"<<op.t1<<"\t"<<op.t2<<"\t"<<op.total<<"\t"<<op.hp<<"\t"<<op.atk<<"\t"<<op.def<<"\t";
	cout<<op.spatk<<"\t"<<op.spdef<<"\t"<<op.speed<<"\t"<<op.gen<<"\t"<<op.legen<<endl;
	tree = head; // 指回開頭
    int l=level(tree);
    cout << "HP tree height = " << l << endl;
    head = NULL;
    parent = NULL;
    target = NULL;


}

void clear(bstptr &t){
    if (t!= NULL ){
        clear(t->left);
        clear(t->right);
        delete t;
        t = NULL ;
    }
}

void storeBSTNodes(bstptr t, vector<bstptr> &nodes){  // inorder traversal
    if (t==NULL)
        return;
    storeBSTNodes(t->left, nodes);
    nodes.push_back(t);
    storeBSTNodes(t->right, nodes);
}

bstptr buildbalancedtree(vector<bstptr> &nodes, int start,int end){
    if (start > end)
        return NULL;

    int mid = (start + end)/2;
    bstptr head = nodes[mid]; // 把中間的設成頭 

    head->left  = buildbalancedtree(nodes, start, mid-1); // 左邊種 
    head->right = buildbalancedtree(nodes, mid+1, end); // 右邊種 

    return head;
}

bstptr treetovec(bstptr tree){ // 把樹存進vec 
    vector<bstptr> node;
    storeBSTNodes(tree, node); // 用inorder的方法存 

    int n = node.size();
    return buildbalancedtree(node, 0, n-1);
}


void printonelevel(bstptr t, int level){
    if (t == NULL)
        return;
    if (level == 1){
        cout << "("<< t->data[0].hp ;
        for (int i = 0; i < t ->data.size(); i++){
            cout << ","<< t -> data[i].num;
        }
        cout <<")";

    }
    else if (level > 1) {
        printonelevel(t->left, level - 1);
        printonelevel(t->right, level - 1);
    }
}

void printbylevel(bstptr t)
{
    int h = level(t);
    int i;
    for (i = 1; i <= h; i++) {
        cout <<"<level " << i << ">";
        printonelevel(t, i);
        cout << endl;
    }
}

void case4(bstptr &tree){
    tree = treetovec(tree);
    printbylevel(tree);
}



int main(){
	int command = 0; // user command
	bstptr tree = NULL;

 	do{
		cout << endl << "*** Binary Search Tree on Pokemon ***";
 		cout << endl << "* 0. QUIT                           *";
 		cout << endl << "* 1. Read one file to build BST     *";
 		cout << endl << "* 2. Threshold search on one column *";
 		cout << endl << "* 3. Delete the max on one column   *";
 		cout << endl << "* 4. Convert into balanced BST      *";
 		cout << endl << "*************************************";
 		cout << endl << "Input a choice(0, 1, 2, 3, 4): ";
 		cin >> command; // get a command
 		switch (command){
			case 0: break;
 			case 1:
 				clear(tree);
 				if ( tree == NULL)
 				  cout << "----Successfully initialized----\n";
 				case1(tree);
 				break;
 			case 2:
 				if ( tree == NULL)
					cout << "----Execute Mission 1 first!----\n";
				else
 					case2(tree);
 				break;
 			case 3:
 				if ( tree == NULL)
					cout << "----Execute Mission 1 first!----\n";
				else
 					case3(tree);
 				break;
            case 4:
 				if ( tree == NULL)
					cout << "----Execute Mission 1 first!----\n";
				else
 					case4(tree);
                break;

 			default:
				cout << endl << "Command does not exist!" << endl;
 		} // end switch
 	} while (command != 0); // '0': stop the program
 	system("pause"); // pause the display
 	return 0;
} // end main
