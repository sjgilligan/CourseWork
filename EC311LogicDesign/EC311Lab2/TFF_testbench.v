`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/22/2023 04:51:11 PM
// Design Name: 
// Module Name: TFF_testbench
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


module TFF_testbench(

    );
    
    reg T, clk, r;
    wire Q;
    
    TFF TFF1(.T(T),.clk(clk),.r(r),.Q(Q));
    
    initial begin
    r = 0;
    forever begin
    #100 r = ~r;
    end end
    
    initial begin
    T = 0;
    forever begin
    #50 T = ~T;
    end end
    
    
    initial begin 
    clk = 0;
    forever begin
    #10 clk = ~clk;   
 end end
endmodule
