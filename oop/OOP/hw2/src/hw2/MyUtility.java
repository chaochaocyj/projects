package hw2;

public  class MyUtility {
	  public void mySort(Object[] arr,Compare c) {

		    int n = arr.length;
		    // sort the array in ascending order
		    for ( int i = 0; i < n; i++ ) {
		      for ( int j = 0; j < n - i - 1; j++ ) {
		          if (c.compare(arr[j], arr[j+1])) { // 比較
		        	  Object t = arr[j];
		        	  arr[j] = arr[j+1];
		        	  arr[j+1] = t;
		          }
		      }
		  }
	  }
	  
}
