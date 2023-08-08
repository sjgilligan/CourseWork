`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////

// Module Name:    clk_divider 

// Dependencies: Generates a clock with period X ms, from 40 MHz input clock, T=2.5E-5 ms
//						A counter counts till toggle_value = 'b111111111111111111111

// You have to decide what should be the value of toggle_value to be able make the slow down
// the clock to 1 Hz. X ms will be depend on your toggle_value choice.
//////////////////////////////////////////////////////////////////////////////////
module clk_divider(
	input clk_in,
	input rst,
	output reg divided_clk
    );
	 
	 
parameter toggle_value = 21'b111111111111111111111;

	 
reg[20:0] cnt;

always@(posedge clk_in or posedge rst)
begin
	if (rst==1) begin
		cnt <= 0;
		divided_clk <= 0;
	end
	else begin
		if (cnt==toggle_value) begin
			cnt <= 0;
			divided_clk <= ~divided_clk;
		end
		else begin
			cnt <= cnt +1;
			divided_clk <= divided_clk;		
		end
	end

end
			  
	


endmodule
