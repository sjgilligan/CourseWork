`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 02/15/2023 05:45:08 PM
// Design Name: 
// Module Name: four_bit_adder_testbench
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


module four_bit_adder_testbench(

    );
    
    reg [3:0] A;
    reg [3:0] B;
    wire [3:0] S;
    wire C, V;
    
    //four_bit_adder fba(.A(A[0]), .A(A[1]), .A(A[2]), .A(A[3]) ,.B(B[0]), .B(B[1]), .B(B[2]), .B(B[3]), .S(S[0]), .S(S[1]), .S(S[2]), .S(S[3]), .C(C), .V(V));
    
    four_bit_adder fba(.A(A),.B(B),.S(S),.C(C),.V(V));
    initial begin
        //A[0] = 0; A[1] = 0; A[2] = 0; A[3] = 0; B[0] = 0; B[1] = 0; B[2] = 0; B[3] = 0; // assign initial values
        A = 4'b0; B = 4'b0;
    end
    always begin
        #10 {A,B} = {A,B} + 4'b1;
    end
endmodule
