`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/11/2023 12:57:01 PM
// Design Name: 
// Module Name: fib_counter_testbench
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


module fib_counter_testbench();
wire [5:0] out;
reg count,clk,rst;

fib_counter fib1(.out(out),.count(count),.clk(clk),.rst(rst));

initial begin
    count = 1;
    rst = 1;
    #1 rst = 0;
end

initial begin
    clk = 0;
end
always begin
    #1 clk = ~clk;
end


endmodule
