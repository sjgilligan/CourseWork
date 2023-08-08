`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/29/2023 03:10:52 PM
// Design Name: 
// Module Name: clk_divider_testbench
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


module clk_divider_testbench();
reg clk, D, rst;
wire divided_clk, Q;

initial begin
rst = 1;
#1 rst = 0;
end

initial begin 
    clk = 0;
    forever begin
    #1 clk = ~clk;   
 end end
 
 initial begin 
    D = 0;
    forever begin
    #3 D = ~D;   
 end end
 
part1_b p1(.Q(Q),.divided_clk(divided_clk),.D(D),.clk(clk),.rst(rst));
 
endmodule
