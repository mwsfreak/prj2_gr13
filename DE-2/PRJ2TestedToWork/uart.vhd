library ieee;
use ieee.std_logic_1164.all;
use work.all;

entity uart is
	generic(
		clk_freq		: integer;
		baud_rate	: integer
	);
	port(
		clk, reset, rxd, txvalid	: in std_logic;
		txdata							: in std_logic_vector(7 downto 0);
		txd					: out std_logic;
		rxvalid							: out std_logic;   			
		rxdata							: out std_logic_vector(7 downto 0)
	);
end uart;

architecture logic of uart is
	signal clk_baud	: std_logic;
begin
	BaudRate: entity baud_rate_gen
		generic map(clk_freq, baud_rate)
		port map(clk => clk, reset => reset, clk_out => clk_baud);
	Receive: entity receiver
		port map(rxd => rxd, reset => reset, clk_baud => clk_baud, rxvalid => rxvalid, rxdata => rxdata);
	Transmit: entity transmitter
		port map(txd => txd, reset => reset, clk_baud => clk_baud, txvalid => txvalid, txdata => txdata);

end logic;