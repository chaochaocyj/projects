module ALU( dataA, dataB, ctl, dataOut, reset );
	input reset ;
	input [31:0] dataA ;
	input [31:0] dataB ;
	input [2:0] ctl ;
	output [31:0] dataOut ;

	//   Signal ( 6-bits)
	//   AND  : 36
	//   OR   : 37
	//   ADD  : 32
	//   SUB  : 34
	//   SLT  : 42

	wire [31:0] temp1 ;
	wire [31:0] temp2 ;
	wire [31:0] c;
	wire less;
	wire cout;
	wire binvert;
	wire [1:0]sel;

	parameter AND = 3'b000;
	parameter OR  = 3'b001;
	parameter ADD = 3'b010;
	parameter SUB = 3'b110;
	parameter SLT = 3'b111;



	assign temp1 = 32'b0;
	assign sel = ( ctl == AND ) ?  2'b00 :( ctl == OR ) ? 2'b01 : ( ctl == ADD ||  ctl == SUB ) ? 2'b10 : ( ctl == SLT ) ? 2'b11 : 2'b00;
	assign c[0] = ( ctl == SLT ||  ctl == SUB ) ? 1'b1 : 1'b0 ;
	assign binvert = c[0];
	ALU_1bit u_alu1( .a(dataA[0]), .b(dataB[0]), .cin(c[0]), .cout(c[1]), .sel(sel), .out(temp2[0]), .less(less),.binvert(binvert),.set());
	ALU_1bit u_alu2( .a(dataA[1]), .b(dataB[1]), .cin(c[1]), .cout(c[2]), .sel(sel), .out(temp2[1]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu3( .a(dataA[2]), .b(dataB[2]), .cin(c[2]), .cout(c[3]), .sel(sel), .out(temp2[2]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu4( .a(dataA[3]), .b(dataB[3]), .cin(c[3]), .cout(c[4]), .sel(sel), .out(temp2[3]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu5( .a(dataA[4]), .b(dataB[4]), .cin(c[4]), .cout(c[5]), .sel(sel), .out(temp2[4]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu6( .a(dataA[5]), .b(dataB[5]), .cin(c[5]), .cout(c[6]), .sel(sel), .out(temp2[5]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu7( .a(dataA[6]), .b(dataB[6]), .cin(c[6]), .cout(c[7]), .sel(sel), .out(temp2[6]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu8( .a(dataA[7]), .b(dataB[7]), .cin(c[7]), .cout(c[8]), .sel(sel), .out(temp2[7]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu9( .a(dataA[8]), .b(dataB[8]), .cin(c[8]), .cout(c[9]), .sel(sel), .out(temp2[8]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu10( .a(dataA[9]), .b(dataB[9]), .cin(c[9]), .cout(c[10]), .sel(sel), .out(temp2[9]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu11( .a(dataA[10]), .b(dataB[10]), .cin(c[10]), .cout(c[11]), .sel(sel), .out(temp2[10]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu12( .a(dataA[11]), .b(dataB[11]), .cin(c[11]), .cout(c[12]), .sel(sel), .out(temp2[11]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu13( .a(dataA[12]), .b(dataB[12]), .cin(c[12]), .cout(c[13]), .sel(sel), .out(temp2[12]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu14( .a(dataA[13]), .b(dataB[13]), .cin(c[13]), .cout(c[14]), .sel(sel), .out(temp2[13]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu15( .a(dataA[14]), .b(dataB[14]), .cin(c[14]), .cout(c[15]), .sel(sel), .out(temp2[14]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu16( .a(dataA[15]), .b(dataB[15]), .cin(c[15]), .cout(c[16]), .sel(sel), .out(temp2[15]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu17( .a(dataA[16]), .b(dataB[16]), .cin(c[16]), .cout(c[17]), .sel(sel), .out(temp2[16]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu18( .a(dataA[17]), .b(dataB[17]), .cin(c[17]), .cout(c[18]), .sel(sel), .out(temp2[17]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu19( .a(dataA[18]), .b(dataB[18]), .cin(c[18]), .cout(c[19]), .sel(sel), .out(temp2[18]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu20( .a(dataA[19]), .b(dataB[19]), .cin(c[19]), .cout(c[20]), .sel(sel), .out(temp2[19]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu21( .a(dataA[20]), .b(dataB[20]), .cin(c[20]), .cout(c[21]), .sel(sel), .out(temp2[20]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu22( .a(dataA[21]), .b(dataB[21]), .cin(c[21]), .cout(c[22]), .sel(sel), .out(temp2[21]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu23( .a(dataA[22]), .b(dataB[22]), .cin(c[22]), .cout(c[23]), .sel(sel), .out(temp2[22]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu24( .a(dataA[23]), .b(dataB[23]), .cin(c[23]), .cout(c[24]), .sel(sel), .out(temp2[23]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu25( .a(dataA[24]), .b(dataB[24]), .cin(c[24]), .cout(c[25]), .sel(sel), .out(temp2[24]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu26( .a(dataA[25]), .b(dataB[25]), .cin(c[25]), .cout(c[26]), .sel(sel), .out(temp2[25]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu27( .a(dataA[26]), .b(dataB[26]), .cin(c[26]), .cout(c[27]), .sel(sel), .out(temp2[26]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu28( .a(dataA[27]), .b(dataB[27]), .cin(c[27]), .cout(c[28]), .sel(sel), .out(temp2[27]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu29( .a(dataA[28]), .b(dataB[28]), .cin(c[28]), .cout(c[29]), .sel(sel), .out(temp2[28]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu30( .a(dataA[29]), .b(dataB[29]), .cin(c[29]), .cout(c[30]), .sel(sel), .out(temp2[29]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu31( .a(dataA[30]), .b(dataB[30]), .cin(c[30]), .cout(c[31]), .sel(sel), .out(temp2[30]), .less(1'b0),.binvert(binvert),.set());
	ALU_1bit u_alu32( .a(dataA[31]), .b(dataB[31]), .cin(c[31]), .cout(cout), .sel(sel), .out(temp2[31]), .less(1'b0),.binvert(binvert),.set(less));

	assign dataOut = (reset)? temp1:temp2;
endmodule