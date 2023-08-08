`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/05/2023 04:42:02 PM
// Design Name: 
// Module Name: mealy
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


module mealy(aout,clk,rst,ain,state);
output aout;
input clk, rst, ain;
output reg state = 1'b0;

always @(posedge clk)
begin
if(rst == 1'b1)
    state <= 1'b0;
else
    case(state)
        1'b0: state <= (ain == 1'b0) ? 2'b1 : 1'b0;
        1'b1: state <= 1'b0;
    endcase
end

assign aout = (state == 1'b1 && ain == 1'b1) ? 1'b1 : 1'b0;
endmodule
