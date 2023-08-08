`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 02/15/2023 04:48:51 PM
// Design Name: 
// Module Name: half_adder_testbench
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


module half_adder_testbench(

    );
    
    reg A, B;
    wire S, C;
    
    half_adder ha(.A(A),.B(B),.S(S),.C(C));
    
    initial
    begin
        A = 0;
        B = 0;
        
        #100 A = 0; B = 1;
        #100 A = 1; B = 0;
        #100 A = 1; B = 1;
        #100 $finish;
    end
endmodule
