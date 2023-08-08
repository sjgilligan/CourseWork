`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/28/2023 02:29:11 PM
// Design Name: 
// Module Name: asynch_reset_JKFF
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


module asynch_reset_JKFF(Q,J,K,r,clk);
    output reg Q;
    input J,K,r,clk;
    reg [2:0]JKQ;
    //reg [1:0] JK;
    reg D;
    wire Q1;
    
    asynch_reset_DFF DFF2(.Q(Q1),.D(D),.r(r),.clk(clk));
    
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
endmodule
