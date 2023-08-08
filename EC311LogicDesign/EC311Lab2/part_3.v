`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/29/2023 05:02:20 PM
// Design Name: 
// Module Name: part_3
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


module part_3(Q,T,tff_rst,clk_rst,clk,clk_out);
output Q,clk_out;
input T,tff_rst,clk_rst,clk;

clk_divider_part3 cd1(.divided_clk(clk_out),.clk_in(clk),.rst(clk_rst));

TFF TFF1(.Q(Q),.T(T),.r(tff_rst),.clk(clk_out));


endmodule
