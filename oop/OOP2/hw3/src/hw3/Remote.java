package hw3;

public class Remote {
	Control [] buttons = new Control [3];
	Remote(){ // 建構子
	}
	
	void setbutton(int buttonId,Control c) {
		buttons[buttonId] = c;
	}
    
    void buttonClicked(int buttonId) {
    	buttons[buttonId].execute();
    }
    
    /**
     * 直接啟動所有的裝置(意即，每個裝置的按鈕都按一下)
     */
    void pressAllButtons() {
        for ( int i = 0; i < buttons.length; i++ ) {
            buttonClicked(i);
        }
    }
}
