`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/11/2023 02:34:41 PM
// Design Name: 
// Module Name: debouncer
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


module debouncer(clk,PB,clean);
input clk,PB;
output reg clean;
reg [19:0] deb_count;
reg output_exists;

initial begin
    clean = 0;
    deb_count = 19'd0;
    output_exists = 0;
end

always @ (posedge clk)
begin
    if (PB == 1)begin
        if(output_exists == 0) begin
            if (&deb_count) begin
                clean = 1;
                deb_count = 19'd0;
                output_exists = 1;
            end else begin
                deb_count = deb_count + 19'd1;
            end
        end else begin
            clean = 0;
        end
    end else begin
        clean = 0;
        deb_count = 0;
        output_exists = 0;
    end
end
endmodule