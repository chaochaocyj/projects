package hw3;

public class Light {
    private int brightness = 0;  // 同上，理論上這邊要用enum處理比較好
    
    public void off() {
        brightness = 0;
        System.out.println("關燈");
    }
    
    public void dim() {
        brightness = 1;
        System.out.println("燈光暗");
    }
    
    public void bright() {
        brightness = 2;
        System.out.println("燈光亮");
    }
    
    public int getBrightness() {
        return brightness;
    }
}
