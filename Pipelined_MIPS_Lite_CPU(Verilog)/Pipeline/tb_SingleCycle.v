/*
	Title: MIPS Single Cycle CPU Testbench
	Author: Selene (Computer System and Architecture Lab, ICE, CYCU) 
*/
module tb_SingleCycle();
	reg clk, rst;
	
	// 設定要執行多少cycle 若執行不完請自行調整
	parameter cycle_count = 9 ;
	
	// 產生時脈，週期：10單位時間
	initial begin
		clk = 1;
		forever #5 clk = ~clk;
	end

	initial begin
		$dumpfile("mips_single.vcd");
		$dumpvars(0,tb_SingleCycle);
		rst = 1'b1;
		/*
			指令資料記憶體，檔名"instr_mem.txt, data_mem.txt"可自行修改
			每一行為1 Byte資料，以兩個十六進位數字表示
			且為Little Endian編碼
		*/
		$readmemh("instr_mem.txt", CPU.InstrMem.mem_array );
		$readmemh("data_mem.txt", CPU.DataMem.mem_array );
		// 設定暫存器初始值，每一行為一筆暫存器資料
		$readmemh("reg.txt", CPU.regFile.file_array );
		#10;
		rst = 1'b0;
		
	end

	
	always @( posedge clk ) begin
		$display( "%d, PC:", $time/10-1, CPU.pc );

		if (CPU.instr_ID == 32'b0 && ($time/10-1) != 0 )
			$display( "%d, NOP", $time/10-1 );
		else if ( CPU.opcode == 6'd0 ) 
		begin 
			$display( "%d, wd: %d", $time/10-1, CPU.regFile_WD );
			if ( CPU.funct == 6'd32 ) $display( "%d, ADD", $time/10-1 );
			else if ( CPU.funct == 6'd34 ) $display( "%d, SUB", $time/10-1 );
			else if ( CPU.funct == 6'd36 ) $display( "%d, AND", $time/10-1 );
			else if ( CPU.funct == 6'd37 ) $display( "%d, OR", $time/10-1 );
			else if ( CPU.funct == 6'd42 ) $display( "%d, SLT", $time/10-1 );
			else if ( CPU.funct == 6'd25 ) $display( "%d, MULTU", $time/10-1 );
			else if ( CPU.funct == 6'd16 ) $display( "%d, MFHI", $time/10-1 );
			else if ( CPU.funct == 6'd18 ) $display( "%d, MFLO", $time/10-1 );
			else if ( CPU.funct == 6'd2 && ($time/10-1) != 0 )
			begin
				if ( CPU.check_EX > 0 )
					$display( "%d, //BUBBLE//", $time/10-1 );
				else
					$display( "%d, SRL", $time/10-1 );
			end
		end
		else if ( CPU.opcode == 6'd35 ) $display( "%d, LW", $time/10-1 );
		else if ( CPU.opcode == 6'd43 ) $display( "%d, SW", $time/10-1 );
		else if ( CPU.opcode == 6'd4 ) $display( "%d, BEQ", $time/10-1 );
		else if ( CPU.opcode == 6'd2 ) $display( "%d, J", $time/10-1 );
		else if ( CPU.opcode == 6'd9 ) $display( "%d, ADDIU", $time/10-1 );
		else if ( CPU.opcode == 6'd28 ) $display( "%d, MADDU", $time/10-1 );
		
		$display( "\n" );
	end
	
	mips_pipeline CPU( clk, rst );
	
endmodule
