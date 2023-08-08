`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/05/2023 05:37:27 PM
// Design Name: 
// Module Name: moore_testbench
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


module moore_testbench();
wire aout;
reg clk, rst;
reg [1:0]ain;

moore moore1(.aout(aout),.clk(clk),.rst(rst),.ain(ain),.state(state));

initial begin
    rst = 0;
end

initial begin
    clk = 0;
end
always begin
    #1 clk = ~clk;
end

initial begin
    ain = 00;
    #10 ain = 01;
    #10 ain = 10;
    #10 ain = 11;
    #10 ain = 11;
    #10 ain = 00;
    #10 ain = 10;
    #10 ain = 10;
    #10 ain = 01;
    #10 ain = 00;
end

   
endmodule

