`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/28/2023 02:34:30 PM
// Design Name: 
// Module Name: both_JKFF
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


module both_JKFF(Q_syn,Q_asyn,J,K,r,clk);
output reg Q_syn, Q_asyn;
input J,K,r,clk;
wire Q_syn1, Q_asyn1;

JKFF JKFF1(.Q(Q_syn1),.J(J),.K(K),.r(r),.clk(clk));
asynch_reset_JKFF JKFF2(.Q(Q_asyn1),.J(J),.K(K),.r(r),.clk(clk));

always @ (Q_syn1,Q_asyn1)
begin
Q_syn = Q_syn1;
Q_asyn = Q_asyn1;
end

endmodule
