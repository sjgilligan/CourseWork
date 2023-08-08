`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/22/2023 05:13:10 PM
// Design Name: 
// Module Name: asynch_reset_TFF_testbench
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


module asynch_reset_TFF_testbench( );
    
    reg T, clk, r;
    wire Q;
    
    asynch_reset_TFF TFF2(.T(T),.clk(clk),.r(r),.Q(Q));
    
    initial begin
    r = 0;
    forever begin
    #100 r = ~r;
    end end
    
    
    initial begin
    T = 0;
    forever begin
    #23 T = ~T;
    end end
    
    
    initial begin 
    clk = 0;
    forever begin
    #9 clk = ~clk;   
 end end
endmodule

