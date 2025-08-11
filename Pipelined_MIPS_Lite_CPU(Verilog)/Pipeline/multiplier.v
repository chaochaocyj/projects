module Multiplier (clk,reset,dataA,dataB,Signal,dataOut);
    input clk;
    input reset;
    input[31:0] dataA;  // 被乘數
    input[31:0] dataB;  // 乘數
    input[5:0] Signal;
    output [63:0] dataOut ;  // 輸出積



  reg [63:0] multiplicand;
  reg [31:0] multiplier;
  reg [63:0] result ;
  reg [6:0]  cal;
  parameter MULT = 6'b011001; // MULT:25
  parameter MADDU = 6'b000001; // MADDU
  parameter OUT = 6'b111111;

  always@( posedge clk or reset ) //  posedge = positive edge.    clk positive or reset == 1, in always 
  begin
      if ( reset ) //reset = 1    重置
      begin
        result = 64'b0 ;
        multiplicand = 64'b0;
        multiplier = 32'b0;
        cal = 0 ;
      end

      else
      begin
      // wire只存單個時序的東西
      
        if ( Signal == MULT || Signal == MADDU )
        begin
          if ( cal == 0 )
          begin 
            multiplicand <= dataA; // wire dataA assign 給register  multiplicand
            multiplier <= dataB;
            cal = cal +1;
          end

          else if (cal <= 32)
          begin   
            if(multiplier[0] == 1'b1) // 乘數最後一位為1
              begin
                result <= result + multiplicand ;
              end
              multiplicand <= multiplicand << 1; // 左移
              multiplier <= multiplier >> 1; // 右移
              cal = cal+1;
            end
            else
            begin
              cal = 0 ;
            end
          end
        end

    
  end


  assign dataOut = result ;


endmodule