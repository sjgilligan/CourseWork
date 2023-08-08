`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 02/28/2023 03:02:44 PM
// Design Name: 
// Module Name: behavioral_adder_subtractor_testbench
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


module behavioral_adder_subtractor_testbench(
    
    );
    
    reg [3:0] A;
    reg [3:0] B;
    reg M;
    wire [4:0] S;
    
    behavioral_adder_subtractor bas(.A(A),.B(B),.M(M),.S(S));
    
    initial begin
        A = 4'b0; B = 4'b0; M = 1'b0;
    end
    always begin
        #10 {A,B,M} = {A,B,M} + 4'b1; 
    end
endmodule
