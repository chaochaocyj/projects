module MUX( ALUOut, HiOut, LoOut, Shifter, Signal, dataOut );
input [31:0] ALUOut ;
input [31:0] HiOut ;
input [31:0] LoOut ;
input [31:0] Shifter ;
input [1:0] Signal ;  //slt
output [31:0] dataOut ;

wire [31:0] dataOut ;




assign dataOut = ( Signal == 2'b00 ) ? ALUOut :
				 ( Signal == 2'b01 ) ? HiOut :
				 ( Signal == 2'b10 ) ? LoOut :
				 ( Signal == 2'b11 ) ? Shifter : 32'b0 ;

endmodule
