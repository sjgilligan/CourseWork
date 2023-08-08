`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/20/2023 12:58:31 PM
// Design Name: 
// Module Name: counter_test
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


module counter_test();
reg start;
reg clk;
wire [15:0] out;

counter1 counter11(.out(out),.clk(clk),.start(start));

initial begin
    clk = 1;
end
always begin
    #10 clk = ~clk;
end

endmodule
