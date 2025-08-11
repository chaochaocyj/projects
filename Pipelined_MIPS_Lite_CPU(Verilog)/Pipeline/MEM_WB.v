module MEM_WB( clk, rst, WB, RD, pc, ALUout, WN_EX, WB_Reg, RD_Reg, pc_Reg, ALU_Reg, WN_Reg );

	input clk, rst;
	input [1:0] WB;
	input [4:0] WN_EX ;
	input [31:0] RD, pc, ALUout;

	output reg [1:0] WB_Reg;
	output reg [31:0] RD_Reg, pc_Reg, ALU_Reg;
	output reg [4:0] WN_Reg;

	always@( posedge clk )
	begin
		if ( rst )
		begin
			WB_Reg <= 0;
			RD_Reg <= 0;
			pc_Reg <= 0;
			ALU_Reg <= 0;
			WN_Reg <= 0;
		end
		else
		begin
			WB_Reg <= WB;
			RD_Reg <= RD;
			pc_Reg <= pc;
			ALU_Reg <= ALUout;
			WN_Reg <= WN_EX ;		
		end
	end

endmodule
