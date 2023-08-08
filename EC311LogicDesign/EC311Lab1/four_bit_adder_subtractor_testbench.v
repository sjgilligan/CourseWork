`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 02/28/2023 02:03:16 PM
// Design Name: 
// Module Name: four_bit_adder_subtractor_testbench
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


module four_bit_adder_subtractor_testbench(

    );
    
    reg [3:0] A;
    reg [3:0] B;
    reg M;
    wire [3:0] S;
    wire C, V;
    
    //four_bit_adder fba(.A(A[0]), .A(A[1]), .A(A[2]), .A(A[3]) ,.B(B[0]), .B(B[1]), .B(B[2]), .B(B[3]), .S(S[0]), .S(S[1]), .S(S[2]), .S(S[3]), .C(C), .V(V));
    
    four_bit_adder_subtractor fba(.A(A),.B(B),.M(M),.S(S),.C(C),.V(V));
    initial begin
        //A[0] = 0; A[1] = 0; A[2] = 0; A[3] = 0; B[0] = 0; B[1] = 0; B[2] = 0; B[3] = 0; // assign initial values
        A = 4'b0; B = 4'b0; M = 1'b0;
    end
    always begin
        #10 {A,B,M} = {A,B,M} + 4'b1;
    end
endmodule
