`timescale 1ns / 1ps

module counter(out,clk,start);
input start;
input clk;
output [15:0] out;
reg [4:0] count = 0;

always @ (posedge clk)
begin
    if(count < 5'd16)begin
        count = count + 1;

    end else begin
        count = 16'd0;
    end
end

assign out = (2**count);

endmodule