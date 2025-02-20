// Copyright (C) 1991-2009 Altera Corporation
// Your use of Altera Corporation's design tools, logic functions 
// and other software and tools, and its AMPP partner logic 
// functions, and any output files from any of the foregoing 
// (including device programming or simulation files), and any 
// associated documentation or information are expressly subject 
// to the terms and conditions of the Altera Program License 
// Subscription Agreement, Altera MegaCore Function License 
// Agreement, or other applicable license agreement, including, 
// without limitation, that your use is for the sole purpose of 
// programming logic devices manufactured by Altera and sold by 
// Altera or its authorized distributors.  Please refer to the 
// applicable agreement for further details.

// VENDOR "Altera"
// PROGRAM "Quartus II"
// VERSION "Version 9.1 Build 222 10/21/2009 SJ Web Edition"

// DATE "12/12/2023 12:19:28"

// 
// Device: Altera EP2S15F484C3 Package FBGA484
// 

// 
// This greybox netlist file is for third party Synthesis Tools
// for timing and resource estimation only.
// 


module lpm_xor0 (
	data0x,
	data1x,
	result)/* synthesis synthesis_greybox=0 */;
input 	[8:0] data0x;
input 	[8:0] data1x;
output 	[8:0] result;

wire gnd;
wire vcc;
wire unknown;

assign gnd = 1'b0;
assign vcc = 1'b1;
// unknown value (1'bx) is not needed for this tool. Default to 1'b0
assign unknown = 1'b0;

wire \lpm_xor_component|xor_cascade[0][1]~combout ;
wire \lpm_xor_component|xor_cascade[1][1]~combout ;
wire \lpm_xor_component|xor_cascade[2][1]~combout ;
wire \lpm_xor_component|xor_cascade[3][1]~combout ;
wire \lpm_xor_component|xor_cascade[4][1]~combout ;
wire \lpm_xor_component|xor_cascade[5][1]~combout ;
wire \lpm_xor_component|xor_cascade[6][1]~combout ;
wire \lpm_xor_component|xor_cascade[7][1]~combout ;
wire \lpm_xor_component|xor_cascade[8][1]~combout ;


lpm_xor0_lpm_xor_1 lpm_xor_component(
	.xor_cascade_1_0(\lpm_xor_component|xor_cascade[0][1]~combout ),
	.xor_cascade_1_1(\lpm_xor_component|xor_cascade[1][1]~combout ),
	.xor_cascade_1_2(\lpm_xor_component|xor_cascade[2][1]~combout ),
	.xor_cascade_1_3(\lpm_xor_component|xor_cascade[3][1]~combout ),
	.xor_cascade_1_4(\lpm_xor_component|xor_cascade[4][1]~combout ),
	.xor_cascade_1_5(\lpm_xor_component|xor_cascade[5][1]~combout ),
	.xor_cascade_1_6(\lpm_xor_component|xor_cascade[6][1]~combout ),
	.xor_cascade_1_7(\lpm_xor_component|xor_cascade[7][1]~combout ),
	.xor_cascade_1_8(\lpm_xor_component|xor_cascade[8][1]~combout ),
	.data0x_0(data0x[0]),
	.data1x_0(data1x[0]),
	.data0x_1(data0x[1]),
	.data1x_1(data1x[1]),
	.data0x_2(data0x[2]),
	.data1x_2(data1x[2]),
	.data0x_3(data0x[3]),
	.data1x_3(data1x[3]),
	.data0x_4(data0x[4]),
	.data1x_4(data1x[4]),
	.data0x_5(data0x[5]),
	.data1x_5(data1x[5]),
	.data0x_6(data0x[6]),
	.data1x_6(data1x[6]),
	.data0x_7(data0x[7]),
	.data1x_7(data1x[7]),
	.data0x_8(data0x[8]),
	.data1x_8(data1x[8]));

assign result[0] = \lpm_xor_component|xor_cascade[0][1]~combout ;

assign result[1] = \lpm_xor_component|xor_cascade[1][1]~combout ;

assign result[2] = \lpm_xor_component|xor_cascade[2][1]~combout ;

assign result[3] = \lpm_xor_component|xor_cascade[3][1]~combout ;

assign result[4] = \lpm_xor_component|xor_cascade[4][1]~combout ;

assign result[5] = \lpm_xor_component|xor_cascade[5][1]~combout ;

assign result[6] = \lpm_xor_component|xor_cascade[6][1]~combout ;

assign result[7] = \lpm_xor_component|xor_cascade[7][1]~combout ;

assign result[8] = \lpm_xor_component|xor_cascade[8][1]~combout ;

endmodule

module lpm_xor0_lpm_xor_1 (
	xor_cascade_1_0,
	xor_cascade_1_1,
	xor_cascade_1_2,
	xor_cascade_1_3,
	xor_cascade_1_4,
	xor_cascade_1_5,
	xor_cascade_1_6,
	xor_cascade_1_7,
	xor_cascade_1_8,
	data0x_0,
	data1x_0,
	data0x_1,
	data1x_1,
	data0x_2,
	data1x_2,
	data0x_3,
	data1x_3,
	data0x_4,
	data1x_4,
	data0x_5,
	data1x_5,
	data0x_6,
	data1x_6,
	data0x_7,
	data1x_7,
	data0x_8,
	data1x_8)/* synthesis synthesis_greybox=0 */;
output 	xor_cascade_1_0;
output 	xor_cascade_1_1;
output 	xor_cascade_1_2;
output 	xor_cascade_1_3;
output 	xor_cascade_1_4;
output 	xor_cascade_1_5;
output 	xor_cascade_1_6;
output 	xor_cascade_1_7;
output 	xor_cascade_1_8;
input 	data0x_0;
input 	data1x_0;
input 	data0x_1;
input 	data1x_1;
input 	data0x_2;
input 	data1x_2;
input 	data0x_3;
input 	data1x_3;
input 	data0x_4;
input 	data1x_4;
input 	data0x_5;
input 	data1x_5;
input 	data0x_6;
input 	data1x_6;
input 	data0x_7;
input 	data1x_7;
input 	data0x_8;
input 	data1x_8;

wire gnd;
wire vcc;
wire unknown;

assign gnd = 1'b0;
assign vcc = 1'b1;
// unknown value (1'bx) is not needed for this tool. Default to 1'b0
assign unknown = 1'b0;



stratixii_lcell_comb \xor_cascade[0][1] (
	.dataa(!data0x_0),
	.datab(!data1x_0),
	.datac(vcc),
	.datad(vcc),
	.datae(vcc),
	.dataf(vcc),
	.datag(vcc),
	.cin(gnd),
	.sharein(gnd),
	.combout(xor_cascade_1_0),
	.sumout(),
	.cout(),
	.shareout());
defparam \xor_cascade[0][1] .extended_lut = "off";
defparam \xor_cascade[0][1] .lut_mask = 64'h6666666666666666;
defparam \xor_cascade[0][1] .shared_arith = "off";

stratixii_lcell_comb \xor_cascade[1][1] (
	.dataa(!data0x_1),
	.datab(!data1x_1),
	.datac(vcc),
	.datad(vcc),
	.datae(vcc),
	.dataf(vcc),
	.datag(vcc),
	.cin(gnd),
	.sharein(gnd),
	.combout(xor_cascade_1_1),
	.sumout(),
	.cout(),
	.shareout());
defparam \xor_cascade[1][1] .extended_lut = "off";
defparam \xor_cascade[1][1] .lut_mask = 64'h6666666666666666;
defparam \xor_cascade[1][1] .shared_arith = "off";

stratixii_lcell_comb \xor_cascade[2][1] (
	.dataa(!data0x_2),
	.datab(!data1x_2),
	.datac(vcc),
	.datad(vcc),
	.datae(vcc),
	.dataf(vcc),
	.datag(vcc),
	.cin(gnd),
	.sharein(gnd),
	.combout(xor_cascade_1_2),
	.sumout(),
	.cout(),
	.shareout());
defparam \xor_cascade[2][1] .extended_lut = "off";
defparam \xor_cascade[2][1] .lut_mask = 64'h6666666666666666;
defparam \xor_cascade[2][1] .shared_arith = "off";

stratixii_lcell_comb \xor_cascade[3][1] (
	.dataa(!data0x_3),
	.datab(!data1x_3),
	.datac(vcc),
	.datad(vcc),
	.datae(vcc),
	.dataf(vcc),
	.datag(vcc),
	.cin(gnd),
	.sharein(gnd),
	.combout(xor_cascade_1_3),
	.sumout(),
	.cout(),
	.shareout());
defparam \xor_cascade[3][1] .extended_lut = "off";
defparam \xor_cascade[3][1] .lut_mask = 64'h6666666666666666;
defparam \xor_cascade[3][1] .shared_arith = "off";

stratixii_lcell_comb \xor_cascade[4][1] (
	.dataa(!data0x_4),
	.datab(!data1x_4),
	.datac(vcc),
	.datad(vcc),
	.datae(vcc),
	.dataf(vcc),
	.datag(vcc),
	.cin(gnd),
	.sharein(gnd),
	.combout(xor_cascade_1_4),
	.sumout(),
	.cout(),
	.shareout());
defparam \xor_cascade[4][1] .extended_lut = "off";
defparam \xor_cascade[4][1] .lut_mask = 64'h6666666666666666;
defparam \xor_cascade[4][1] .shared_arith = "off";

stratixii_lcell_comb \xor_cascade[5][1] (
	.dataa(!data0x_5),
	.datab(!data1x_5),
	.datac(vcc),
	.datad(vcc),
	.datae(vcc),
	.dataf(vcc),
	.datag(vcc),
	.cin(gnd),
	.sharein(gnd),
	.combout(xor_cascade_1_5),
	.sumout(),
	.cout(),
	.shareout());
defparam \xor_cascade[5][1] .extended_lut = "off";
defparam \xor_cascade[5][1] .lut_mask = 64'h6666666666666666;
defparam \xor_cascade[5][1] .shared_arith = "off";

stratixii_lcell_comb \xor_cascade[6][1] (
	.dataa(!data0x_6),
	.datab(!data1x_6),
	.datac(vcc),
	.datad(vcc),
	.datae(vcc),
	.dataf(vcc),
	.datag(vcc),
	.cin(gnd),
	.sharein(gnd),
	.combout(xor_cascade_1_6),
	.sumout(),
	.cout(),
	.shareout());
defparam \xor_cascade[6][1] .extended_lut = "off";
defparam \xor_cascade[6][1] .lut_mask = 64'h6666666666666666;
defparam \xor_cascade[6][1] .shared_arith = "off";

stratixii_lcell_comb \xor_cascade[7][1] (
	.dataa(!data0x_7),
	.datab(!data1x_7),
	.datac(vcc),
	.datad(vcc),
	.datae(vcc),
	.dataf(vcc),
	.datag(vcc),
	.cin(gnd),
	.sharein(gnd),
	.combout(xor_cascade_1_7),
	.sumout(),
	.cout(),
	.shareout());
defparam \xor_cascade[7][1] .extended_lut = "off";
defparam \xor_cascade[7][1] .lut_mask = 64'h6666666666666666;
defparam \xor_cascade[7][1] .shared_arith = "off";

stratixii_lcell_comb \xor_cascade[8][1] (
	.dataa(!data0x_8),
	.datab(!data1x_8),
	.datac(vcc),
	.datad(vcc),
	.datae(vcc),
	.dataf(vcc),
	.datag(vcc),
	.cin(gnd),
	.sharein(gnd),
	.combout(xor_cascade_1_8),
	.sumout(),
	.cout(),
	.shareout());
defparam \xor_cascade[8][1] .extended_lut = "off";
defparam \xor_cascade[8][1] .lut_mask = 64'h6666666666666666;
defparam \xor_cascade[8][1] .shared_arith = "off";

endmodule
