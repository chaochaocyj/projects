package hw2;

public class Dog implements Compare{
	int age;
	String name;
	public int getage() {
		return age;
	}
	public String getname() {
		return name;
	}
	public void setage(int num) {
		age = num;
	}
	public void setname(String s) {
		name = s;
	}
	
	public boolean compare(Object o1,Object o2) {
		Dog d1 = (Dog) o1;
		Dog d2 = (Dog) o2;
		if (d1.age>d2.age) {
			return true;
		}
		return false;
	}
	
}
