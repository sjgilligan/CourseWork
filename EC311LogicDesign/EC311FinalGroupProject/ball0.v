`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/26/2023 11:07:03 AM
// Design Name: 
// Module Name: ball0
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


module ball0(ball0,player1,pause);
input ball0;
input player1;
output pause;
reg pause1;

always @ (posedge ball0) begin
    if(!player1)begin
        pause1 = 1;
    end else begin
        pause1 = 0;
    end
end

assign pause = pause1;
endmodule
