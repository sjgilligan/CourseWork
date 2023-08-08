`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/05/2023 05:14:40 PM
// Design Name: 
// Module Name: moore
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


module moore(aout,clk,rst,ain,state);
output aout;
input clk, rst;
input [1:0]ain;
output reg state = 1'b0;

always @(posedge clk)
begin
    if(rst == 1)
        state = 1'b0;
    else
    case(ain)
        2'b00 : state <= state;
        2'b01 : state <= 1'b0;
        2'b10 : state <= ~state;
        default: state <= 1'b1;
    endcase
end

assign aout = state; 
endmodule
