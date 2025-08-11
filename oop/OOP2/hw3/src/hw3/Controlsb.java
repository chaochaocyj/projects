package hw3;

public class Controlsb extends Soundbox implements Control {

	@Override
	public void execute() {
		// TODO Auto-generated method stub
		if ( getLevel() == 0) {
			low();
		}
		else if (getLevel() == 1) {
			medium();
		}
		else if (getLevel() == 2) {
			high();
		}
		else {
			off();
		}
	}

}
