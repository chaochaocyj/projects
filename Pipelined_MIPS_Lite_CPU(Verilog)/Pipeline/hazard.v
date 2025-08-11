module hazard( clk, rst, instr, en_pc, hazard, check );
	input clk, rst;
	input [31:0] instr;
	output reg en_pc, hazard;
	output reg [6:0] check;
	
	reg [6:0] count;
	
	always @ ( instr ) begin
	
		if ( instr[31:26] == 6'd0 && instr[5:0] == 6'd25 ) begin
				check <= 33;
				en_pc <= 0;
			end
		else if ( instr[31:26] == 6'd28 && instr[5:0] == 6'd1) begin 
				check <= 33;
				en_pc <= 0;
			end
		else if ( instr[31:0] == 32'd0 && count == 0 ) begin
			count <= 1;
			check <= 1;
			en_pc <= 0;
		end
		else if ( instr[31:26] == 6'd4 ) begin
			check <= 2;
			en_pc <= 0;
		end

		else if ( instr[31:26] == 6'd2 ) begin
			check <= 2;
			en_pc <= 0;
		end
		else if ( check == 0 ) begin
			check <= 0;	
			en_pc <= 1;
		end		
	
	end
	
	always @ ( posedge clk ) begin
	    if ( rst ) begin
	        count <= 0;
	   	    check <= 0;
			en_pc <= 1;
			hazard <= 0;
	    end
	    else begin
		    if ( count == 1'b0 && en_pc == 1'b1 ) hazard <= 0;
			else hazard <= 1;
			
	        count <= count + 1;
	        if ( count == check ) begin
	          en_pc <= 1;
		      count <= 0;
			  check <= 0;
	        end	
	    end	
	end


endmodule