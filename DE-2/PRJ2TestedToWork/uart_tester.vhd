library ieee;
use ieee.std_logic_1164.all;
use work.all;

entity uart_tester is
	port(
		CLOCK_50		: in std_logic;
		KEY			: in std_logic_vector(2 downto 0);
		GPIO_0		: out std_logic_vector(0 downto 0);
		SW				: in std_logic_vector(7 downto 0);
		LCD_RW		: out std_logic;
		LCD_RS		: out std_logic;
		LCD_EN		: out std_logic;
		LCD_ON		: out std_logic;
		LCD_BLON		: out std_logic;
		LCD_DATA		: out std_logic_vector(7 downto 0);
		LEDG			: out std_logic_vector(7 downto 0);
		LEDR			: out std_logic_vector(17 downto 0);
		GPIO_1		: in std_logic_vector(0 downto 0)
	);
end uart_tester;

architecture testbench of uart_tester is
signal receivedData 	: std_logic_vector(7 downto 0);
signal valid			: std_logic;
begin
	uut: entity uart
		generic map(50000000, 9600)
		port map(clk => CLOCK_50, reset => KEY(0), rxd => GPIO_1(0), rxvalid => valid,
				rxdata => receivedData, txd => GPIO_0(0), txvalid => NOT(KEY(1)), txdata => SW);

	lcd: entity LCDautoSend2L
		port map(CLOCK_50 => CLOCK_50, writeNow => valid, reset => KEY(0), rxdata => receivedData, 
		LCD_RW => LCD_RW, LCD_ON => LCD_ON, LCD_BLON => LCD_BLON, LCD_RS => LCD_RS, LCD_EN => LCD_EN, LCD_DATA => LCD_DATA);

	LEDR(0) <= valid;
	LEDG <= receivedData;
	
	
end testbench;