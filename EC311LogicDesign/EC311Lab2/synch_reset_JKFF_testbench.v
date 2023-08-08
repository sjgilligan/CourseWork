`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/28/2023 01:30:08 PM
// Design Name: 
// Module Name: synch_reset_JKFF_testbench
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


module synch_reset_JKFF_testbench();
reg J, K, r, clk;
wire Q;

JKFF JKFF1(.Q(Q),.J(J),.K(K),.r(r),.clk(clk));

    initial begin
    r = 0;
    forever begin
    #13 r = ~r;
    end end
    
    
    initial begin
    J = 0;
    forever begin
    #2 J = ~J;
    end end
    
    initial begin
    K = 0;
    forever begin
    #5 K = ~K;
    end end
    
    
    initial begin 
    clk = 0;
    forever begin
    #1 clk = ~clk; 
    end end

endmodule
