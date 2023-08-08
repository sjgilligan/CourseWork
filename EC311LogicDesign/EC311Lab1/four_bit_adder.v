`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 02/15/2023 05:36:56 PM
// Design Name: 
// Module Name: four_bit_adder
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


module four_bit_adder(
    input [3:0] A,
    input [3:0] B,
    output [3:0] S,
    output C,
    output V
    );
    
    wire c1; 
    wire c2;
    wire c3; 
    //wire c4;
    
    full_adder G1(0,A[0],B[0],S[0],c1);
    full_adder G2(c1,A[1],B[1],S[1],c2);
    full_adder G3(c2,A[2],B[2],S[2],c3);
    full_adder G4(c3,A[3],B[3],S[3],C);
    xor G5(V,C,c3);
    
endmodule
