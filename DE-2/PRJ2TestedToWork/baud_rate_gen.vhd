library ieee;
use ieee.std_logic_1164.all;

entity baud_rate_gen is
	generic(
		clk_freq		: integer;
		baud_rate	: integer
	);
	port(
		clk, reset	: in std_logic;
		clk_out		: out std_logic
	);
end baud_rate_gen;

architecture logic of baud_rate_gen is
	
begin
	clk_gen: process(clk, reset)
		variable clk_count	: integer := 0;
	begin
		if (reset = '0') then
			clk_count := 0;
		elsif (rising_edge(clk)) then
			clk_count := clk_count + 1;
			if (clk_count >= (clk_freq/baud_rate)) then
				clk_out <= '1';
				clk_count := 0;
			else
				clk_out <= '0';
			end if;
		end if;
	end process;
end logic;