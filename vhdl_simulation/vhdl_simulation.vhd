library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.STD_LOGIC_TEXTIO.all;
use IEEE.NUMERIC_STD.all;
use STD.TEXTIO.all;

use work.signal_processing.all;

entity vhdl_simulation is
	generic(FILENAME : string := "SIGNAL.sgn");
end vhdl_simulation;

architecture Behavioral of vhdl_simulation is

	--------------------------------------------------------
	-- TestBench Internal Signals	--
	--------------------------------------------------------
	subtype myType is character; -- reads 1 byte
	type FileType is file of myType;
	file file_input_signal : FileType; -- Pointer to the input .sgn file
	file file_output_signal : FileType; -- Pointer to the output .sgn file

		
begin
				
	--------------------------------------------------------
	-- I/O HANDLER PROCESS
	--------------------------------------------------------
	
io_proc: process
				
				variable char_buffer_in    : myType; -- character received
				variable char_buffer_out   : myType; -- character ouputted
				variable vector_buffer_in  : std_logic_vector(7 downto 0);
				variable vector_buffer_out : std_logic_vector(7 downto 0);
				variable new_line_count    : integer := 0; -- Counts the 4 newLine characters to ignore the HEADER
				
			begin
				-- The simulation must be executed in the directory \vhdl_simulation\
				file_open(file_input_signal, ".\input_files\" & FILENAME, READ_MODE);
				file_open(file_output_signal, ".\output_files\" & FILENAME, WRITE_MODE);
				
				while not endfile(file_input_signal) loop -- while there is information to be read
					-------------------------------------------
					-- Character Read
					-------------------------------------------
					-- Reads the input bits from the input file
					read(file_input_signal, char_buffer_in);
					-------------------------------------------
					-- HEADER Remover
					-------------------------------------------
					-- If /= 4 it's still reading the HEADER
					if(new_line_count /= 4) then
						-- If the current char is a new line character
						if(character'pos(char_buffer_in) = 10) then
						-- if new_line_count = 4, means HEADER is over
							new_line_count :=  new_line_count + 1;
						end if;
						write(file_output_signal, char_buffer_in); -- writes the header in the output file
						next;
					end if;
					-------------------------------------------
					-- Computation
					-------------------------------------------
					vector_buffer_in := std_logic_vector(to_unsigned(character'pos(char_buffer_in),8));
					computation(vector_buffer_in,vector_buffer_out); -- Signal Processing
					char_buffer_out := character'val(to_integer(unsigned(vector_buffer_out)));
					-------------------------------------------
					-- Write back
					-------------------------------------------
					-- Writes the output bits to the output file
					write(file_output_signal, char_buffer_out);
				end loop;
				
				file_close(file_input_signal);
				file_close(file_output_signal);
				
				wait;
			end process;
end Behavioral;