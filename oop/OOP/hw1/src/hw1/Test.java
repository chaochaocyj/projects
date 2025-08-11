package hw1;

public class Test {
	public void test() {
		BeverageMaker b1 = new TeaMaker();
		b1.prepare();
		System.out.println("~First beverage done!~");
		BeverageMaker b2 = new CoffeeMaker();
		b2.prepare();
		System.out.println("~Second beverage done!~");
	}
}
