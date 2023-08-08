`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/15/2023 04:54:48 PM
// Design Name: 
// Module Name: DFF_testbench
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


module DFF_testbench(

    );
    
    reg D, clk;
    wire Q;
    
    DFF DFF1(.D(D),.clk(clk),.Q(Q));
    
    initial begin
    D = 0;
    forever begin
    #50 D = ~D;
    end end
    
    
    initial begin 
    clk = 0;
    forever begin
    #10 clk = ~clk;   
 end end
endmodule
