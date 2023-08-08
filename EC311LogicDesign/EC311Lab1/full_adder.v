`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 02/15/2023 05:03:14 PM
// Design Name: 
// Module Name: full_adder
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

module full_adder(
    input Cin,
    input A,
    input B,
    output S,
    output Cout
    );
    
    wire w1;
    wire w2;
    wire w3;
    wire w4;
    
    half_adder G1(A, B, w1, w2);
    half_adder G2(Cin, w1, S, w4);
    or G3(Cout, w2, w4);
endmodule
