module and_gate(g_input, e_input, o);
	input[3:0] g_input, e_input;
	output[3:0] o;
	assign o = g_input ^ e_input;
endmodule
