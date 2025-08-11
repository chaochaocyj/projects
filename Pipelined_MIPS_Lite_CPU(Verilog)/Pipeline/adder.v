module adder(a, b, result);
  // for PC+4 and beq
  input [31:0] a, b;
  output [31:0] result;
        
  assign result = a + b;
endmodule