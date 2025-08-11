package hw3;

public class Controltv extends Television implements Control {

	@Override
	public void execute() {
		// TODO Auto-generated method stub
		if(getStatus()) {
			off();
		}
		else
			on();
	}

}
