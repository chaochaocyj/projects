package hw2;

public class Cat implements Compare{
	int weight;
	String variety;
	public int getweight() {
		return weight;
	}
	public String getvariety() {
		return variety;
	}
	public void setweight(int num) {
		weight = num;
	}
	public void setvariety(String s) {
		variety = s;
	}
	
	public boolean compare(Object o1,Object o2) {
		Cat c1 = (Cat) o1;
		Cat c2 = (Cat) o2;
		if (c1.weight>c2.weight) {
			return true;
		}
		return false;
	}
}
