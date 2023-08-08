`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 02/15/2023 05:14:06 PM
// Design Name: 
// Module Name: full_adder_testbench
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


module full_adder_testbench(

    );
    
    reg A, B, Cin;
    wire S, C;
    
    full_adder fa(.A(A),.B(B),.Cin(Cin),.S(S),.Cout(Cout));
    
    initial
    begin
        A = 0;
        B = 0;
        Cin = 0;
        
        #100 A = 0; B = 0; Cin = 1;
        #100 A = 0; B = 1; Cin = 0;
        #100 A = 0; B = 1; Cin = 1;
        #100 A = 1; B = 0; Cin = 0;
        #100 A = 1; B = 0; Cin = 1;
        #100 A = 1; B = 1; Cin = 0;
        #100 A = 1; B = 1; Cin = 1;
        #100 $finish;
    end
endmodule
