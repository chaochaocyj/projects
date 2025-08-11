module TotalALU( clk, ALUop, dataA, dataB, Funct, Output, reset, check, extend_SHT );
input reset ;
input clk ;
input [1:0] ALUop;
input [31:0] dataA ;
input [31:0] dataB ;
input [5:0] Funct ;
input [6:0] check ;
input [4:0] extend_SHT ;
output [31:0] Output ;

//   Funct ( 6-bits)
//   AND  : 36
//   OR   : 37
//   ADD  : 32
//   SUB  : 34
//   SRL  : 2
//   SLT  : 42
//   MULT : 25

wire [31:0] temp ;

parameter AND = 6'b100100;
parameter OR  = 6'b100101;
parameter ADD = 6'b100000;
parameter SUB = 6'b100010;
parameter SLT = 6'b101010;
parameter SRL = 6'b000010;
parameter MULT= 6'b011001;
parameter MFHI= 6'b010000;
parameter MFLO= 6'b010010;

//============================
wire [2:0]  SignaltoALU ;
wire [5:0]  SignaltoSHT ;
wire [5:0]  SignaltoMULT ;
wire add ;
wire [1:0]  SignaltoMUX ;
wire [31:0] ALUOut, HiOut, LoOut, ShifterOut ;
wire [31:0] dataOut ;
wire [63:0] MultAns ;

alu_ctl alu_ctl( .clk(clk), .ALUop(ALUop), .Funct(Funct), .SignaltoALU(SignaltoALU), .SignaltoSHT(SignaltoSHT), .SignaltoMULTU(SignaltoMULT), .SignaltoMUX(SignaltoMUX), .check(check), .add (add));
ALU ALU( .dataA(dataA), .dataB(dataB), .ctl(SignaltoALU), .dataOut(ALUOut), .reset(reset) );

Multiplier Multiplier( .clk(clk), .dataA(dataA), .dataB(dataB), .Signal(SignaltoMULT), .dataOut(MultAns), .reset(reset) );
Shifter Shifter( .dataA(dataB), .dataB(extend_SHT), .Signal(SignaltoSHT), .dataOut(ShifterOut), .reset(reset) ); // 要反接

HiLo HiLo( .clk(clk), .MultAns(MultAns), .HiOut(HiOut), .LoOut(LoOut), .reset(reset), .Signal(SignaltoMULT), .add(add));
MUX MUX( .ALUOut(ALUOut), .HiOut(HiOut), .LoOut(LoOut), .Shifter(ShifterOut), .Signal(SignaltoMUX), .dataOut(dataOut) );

assign Output = dataOut ;


endmodule