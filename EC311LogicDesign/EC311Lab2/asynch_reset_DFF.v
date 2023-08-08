`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/15/2023 05:49:12 PM
// Design Name: 
// Module Name: asynch_reset_DFF
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


module asynch_reset_DFF(Q,D,r,clk);
    output reg Q;
    input D, clk, r;
    
    
    always @(posedge clk or posedge r)
    begin
     Q <= D;
     if(r == 1)
        Q <= 0;
        else if(r == 0)
        Q <= 1;
    end   
endmodule
   
