`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////

// Module Name:    clk_divider 

// Dependencies: Generates a clock with period X ms, from 40 MHz input clock, T=2.5E-5 ms
//						A counter counts till toggle_value = 'b111111111111111111111

// You have to decide what should be the value of toggle_value to be able make the slow down
// the clock to 1 Hz. X ms will be depend on your toggle_value choice.
//////////////////////////////////////////////////////////////////////////////////
module clk_divider1(
	input clk_in,
	input rst,
	input [27:0] speed,
	output reg divided_clk
    );
    
//reg toggle_value;
	 
// toggle_value = 28'd40000000;
/*parameter toggle_value2 = 28'd45000000;
parameter toggle_value3 = 28'd40000000;
parameter toggle_value4 = 28'd35000000;
parameter toggle_value5 = 28'd30000000;
parameter toggle_value6 = 28'd25000000;
parameter toggle_value7 = 28'd20000000;
parameter toggle_value8 = 28'd15000000;
parameter toggle_value9 = 28'd10000000;
parameter toggle_value10 = 28'd5000000;*/


	 
reg[27:0] cnt;

/*always@(posedge speed[0])begin
case(speed)
    5'd0 : toggle_value = toggle_value1;
    5'd1 : toggle_value = toggle_value1;
    5'd2 : toggle_value = toggle_value1;
    5'd3 : toggle_value = toggle_value1;
    5'd4 : toggle_value = toggle_value1;
    5'd5 : toggle_value = toggle_value1;
    5'd6 : toggle_value = toggle_value1;
    5'd7 : toggle_value = toggle_value1;
    5'd8 : toggle_value = toggle_value1;
    5'd9 : toggle_value = toggle_value1;
    endcase
end*/

always@(posedge clk_in or posedge rst)
begin
	if (rst==1) begin
		cnt <= 0;
		divided_clk <= 0;
	end
	else begin
		if (cnt == speed) begin
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