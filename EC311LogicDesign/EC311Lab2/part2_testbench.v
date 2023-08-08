`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/29/2023 02:34:53 PM
// Design Name: 
// Module Name: part2_testbench
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


module part2_testbench();
reg X,Y,Z,S0,S1,clk;
wire T;

part2 part2_1(.T(T),.X(X),.Y(Y),.Z(Z),.S0(S0),.S1(S1),.clk(clk));

    initial begin
    X = 0;
    forever begin
    #2 X = ~X;
    end end
    
    initial begin
    Y = 0;
    forever begin
    #3 Y = ~Y;
    end end
    
    initial begin
    Z = 0;
    forever begin
    #5 Z = ~Z;
    end end
    
    initial begin
    S0 = 0;
    forever begin
    #7 S0 = ~S0;
    end end
    
    initial begin
    S1 = 0;
    forever begin
    #11 S1 = ~S1;
    end end
    
    initial begin
    clk = 0;
    forever begin
    #1 clk = ~clk;
    end end
    
endmodule
