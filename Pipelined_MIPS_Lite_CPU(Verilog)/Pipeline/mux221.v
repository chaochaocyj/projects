module Mux221( sel, a, b, y );
    input sel ;
    input a, b;
    output y;
    
  
assign y = sel ? b : a;
 

endmodule
