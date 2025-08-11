module mips_pipeline( clk, rst );
    input clk, rst;

    // instruction
    wire[31:0] instr;
	wire [4:0] rs, rt_ID, rt_EX, rd_ID, rd_EX, shamt,shamt_EX;
    wire [5:0] opcode, funct, funct_EX;
    wire [6:0] check_IF, check_ID, check_EX;
	wire [15:0] immed;
	wire [31:0] extend_immed_ID, extend_immed_EX, b_offset ;
	wire [25:0] jumpOffset;
	
	
    	// signals
	wire [4:0] WnEX, WnMEM, WnWB, Wn ;
	wire [31:0] regFile_Rd1_ID, regFile_Rd2_ID, regFile_Rd1_EX, regFile_Rd2_EX, regFile_Rd2_MEM, regFile_WD, Alu_Beq, Alu_OutEX, Alu_OutM, Alu_OutWB, b_Add_EX,b_Mux_M, pc_Next,pc, pc_IF_add, pc_ID_add, pc_EX_add, pc_M_add, pc_WB_add, dM_RD_MEM,dM_RD_WB,                            jump_Add_ID,jump_Add_EX, jump_Add_MEM, branch_Add, memReg_out;
	
	wire [31:0] instr_IF, instr_ID ;

	// control signals
	// WB[0] : MemtoReg ,WB[1] : RegWrite
    // M[0] : MemWrite, M[1] : MemRead, M[2] : Branch, M[3] : Jump
    // EX[0] : ALUSrc, EX[2:1] : ALUOp, EX[3] : RegDst

    wire [1:0] WB_1, WB_2, WB_3, WB_4 ;
	wire [3:0] M_1, M_2, M_3;
	wire [3:0] EX_1, EX_2;
    


//  *****IF-preparation*****

//  Following:IF
	
	reg32 PC( .clk(clk), .rst(rst), .en_reg(en_pc), .d_in(pc_Next), .d_out(pc) );

	adder PCAdd( .a(pc), .b(32'd4), .result(pc_IF_add) );
    
	memory InstrMem( .clk(clk), .MemRead(1'b1), .MemWrite(1'b0), .wd(32'd0), .addr(pc), .rd(instr_IF) );
    
	
	hazard hazard_detect( .clk(clk), .rst(rst), .instr(instr_IF), .en_pc(en_pc), .hazard(hazard), .check(check_IF) );
	
	mux2 hazardMUX( .sel(hazard), .a(instr_IF), .b(32'd0), .y(instr) );
// Above: IF
// *****IF/ID*****
	
	IF_ID IF_ID_Reg( .clk(clk), .rst(rst), .pc(pc_IF_add), .instr(instr), .check(check_IF),
			 .pc_Reg(pc_ID_add), .instr_Reg(instr_ID), .check_Reg(check_ID) );
			 
// Following: ID
    
	assign en_pc = 1'b1;
	

	assign opcode = instr_ID[31:26];
	assign rs = instr_ID[25:21];
	assign rt_ID = instr_ID[20:16];
	assign rd_ID = instr_ID[15:11];
	assign shamt = instr_ID[10:6];
	assign funct = instr_ID[5:0];
	assign immed = instr_ID[15:0];
	assign jumpOffset = instr_ID[25:0];	
    assign jump_Add_ID = { pc_ID_add[31:28], jumpOffset <<2 };		// jump offset 
	
    reg_file regFile( .clk(clk), .RegWrite(WB_4[1]), .RN1(rs), .RN2(rt_ID), .WN(Wn),.WD(regFile_WD), .RD1(regFile_Rd1_ID                     ), .RD2(regFile_Rd2_ID) );
	
	sign_extend signExt( .immed_in(immed), .ext_immed_out(extend_immed_ID) );		// signextend
	
	control_single Ctl(.opcode(opcode), .funct(funct), .RegDst(EX_1[3]), .ALUSrc(EX_1[0]),.MemtoReg(WB_1[0]), // solve
                   	.RegWrite(WB_1[1]), .MemRead(M_1[1]), .MemWrite(M_1[0]), .Branch(M_1[2]),
                   	.Jump(M_1[3]), .ALUOp(EX_1[2:1]));
					
// Above: ID
// *****ID/EX*****
	ID_EX ID_EX_reg( .clk(clk), .rst(rst), .WB(WB_1) , .M(M_1), .EX(EX_1), .pc(pc_ID_add), .RD1(regFile_Rd1_ID), 
	.RD2(regFile_Rd2_ID), .immed_in(extend_immed_ID), .rt(rt_ID), .rd(rd_ID), .check(check_ID), 
	.jump_addr(jump_Add_ID), .funct(funct), .extend_SHT(shamt), .WB_Reg(WB_2), .MEM_Reg(M_2), .EX_Reg(EX_2), .pc_Reg(pc_EX_add), 
	.RD1_Reg(regFile_Rd1_EX), .RD2_Reg(regFile_Rd2_EX), .immed_in_Reg(extend_immed_EX), .rt_Reg(rt_EX), .rd_Reg(rd_EX), .check_Reg(check_EX), 
	.jump_addr_Reg(jump_Add_EX), .funct_Reg(funct_EX), .extend_SHT_Reg( shamt_EX ));
	
// Following: EX
	assign b_offset = extend_immed_EX << 2;		// branch offset 
	assign WnEX =   (EX_2[3]) ? (rd_EX) : (rt_EX);		// RegDst
	mux2 ALUMux( .sel(EX_2[0]), .a(regFile_Rd2_EX), .b(extend_immed_EX), .y(Alu_Beq) );	// ALUSrc

	adder BRAdd( .a(pc_EX_add), .b(b_offset), .result(b_Add_EX) );
	
	TotalALU totalALU( .clk(clk), .ALUop(EX_2[2:1]), .dataA(regFile_Rd1_EX), .dataB(Alu_Beq), 
			   .Funct(funct_EX), .Output(Alu_OutEX), .reset(rst), .check(check_EX), .extend_SHT( shamt_EX ) ) ;

	
// Above: EX

// *****EX/MEM*****
	EX_MEM EX_MEM_reg( .clk(clk), .rst(rst), .WB(WB_2), .M(M_2), .branch_PC(b_Add_EX), .pc(pc_EX_add), .ALUout(Alu_OutEX),
 		           .WN_EX(WnEX), .RD2(regFile_Rd2_EX), .J(jump_Add_EX), .WB_Reg(WB_3), .M_Reg(M_3), 
			   .branch_PC_Reg(b_Mux_M), .pc_Reg(pc_M_add), .ALU_Reg(Alu_OutM), .WN_mem(WnMEM), 
			   .WD(regFile_Rd2_MEM), .J_Reg(jump_Add_MEM) );
			   
// Following:MEM
	bra_equ Br_Equ( .RD1(regFile_Rd1_ID), .RD2(regFile_Rd2_ID), .Zero(Zero));
	
	and BR_AND(PCSrc, M_3[2], Zero); // solve
	
	memory DataMem( .clk(clk), .MemRead(M_3[1]), .MemWrite(M_3[0]), .wd(regFile_Rd2_MEM), 
				   .addr(Alu_OutM), .rd(dM_RD_MEM) );	   

	mux2 PCMux( .sel(PCSrc), .a(pc_IF_add), .b(b_Mux_M), .y(branch_Add) );		// PCSrc
    mux2 JMux( .sel(M_3[3]), .a(branch_Add), .b(jump_Add_MEM), .y(pc_Next) );		// Jump
    
    
// Above:MEM

// *****MEM/WB*****

	MEM_WB MEM_WB_reg( .clk(clk), .rst(rst), .WB(WB_3), .RD(dM_RD_MEM), .pc(pc_M_add), .ALUout(Alu_OutM), .WN_EX(WnMEM),
											 .WB_Reg(WB_4), .RD_Reg(dM_RD_WB), .pc_Reg(pc_WB_add), .ALU_Reg(Alu_OutWB)
											 , .WN_Reg(WnWB) );
    
// Following:WB
	assign Wn = WnWB;
	
	mux2 WDMux( .sel(WB_4[0]), .a(dM_RD_WB), .b(Alu_OutWB), .y(memReg_out) ); 	// 
	
	assign regFile_WD = (WB_4[1])? memReg_out:32'bx;
	
	
// Above:WB
	

endmodule
