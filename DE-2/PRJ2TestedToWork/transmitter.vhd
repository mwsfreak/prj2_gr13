library ieee;
use ieee.std_logic_1164.all;

entity transmitter is
	port(
		txvalid, reset, clk_baud	: in std_logic;
		txdata							: in std_logic_vector(7 downto 0);
		txd								: out std_logic
	);
end transmitter;

architecture logic of transmitter is
	signal busy				: std_logic;
begin
	clk: process(clk_baud, reset)
		variable bit_count	: integer := 0;
	begin
		if (reset = '0') then
			bit_count := 0;
			busy <= '0';
			txd <= '1';
		elsif (rising_edge(clk_baud)) then
			if (txvalid = '1' AND busy = '0') then			--Startbit
				busy <= '1';
				txd <= '0';
			elsif (busy = '1' AND bit_count < 8) then		--Databit
				txd <= txdata(bit_count);
				bit_count := bit_count + 1;
			elsif (busy = '1' AND bit_count >= 8) then	--Stopbit
				txd <= '1';
				if (txvalid = '0') then
					bit_count := 0;
					busy <= '0';
				end if;
			end if;
		end if;
	end process;
end logic;