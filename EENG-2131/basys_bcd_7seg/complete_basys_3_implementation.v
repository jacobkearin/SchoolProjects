`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Dunwoody College of Technology
// Engineer: Jacob Kearin
// 
// Create Date: 09/29/2021 12:03:54 PM
// Design Name: 
// Module Name: bcd
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

//BCD to sevSeg module - really a Binary coded hexadecimal
module bcd_to_sevseg(BCD, segN);
input [3:0] BCD;
output [6:0] segN;

reg [6:0] segN;

always @(BCD)
begin
    if (BCD == 4'b0000) //0
        segN <= 7'b1000000;
    else if (BCD == 4'b0001) //1
        segN <= 7'b1111001;
    else if (BCD == 4'b0010) //2
        segN <= 7'b0100100;
    else if (BCD == 4'b0011) //3
        segN <= 7'b0110000;
    else if (BCD == 4'b0100) //4
        segN <= 7'b0011001;
    else if (BCD == 4'b0101) //5
        segN <= 7'b0010010;
    else if (BCD == 4'b0110) //6
        segN <= 7'b0000010;
    else if (BCD == 4'b0111) //7
        segN <= 7'b1111000;
    else if (BCD == 4'b1000) //8
        segN <= 7'b0000000;
    else if (BCD == 4'b1001) //9
        segN <= 7'b0010000;
    else if (BCD == 4'b1010) //A
        segN <= 7'b0001000;
    else if (BCD == 4'b1011) //B
        segN <= 7'b0000011;
    else if (BCD == 4'b1100) //C
        segN <= 7'b1000110;
    else if (BCD == 4'b1101) //D
        segN <= 7'b0100001;
    else if (BCD == 4'b1110) //E
        segN <= 7'b0000110;
    else if (BCD == 4'b1111) //F
        segN <= 7'b0001100;
    else
        segN <= 7'b1111111;
    end
endmodule
//digital segment controller module
module dig_cont(clk, an);
input clk;
output [3:0] an;
reg [3:0] an;
reg [31:0] counter0 = 0;
reg [31:0] counter1 = 0;

parameter refresh_ms = 16;
parameter num_digits = 4;
parameter rate = refresh_ms * 100000; //add *100,000 for 100,000,000 Mhz / 1,000 ms/s

always @(posedge (clk))
begin
    counter0 = counter0 + 1;
    if (counter0 >= (rate / num_digits))
    begin    
        counter0 = 0;
        counter1 = counter1 + 1;
    end    
    if (counter1 >= (num_digits))
        counter1 = 0;
end

always @(posedge(clk))
begin
    if (counter1 == 0)
            an = 4'b0111;
    else if (counter1 == 1)
            an = 4'b1011;
    else if (counter1 == 2)
            an = 4'b1101;
    else if (counter1 == 3)
            an = 4'b1110;
    else 
        an <= 4'b1111;
end
endmodule

//combination module
module bcd_seg_complete(CLK, BCD0, BCD1, BCD2, BCD3, AN, SEGN);
input CLK;
input [3:0] BCD0;
input [3:0] BCD1;
input [3:0] BCD2;
input [3:0] BCD3;
output [3:0] AN;
output [6:0] SEGN;

reg [3:0] BCD;

dig_cont (CLK, AN);
bcd_to_sevseg (BCD, SEGN);

always @(posedge CLK)
begin
    if (AN == 4'b1110)
        BCD = BCD0;
    else if (AN == 4'b1101)
        BCD = BCD1;
    else if (AN == 4'b1011)
        BCD = BCD2;
    else if (AN == 4'b0111)
        BCD = BCD3;
    else;
end
endmodule

//Basys 3 FPGA implementation utilizing switches as inputs
module basys_bcd(clk, sw, seg, an);
input clk;
input [15:0] sw;
output [6:0] seg;
output [3:0] an;

wire [3:0] BCD0;
wire [3:0] BCD1;
wire [3:0] BCD2;
wire [3:0] BCD3;

assign BCD0[0] = sw[0];
assign BCD0[1] = sw[1];
assign BCD0[2] = sw[2];
assign BCD0[3] = sw[3];
assign BCD1[0] = sw[4];
assign BCD1[1] = sw[5];
assign BCD1[2] = sw[6];
assign BCD1[3] = sw[7];
assign BCD2[0] = sw[8];
assign BCD2[1] = sw[9];
assign BCD2[2] = sw[10];
assign BCD2[3] = sw[11];
assign BCD3[0] = sw[12];
assign BCD3[1] = sw[13];
assign BCD3[2] = sw[14];
assign BCD3[3] = sw[15];

bcd_seg_complete(clk, BCD0, BCD1, BCD2, BCD3, an, seg);

endmodule

