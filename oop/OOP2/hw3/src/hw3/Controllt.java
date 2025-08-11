package hw3;

public class Controllt extends Light implements Control {

	@Override
	public void execute() {
		// TODO Auto-generated method stub
		if (getBrightness() == 0) {
			dim();
		}
		else if (getBrightness() == 1) {
			bright();
		}
		else {
			off();
		}
			
	}

}
