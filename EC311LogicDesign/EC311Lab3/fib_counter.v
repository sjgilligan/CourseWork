`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/05/2023 05:46:03 PM
// Design Name: 
// Module Name: fib_counter
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


module fib_counter(out,count,clk,rst);
output reg [5:0]out = 6'b000000;
input count,clk,rst;
reg [5:0]next = 6'b000000;
reg [5:0]t1 = 6'b000000;
reg [5:0]t2 = 6'b000001;



always @(posedge clk or posedge rst)
begin
if(rst == 1'b1)begin
    out = 6'b000000;
    next = 6'b000000;
    t1 = 6'b000000;
    t2 = 6'b000001;
end
else if(count == 1'b1)begin
    if(out < 6'd34)begin
        out = next;
        t1 = t2;
        t2 = next;
        next = t1 + t2;
    end
    else begin
        out = 6'b000000;
        next = 6'b000000;
        t1 = 6'b000000;
        t2 = 6'b000001;
        
    end
end    
end
    
endmodule
