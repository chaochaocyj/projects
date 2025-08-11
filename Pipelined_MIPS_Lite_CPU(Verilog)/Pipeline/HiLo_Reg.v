module HiLo( clk, MultAns, HiOut, LoOut, reset, Signal, add );
input clk ;
input reset ;
input [5:0] Signal;
input [63:0] MultAns ;
input add;
output [31:0] HiOut ;
output [31:0] LoOut ;

reg [31:0] HiOut, LoOut;
reg [63:0] HiLo ;

parameter MULT = 6'b011001; // MULT:25
parameter MADDU = 6'b000001; // MADDU
parameter OUT = 6'b111111;

always@( posedge clk or reset )
begin
  if ( reset )
  begin
    HiLo = 64'b0 ;
  end

//reset訊號 如果是reset就做歸0

  else
  begin
  if ( Signal == OUT )begin
    if ( add == 1'b0 )
    begin
      
      HiLo = MultAns ;
    end
    else
    begin
      HiLo = MultAns + HiLo ;
    end
  end

//把傳入的乘法答案存起來
    end
end

always@( posedge clk )
begin
 HiOut <= HiLo[63:32] ;
 LoOut <= HiLo[31:0] ;
end



endmodule








