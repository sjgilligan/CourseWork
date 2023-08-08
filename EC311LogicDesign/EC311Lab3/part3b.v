`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/11/2023 02:38:53 PM
// Design Name: 
// Module Name: part3b
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


module part3b(out,count,clk,rst);
output [5:0]out;
wire clean1;
input count,clk,rst;

debouncer db1(.clk(clk),.PB(count),.clean(clean1));

fib_counter fib1(.out(out),.count(clean1),.clk(clk),.rst(rst));
endmodule
