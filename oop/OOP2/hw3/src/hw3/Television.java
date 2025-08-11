package hw3;

public class Television {
    private boolean isOn = false;
    
    public void on() {
        isOn = true;
        System.out.println("開電視");
    }
    
    public void off() {
        isOn = false;
        System.out.println("關電視");
    }
    
    public boolean getStatus() {
        return isOn;
    }
}
