`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/24/2023 03:34:46 PM
// Design Name: 
// Module Name: tennis_test
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


module tennis_test();
reg rst;
reg player1;
reg player2;
reg clk;
wire [15:0] ball;
wire [7:0] AN_Out;
wire [6:0] C_Out;
wire [6:0] p1_display;
wire [6:0] p2_display;
wire p1_out;

tennis_main tennis_main1(.ball(ball),.clk(clk),.player1(player1),.player2(player2),.rst(rst),.AN_Out(AN_Out),.C_Out(C_Out),.p1_display(p1_display),.p2_display(p2_display));
debouncer debouncer1(.clk(clk),.PB(player1),.clean(p1_out));

initial begin
clk = 1;
end
always begin
#1 clk = ~clk;
end 

initial begin
player1 = 10;
end
always begin
#5 player1 = ~player1;
end 

initial begin
player2 = 10;
end
always begin
#7 player2 = ~player2;
end 


endmodule
