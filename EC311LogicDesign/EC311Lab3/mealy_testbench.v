`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/05/2023 04:59:20 PM
// Design Name: 
// Module Name: mealy_testbench
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


module mealy_testbench();
wire aout;
reg clk, rst, ain;

mealy mealy1(.aout(aout),.clk(clk),.rst(rst),.ain(ain),.state(state));

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
    ain = 0;
    #10 ain = 1;
    #10 ain = 1;
    #10 ain = 0;
    #10 ain = 0;
    #10 ain = 0;
    #10 ain = 1;
    #10 ain = 0;
    #10 ain = 1;
end

   
endmodule
