module EX_MEM( clk, rst, WB, M, branch_PC, pc, ALUout, WN_EX, RD2, J, WB_Reg, M_Reg, branch_PC_Reg, pc_Reg, ALU_Reg, WN_mem, WD, J_Reg );
					
	input clk, rst;

	input [1:0] WB;
	input [3:0] M;
	input [4:0] WN_EX;
	input [31:0] ALUout, RD2, branch_PC, pc;
	input [31:0] J;

	output reg [1:0] WB_Reg;
	output reg [3:0] M_Reg;
	output reg [4:0] WN_mem;
	output reg [31:0] ALU_Reg, WD, branch_PC_Reg, pc_Reg;
	output reg [31:0] J_Reg;

	always@( posedge clk )
	begin
		if ( rst )
		begin
			WB_Reg <= 0;
			M_Reg <= 0;
			branch_PC_Reg <= 0;
			pc_Reg <= 0;
			ALU_Reg <= 0;
			WD <= 0;
			WN_mem <= 0;
            J_Reg <= 0;
       
		end
		else
		begin
			WB_Reg <= WB;
			M_Reg <= M;
			branch_PC_Reg <= branch_PC;
			pc_Reg <= pc;
			ALU_Reg <= ALUout;
			WD <= RD2;	
			WN_mem <= WN_EX;
			J_Reg <= J ;
            
		end
	end
	
endmodule
