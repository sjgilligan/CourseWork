`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 02/28/2023 01:54:07 PM
// Design Name: 
// Module Name: four_bit_adder_subtractor
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


module four_bit_adder_subtractor(
    input [3:0] A,
    input [3:0] B,
    input M,
    output [3:0] S,
    output C,
    output V
    );
    wire c1; 
    wire c2;
    wire c3; 
    wire b0;
    wire b1;
    wire b2;
    wire b3;
    
    xor X1(b0,B[0],M);
    xor X2(b1,B[1],M);
    xor X3(b2,B[2],M);
    xor X4(b3,B[3],M);
        
    full_adder G1(M,A[0],b0,S[0],c1);
    full_adder G2(c1,A[1],b1,S[1],c2);
    full_adder G3(c2,A[2],b2,S[2],c3);
    full_adder G4(c3,A[3],b3,S[3],C);
    xor G5(V,C,c3);
endmodule
