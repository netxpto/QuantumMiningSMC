library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.NUMERIC_STD.all;


------------------------------------------
---------- PACKAGE DECLARATIONS ----------
------------------------------------------
package signal_processing is
	
	procedure computation (signalIn:in std_logic_vector; signalOut:out std_logic_vector);
	
end signal_processing;

------------------------------------------
------------- PACKAGE BODY ---------------
------------------------------------------
package body signal_processing is
	
	-- Processes each byte of the signal
	procedure computation (signalIn:in std_logic_vector; signalOut:out std_logic_vector) is
	begin
		-- Implementation
		signalOut := signalIn; --remove later
		
	end computation;
	
end signal_processing;