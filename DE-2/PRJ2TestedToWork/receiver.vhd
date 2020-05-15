library ieee;
use ieee.std_logic_1164.all;

entity receiver is
	port(
		rxd, reset, clk_baud	: in std_logic;
		rxvalid					: out std_logic;
		rxdata					: out std_logic_vector(7 downto 0)
	);
end receiver;

architecture logic of receiver is
	signal busy				: std_logic;
begin
	clk: process(clk_baud, reset)
		variable bit_count	: integer := 0;
	begin
		if (reset = '0') then
			bit_count := 0;
			rxdata <= (others => '0');
			busy <= '0';
			rxvalid <= '0';
		elsif (rising_edge(clk_baud)) then
			if (rxd = '0' AND busy = '0') then				--Startbit
				busy <= '1';
				rxvalid <= '0';
			elsif (busy = '1' AND bit_count < 8) then		--Databit
				rxdata(bit_count) <= rxd;
				bit_count := bit_count + 1;
			elsif (busy = '1' AND bit_count = 8) then	--Stopbit
				if (rxd = '1') then
					rxvalid <= '1';
					bit_count := bit_count + 1;
				else
					rxvalid <= '0';
					bit_count := 0;
					busy <= '0';
				end if;
			elsif (busy = '1' AND bit_count > 8 AND bit_count <= 9) then
				bit_count := bit_count + 1;
			elsif (busy = '1' AND bit_count > 9) then		--Reset rxvalid and busy after 208us (2 clock cycle) of receiving the stopbit
				rxvalid <= '0';
				bit_count := 0;
				busy <= '0';
			end if;
		end if;
	end process;
end logic;