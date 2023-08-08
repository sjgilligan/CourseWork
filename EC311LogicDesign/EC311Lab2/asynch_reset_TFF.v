`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/22/2023 05:10:10 PM
// Design Name: 
// Module Name: asynch_reset_TFF
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


module asynch_reset_TFF(Q,T,r,clk);
    output reg Q;
    input T,clk,r;
    
    initial
    begin
        Q = 1;
    end  
    
    always @(posedge clk or posedge r)
    begin
        if(T == 0 && r == 0)
        Q = Q;
        else if(T == 1 && r == 1)
        Q = ~Q;
        else
        Q = 0;
        end
endmodule
