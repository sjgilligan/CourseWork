`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/20/2023 01:06:02 PM
// Design Name: 
// Module Name: counter1
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


module counter1(turn,rst,out,clk,pause);
input rst;
input pause;
input turn;
input clk;
output [15:0] out;
reg [4:0] count = 0;
reg up = 1;

always @ (posedge clk)
begin
    if(!pause)begin
        if(up)begin
            count = count + 1;
        end else if(!up)begin   
            count = count - 1;
        end    
        if(count == 5'd15 || count == 5'd0) begin
            up = ~up;
        end
    end else begin
        if(!turn)begin
            count = 0;
            up = 1;
        end else begin
            count = 15;
            up = 0;
        end 
    end
end

assign out = (2**count);

endmodule
