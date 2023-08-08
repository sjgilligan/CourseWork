`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/22/2023 05:45:13 PM
// Design Name: 
// Module Name: JKFF
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
//module synch_reset_DFF(input

module JKFF(Q,J,K,r,clk);
    output reg Q;
    input J,K,r,clk;
    reg [2:0]JKQ;
    //reg [1:0] JK;
    reg D;
    wire Q1;
    
    synch_reset_DFF DFF1(.Q(Q1),.D(D),.r(r),.clk(clk));
    
    initial
    begin
        Q <= 0;
    end
    
    always @ (Q1)
    begin
        Q <= Q1;
    end
    
    always @ (J,K,Q)
    begin
        JKQ[0] = Q;
        JKQ[1] = K;
        JKQ[2] = J;
        case(JKQ)
        3'b000 : D = 0;
        3'b001 : D = 1;
        3'b010 : D = 0;
        3'b011 : D = 0;
        3'b100 : D = 1;
        3'b101 : D = 1;
        3'b110 : D = 1;
        3'b111 : D = 0;
        default : D = 0;
        endcase
    end
    
    /*always @ (posedge clk)
    begin
        JK[0] = K;
        JK[1] = J;
        case(JK)
        2'b00 : Q = Q;
        2'b01 : Q = 0;
        2'b10 : Q = 1;
        2'b11 : Q = ~Q;
        endcase
     end*/
      
        
    
    

endmodule
