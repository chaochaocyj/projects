module MUX4to1(out, in0, in1, in2, in3, sel);
	output out;
	input in0, in1, in2, in3;
	input [1:0]sel;
	wire out;
	assign out = (sel[1]) ? ( sel[0] ? in3 : in2 ):( sel[0] ? in1 : in0 ); 
endmodule