`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04/20/2023 12:43:49 PM
// Design Name: 
// Module Name: tennis_main
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


module tennis_main(ball,clk,player1,player2,rst,AN_Out,C_Out);
input rst;
input player1;
input player2;
input clk;
output [15:0] ball;
wire div_clk;
wire clean_p1;
wire clean_p2;
reg [27:0] speed = 27'd30000000;
reg turn = 0; //0 = p1, 1 = p2
reg game_paused = 1;

//parameter ZERO = 7'b0111111, ONE = 7'b0000110, TWO=7'b1101101, THREE = 7'b1111001, FOUR = 7'b0110011, FIVE = 7'b1011011, SIX = 7'b101111, SEVEN = 7'b1110000, EIGHT = 7'b1111111, NINE = 7'b111011, P=7'b1110011;
parameter ZERO = 7'b0111111, ONE = 7'b0000110, TWO=7'b1011011, THREE = 7'b1001111, FOUR = 7'b1100110, FIVE = 7'b1101101, SIX = 7'b1111101, SEVEN = 7'b0000111, EIGHT = 7'b1111111, NINE = 7'b1100111, P=7'b1110011; 
reg [6:0] p1_score = 0;
reg [6:0] p2_score = 0;
reg [6:0] p1_display = ZERO;
reg [6:0] p2_display = ZERO;

//display
output [7:0] AN_Out;
output [6:0] C_Out;
reg [7:0] AN_In;
reg [55:0] C_In;

//push 3 time checkers
reg [1:0] p1_counter;
reg [1:0] p2_counter;
reg p1_counter_rst;
reg p2_counter_rst;


debouncer debouncer1(.clk(clk),.PB(player1),.clean(clean_p1));
debouncer debouncer2(.clk(clk),.PB(player2),.clean(clean_p2));

clk_divider1 clk_divider11(.clk_in(clk),.divided_clk(div_clk),.speed(speed));

counter1 counter11(.rst(ball_rst),.out(ball),.clk(div_clk),.turn(turn),.pause(game_paused));

SevenSegmentLED SevenSegmentLED(.clk(clk),.rst(rst),.AN_In(AN_In),.C_In(C_In),.AN_Out(AN_Out),.C_Out(C_Out));
//checks to see if swing was valid, if not other player gets point

//ball0 ball0(.ball0(ball[0]),.player1(player1),.pause(game_paused));

always @ (posedge clk)
begin

    case(p1_score)
    7'd0 : p1_display <= ZERO;
    7'd1 : p1_display <= ONE;
    7'd2 : p1_display <= TWO;
    7'd3 : p1_display <= THREE;
    7'd4 : p1_display <= FOUR;
    7'd5 : p1_display <= FIVE;
    7'd6 : p1_display <= SIX;
    7'd7 : p1_display <= SEVEN;
    7'd8 : p1_display <= EIGHT;
    7'd9 : p1_display <= NINE;
    endcase
    
    case(p2_score)
    7'd0 : p2_display <= ZERO;
    7'd1 : p2_display <= ONE;
    7'd2 : p2_display <= TWO;
    7'd3 : p2_display <= THREE;
    7'd4 : p2_display <= FOUR;
    7'd5 : p2_display <= FIVE;
    7'd6 : p2_display <= SIX;
    7'd7 : p2_display <= SEVEN;
    7'd8 : p2_display <= EIGHT;
    7'd9 : p2_display <= NINE;
    endcase
    
    if(p1_score < 3 && p2_score < 3) begin
        AN_In <= 8'b11100111;
        C_In <= {P,TWO,p2_display,7'd0,7'd0,P,ONE,p1_display};
    end else if(p1_score == 3) begin
        AN_In <= 8'b00011000;
        C_In <= {7'd0,7'd0,7'd0,P,ONE,7'd0,7'd0,7'd0};
    end else if(p2_score == 3) begin
        AN_In <= 8'b00011000;
        C_In <= {7'd0,7'd0,7'd0,P,TWO,7'd0,7'd0,7'd0};
    end
end

always @ (posedge div_clk) begin
    if(ball[0] && !player1 && !game_paused)begin
        p2_score = p2_score + 1;
        game_paused = 1;
        turn = ~turn;
    end
    
    if(ball[15] && !player2 && !game_paused)begin
        p1_score = p1_score + 1;
        game_paused = 1;
        turn = ~turn;
    end
    
    if(ball[0] && player1 && !game_paused)begin
        speed = speed - 28'd2500000;
    end
    
    if(ball[15] && player2 && !game_paused)begin
        speed = speed - 28'd2500000;
    end
    
    if(game_paused && player1 && !turn)begin
        game_paused = 0;
        speed = 27'd30000000;
        p1_counter_rst = 0;
        p2_counter_rst = 0;
    end
    
    if(game_paused && player2 && turn)begin
        game_paused = 0;
        speed = 27'd30000000;
        p1_counter_rst = 0;
        p2_counter_rst = 0;
    end
    
    if(p1_score == 3 && (player1 || player2)) begin
        game_paused = 1;
        p1_score = 0;
        p2_score = 0;
        turn = 1;
        speed = 27'd30000000;
    end
    
    if(p2_score == 3 && (player1 || player2)) begin
        game_paused = 1;
        p1_score = 0;
        p2_score = 0;
        turn = 0;
        speed = 27'd30000000;
    end
    
    if(p1_counter == 3)begin
        game_paused = 1;
        p2_score = p2_score + 1;
        turn = 1;
        p1_counter_rst = 1;
    end
    
    if(p2_counter == 3)begin
        game_paused = 1;
        p1_score = p1_score + 1;
        turn = 0;
        p2_counter_rst = 1;
    end
end


always @ (posedge clean_p1 or posedge p1_counter_rst)begin
    if(!ball[0])begin
        p1_counter = p1_counter + 1;
    end
    
    if(p1_counter_rst)begin
        p1_counter = 0;
    end
end

always @ (posedge clean_p2 or posedge p2_counter_rst)begin
    if(!ball[15])begin
        p2_counter = p2_counter + 1;
    end
    
    if(p2_counter_rst)begin
        p2_counter = 0;
    end
end
    
endmodule