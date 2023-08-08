`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/11/2023 01:27:38 PM
// Design Name: 
// Module Name: part3_a
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


module part3_a(out,count,clk,rst);
output [5:0]out;
wire clk_out;
input count,clk,rst;


part3a_clkdivider clkdiv(.divided_clk(clk_out),.rst(rst),.clk_in(clk));

fib_counter fib1(.out(out),.count(count),.clk(clk_out),.rst(rst));
endmodule
