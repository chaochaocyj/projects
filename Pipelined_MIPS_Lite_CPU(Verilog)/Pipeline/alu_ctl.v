
module alu_ctl( clk, ALUop, Funct, SignaltoALU, SignaltoSHT, SignaltoMULTU, SignaltoMUX, check, add );
	input clk ;
 	input [1:0] ALUop;
 	input [5:0] Funct;
	input [6:0] check ;
	reg [2:0] ALUOperation ;
	reg [5:0] SRLOperation ;
	reg [1:0] MUXSel;
	reg [5:0] MULTUOperation ;
	reg [6:0] counter ;
	output reg add;
	output [2:0] SignaltoALU ;
	output [5:0] SignaltoMULTU ;
	output [1:0] SignaltoMUX ;
	output [5:0] SignaltoSHT ;

  // symbolic constants for instruction function code
  parameter F_add = 6'd32;
  parameter F_sub = 6'd34;
  parameter F_and = 6'd36;
  parameter F_or  = 6'd37;
  parameter F_slt = 6'd42;
  parameter F_srl = 6'd2;
  parameter F_mult = 6'd25;
  parameter F_mfhi = 6'd16;
  parameter F_mflo = 6'd18;
  parameter F_maddu = 6'd1;
	

  // symbolic constants for ALU Operations
  parameter ALU_and = 3'b000;//0
  parameter ALU_or  = 3'b001;//1
  parameter ALU_add = 3'b010;//2
  parameter ALU_sub = 3'b110;//6
  parameter ALU_slt = 3'b111;//7
  parameter ALU_srl = 6'b000010;//shift
  parameter ALU_mfhi =3'b100;//4
  parameter ALU_mflo =3'b011;//3

  always @( Funct ) 
	begin
		if ( Funct == F_mult || Funct == F_maddu )
		  counter = 0 ; // initial counter

 	end

  always @(ALUop or Funct)
 	begin
	MUXSel = 2'b00 ;
        case (ALUop) 
            2'b00 : ALUOperation = ALU_add; // for lw and sw and addiu
            2'b01 : ALUOperation = ALU_sub; // for beq
            2'b10 : case (Funct) // R-type, up to funct
                        F_add : ALUOperation = ALU_add;
                        F_sub : ALUOperation = ALU_sub;
                        F_and : ALUOperation = ALU_and;
                        F_or  : ALUOperation = ALU_or;
                        F_slt : ALUOperation = ALU_slt;
                        F_srl : 
				                begin
				                  SRLOperation = ALU_srl;
				                  MUXSel = 2'b11 ;
				                end
				                 
                        F_mfhi : begin 
				                ALUOperation = ALU_mfhi;
				                MUXSel = 2'b01 ;
			                  end
                        F_mflo : begin
				                ALUOperation = ALU_mflo;
				                MUXSel = 2'b10 ;
			                  end
			            F_mult : begin 
							MULTUOperation = 6'b011001;
							add = 0;
						end
						
			            F_maddu : begin
							MULTUOperation = 6'b000001;
							add = 1;
						end
                    endcase
            default begin
		        ALUOperation = 3'bxxx;
		        MULTUOperation = 6'bxxxxxx;
		        end
        endcase
	end
	
	always @( posedge clk )
	begin
		if ( MULTUOperation == 6'b111111 )
			MULTUOperation = 6'bxxxxxx;
		else
		begin 
			if (  MULTUOperation == 6'b011001 || MULTUOperation == 6'b000001)
			begin
				counter = counter + 1 ;
				
				if ( counter == 32 )
				begin
					MULTUOperation = 6'b111111; 
					counter = 0 ;
				end
			end
		end
	end
	
	assign SignaltoALU = ALUOperation ;
	assign SignaltoSHT = SRLOperation ;
	assign SignaltoMULTU = MULTUOperation ;
	assign SignaltoMUX = MUXSel ;
	
endmodule