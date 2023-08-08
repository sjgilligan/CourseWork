`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/29/2023 03:47:59 PM
// Design Name: 
// Module Name: part1_b
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


module part1_b(Q,divided_clk,D,clk,rst);
output Q, divided_clk;
input D, clk, rst;

clk_divider cd1(.divided_clk(divided_clk),.clk_in(clk),.rst(rst));

DFF DFF1(.Q(Q),.D(D),.clk(divided_clk));

endmodule

