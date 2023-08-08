`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/15/2023 05:38:30 PM
// Design Name: 
// Module Name: synch_rest_DFF_testbench
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


module synch_rest_DFF_testbench(
        );
    
    reg D, clk, r;
    wire Q;
    
    synch_reset_DFF DFF2(.D(D),.clk(clk),.r(r),.Q(Q));
    
    initial begin
    r = 0;
    forever begin
    #20 r = ~r;
    end end
    
    
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
