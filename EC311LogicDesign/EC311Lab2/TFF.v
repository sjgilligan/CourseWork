`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/22/2023 04:41:28 PM
// Design Name: 
// Module Name: TFF
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


module TFF(Q,T,r,clk);
    output reg Q;
    input T,clk,r;
    
    /*initial
    begin
        Q = 1;
    end*/  
    
    always @(posedge clk)
    begin
        if(T == 0 && r == 0)
        Q = Q;
        else if(T == 1 && r == 0)
        Q = ~Q;
        else
        Q = 0;
    end
       
          
endmodule
