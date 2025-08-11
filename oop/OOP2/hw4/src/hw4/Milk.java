package hw4;

public class Milk implements Tea {
	Tea t;
	int cost = 20;
	Milk(){} // 建構子
	Milk(Tea already){
		t = already;
	}
	@Override
	public String getIngredient() {
		if(t != null)
			return "牛奶\n" + t.getIngredient();
		else
			return "牛奶\n";
	}

	@Override
	public int getCost() {
		if(t != null)
			return cost + t.getCost();
		else
			return cost;
	}

}
