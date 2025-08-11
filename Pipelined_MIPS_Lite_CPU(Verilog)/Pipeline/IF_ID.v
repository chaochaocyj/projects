module IF_ID( clk, rst, pc, instr, check, pc_Reg, instr_Reg, check_Reg );

	input [31:0] pc, instr;
	input clk, rst ;
	input [6:0] check ;
	output reg [6:0] check_Reg ;
	output reg [31:0] instr_Reg, pc_Reg;

	always@( posedge clk )
	begin
		if ( rst )
		begin
			pc_Reg <= 0;
			instr_Reg <= 0;
			
		end
		else
		begin
			pc_Reg <= pc;
			instr_Reg <= instr;
			
		end
	end
	
endmodule 
