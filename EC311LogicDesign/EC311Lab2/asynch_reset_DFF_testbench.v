`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/15/2023 05:52:06 PM
// Design Name: 
// Module Name: asynch_reset_DFF_testbench
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


module asynch_reset_DFF_testbench(
    );
    
    reg D, clk, r;
    wire Q;
    
    asynch_reset_DFF DFF3(.D(D),.clk(clk),.r(r),.Q(Q));
    
    initial begin
    r = 0;
    forever begin
    #13 r = ~r;
    end end
    
    
    initial begin
    D = 0;
    forever begin
    #23 D = ~D;
    end end
    
    
    initial begin 
    clk = 0;
    forever begin
    #9 clk = ~clk;   
 end end
endmodule
