module moda(g_input, e_input, o);

	input[31:0] g_input, e_input;
	wire[16:0] tmp_0, tmp_1;
	output [1:0] o;

	ADD 
	#(
		.N(16)
	)
	input_0
	(
		.A(g_input[31:16]),
		.B(e_input[31:16]),
		.CI(),
		.S(tmp_0[15:0]),
		.CO(tmp_0[16:16])
	);

	ADD 
	#(
		.N(16)
	)
	input_1
	(
		.A(g_input[15:0]),
		.B(e_input[15:0]),
		.CI(),
		.S(tmp_1[15:0]),
		.CO(tmp_1[16:16])
	);
		
	always@ *
	begin
		if(tmp_0 > tmp_1)
			o <= 0;
		else if(tmp_0 < tmp_1)
			o <= 1;
		else
			o <= 2;
	end
	
endmodule
