`timescale 1ns / 1ps

module SevenSegmentLED(clk,rst,AN_In,C_In,AN_Out,C_Out);

input clk,rst;
input [55:0] C_In;
input [7:0] AN_In;
output reg [7:0] AN_Out;
output reg [6:0] C_Out;

reg [2:0] LEDCounter;

reg [16:0] clkCounter;
reg slowClk;
parameter TOGGLE = 17'd100000;

always @ (posedge slowClk or posedge rst)
begin
    if(rst)
    begin
        AN_Out <= 0;
        C_Out <= 0;
        LEDCounter <= 0;
    end
    else
    begin
        LEDCounter <= LEDCounter + 1;
        AN_Out <= ~((8'd1 << LEDCounter)&AN_In);
        C_Out <= ~C_In[LEDCounter*7+:7];
    end
end

always @ (posedge clk or posedge rst)
begin
    if(rst)
    begin
        clkCounter <= 0;
        slowClk <= 0;
    end
    else
    begin
        if(clkCounter == TOGGLE)
        begin
            slowClk <= 0;
            clkCounter <= 0;
        end
        else if(clkCounter > TOGGLE/2)
        begin
            slowClk <= 1;
            clkCounter <= clkCounter + 1;
        end
        else
        begin
            slowClk <= 0;
            clkCounter <= clkCounter + 1;
        end
    end
end

endmodule
