package hw4;

public class TestBeverage {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
	      /* 製作珍珠奶茶 */     
	      // 1.產生牛奶
	      // 2.加紅茶
	      // 3.加珍珠
	      // 完成！
		  Tea t1 = new Milk();
		  Tea t2 = new BlackTea(t1);
		  Tea t3 = new Pearl(t2);
		  System.out.println(t3.getIngredient());
		  System.out.println(t3.getCost());
		  System.out.println("_________________");
		  Tea t4 = new Sugar();
		  t1 = new GreenTea(t4);
		  t2 = new Pearl(t1);
		  t3 = new Milk(t2);
		  
		  System.out.println(t3.getIngredient());
		  System.out.println(t3.getCost());
		  	
	}

}
