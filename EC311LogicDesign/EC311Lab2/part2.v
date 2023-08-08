`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/28/2023 03:04:30 PM
// Design Name: 
// Module Name: part2
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module part2(T,X,Y,Z,S0,S1,clk);
output reg T;
input X,Y,Z,S0,S1,clk;
wire Q1, Q2, Q3, w1, w2, D, J, K, w3, w4, r, w5;
wire D0,D1,D2,D3;
wire [1:0]sel;

xor G1(w1, ~Q1, X);
and G2(w2, Q1, X);
or G3(D, w1, w2);

nand G4(w3, Q1, Y);
and G5(J, w3, Q2);

xor G6(w4, Y, Z);
nand G7(K, w4, w4);

synch_reset_DFF DFF1(.Q(Q1),.D(D),.r(r),.clk(clk));

TFF TFF1(.Q(Q2),.T(J),.r(r),.clk(clk));

JKFF JKFF1(.Q(Q3),.J(J),.K(K),.r(r),.clk(clk));

assign D0 = Q1;

xor G8(w5, Q2, Q3);
xor G9(D1, Q2, w5);

nand G10(D2, Q3, Q3);

assign D3 = 1'b0;

assign sel[0] = S0;
assign sel[1] = S1;

always @(sel)
begin
case(sel)
2'b00 : T = D0;
2'b01 : T = D1;
2'b10 : T = D2;
2'b11 : T = D3;
endcase
end

endmodule
