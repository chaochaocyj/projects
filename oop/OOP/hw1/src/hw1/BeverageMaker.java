package hw1;

public abstract class BeverageMaker {
	public final void prepare() {
		  boilWater();     // 步驟1
		  brew();          // 步驟2
		  pourInCup();     // 步驟3
		  addIce();		   // 步驟4
		  addCondiments(); // 步驟5
	}
	  
	public void boilWater() {
		System.out.println("煮水");
	}
	  
	public void pourInCup() {
		System.out.println("倒進杯子");
	}
	
	abstract void brew();
	abstract void addIce();
	abstract void addCondiments();
}
