`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/12/2023 04:16:46 PM
// Design Name: 
// Module Name: debouncer_testbench
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


module debouncer_testbench();
wire clean;
reg clk,PB;

debouncer db1(.clean(clean),.clk(clk),.PB(PB));

initial begin
    clk = 0;
end
always begin
    #10 clk = ~clk;
end

initial begin
    PB = 0;
end
always begin
    #100 PB = ~PB;
end




endmodule
