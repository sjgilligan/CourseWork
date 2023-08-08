`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 02/28/2023 02:40:18 PM
// Design Name: 
// Module Name: behavioral_adder_subtractor
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


module behavioral_adder_subtractor(
    input [3:0] A,
    input [3:0] B,
    input M,
    output [4:0] S
    );
    
    assign S = M ? (A + (~B + 1)) : A + B;
    
endmodule
