`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/15/2023 05:21:21 PM
// Design Name: 
// Module Name: synch_reset_DFF
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


module synch_reset_DFF(Q,D,r,clk);
    output reg Q;
    input D, clk, r;
    
    always @(posedge clk)
    begin
     Q <= D;
     if(r == 1)
        Q <= 0;
    end   
endmodule

