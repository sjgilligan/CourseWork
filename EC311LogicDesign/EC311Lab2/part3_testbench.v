`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/29/2023 05:09:13 PM
// Design Name: 
// Module Name: part3_testbench
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


module part3_testbench();
wire Q, clk_out;
reg T, tff_rst, clk_rst, clk;

part_3 p3(.Q(Q),.clk_out(clk_out),.T(T),.tff_rst(tff_rst),.clk_rst(clk_rst),.clk(clk));


initial begin
    T = 0;
    forever begin
    #50 T = ~T;
    end end
    
    initial begin
    tff_rst = 0;
    forever begin
    #30 tff_rst = ~tff_rst;
    end end
    
    initial begin
    clk_rst = 1;
    #1 clk_rst = 0;
    end
    //forever begin
    //#10 clk_rst = ~clk_rst;
    //end end
    
    initial begin
    clk = 0;
    forever begin
    #1 clk = ~clk;
    end end
    
    
    
    
endmodule
