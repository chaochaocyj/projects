package hw2;

public class Test {
	public void test() {
        Dog [] dogList = new Dog [10];
        /* 這邊做一些Dog物件的建立與設定*/
        dogList[0] = new Dog();
        dogList[0].setage(10);
        dogList[0].setname("Mary");
        dogList[1] = new Dog();
        dogList[1].setage(5);
        dogList[1].setname("Jason");
        dogList[2] = new Dog();
        dogList[2].setage(7);
        dogList[2].setname("Susan");
        dogList[3] = new Dog();
        dogList[3].setage(3);
        dogList[3].setname("Jerry");
        dogList[4] = new Dog();
        dogList[4].setage(2);
        dogList[4].setname("Sam");
        dogList[5] = new Dog();
        dogList[5].setage(9);
        dogList[5].setname("Mark");
        dogList[6] = new Dog();
        dogList[6].setage(6);
        dogList[6].setname("Kevin");
        dogList[7] = new Dog();
        dogList[7].setage(8);
        dogList[7].setname("Linda");
        dogList[8] = new Dog();
        dogList[8].setage(1);
        dogList[8].setname("Lena");
        dogList[9] = new Dog();
        dogList[9].setage(4);
        dogList[9].setname("Joyce");
        
        MyUtility util = new MyUtility();
        
        util.mySort(dogList,dogList[0]); //此行執行完後 dogList 是個排序好的陣列
        
        for(int i =0; i<dogList.length;i++)
        	System.out.println(dogList[i].getage()+" "+dogList[i].getname());
        System.out.println("~第一類~");
        
        Cat [] catList = new Cat [5];
        catList[0] = new Cat();
        catList[0].setweight(5);
        catList[0].setvariety("Shorthair");
        catList[1] = new Cat();
        catList[1].setweight(3);
        catList[1].setvariety("Persian");
        catList[2] = new Cat();
        catList[2].setweight(1);
        catList[2].setvariety("Ragdoll");
        catList[3] = new Cat();
        catList[3].setweight(4);
        catList[3].setvariety("Russian Blue");
        catList[4] = new Cat();
        catList[4].setweight(2);
        catList[4].setvariety("Bengal");
        
        util.mySort(catList,catList[0]);
        
        for(int i =0; i<catList.length;i++)
        	System.out.println(catList[i].getweight()+" "+catList[i].getvariety());
        
        System.out.println("~第二類~");
        

        
	}

}
