library ieee;
use ieee.std_logic_1164.all;
use work.all;

entity LCD_state_machine is
	generic(
		clk_freq		: integer;
		baud_rate	: integer
	);
	port(
		clk, reset	: in std_logic;
		rxvalid 	: in std_logic;     
		rxdata		: in std_logic_vector(7 downto 0);
		LCD_DATA	: out std_logic_vector(7 downto 0);
		LCD_RS, LCD_RW, LCD_ON, LCD_BLON	: out std_logic;
		LEDR		: out std_logic_vector(17 downto 1);
		LCD_EN		: out std_logic
	);
end entity;

architecture LCD of LCD_state_machine  is
	shared variable counter : integer range 0 to 16 := 0;
	signal clk_baud : std_logic;
	
	type lcdDataArray is array (0 to 10) of std_logic_vector(7 downto 0);
	signal armed : lcdDataArray := ("01000001", "01101011", "01110100", "01101001", "01110110", "01100101", "01110010", "01100101", "01110100", "00000000", "00000000");
	signal disarmed : lcdDataArray := ("01000100", "01100101", "01100001", "01101011", "01110100", "01101001", "01110110", "01100101", "01110010", "01100101", "01110100");
	signal writeArray : lcdDataArray;
	
	type lcdCommandArray is array (0 to 3) of std_logic_vector(7 downto 0);
	signal commandArray	:	lcdCommandArray := ("00111000","00001111","00000110","00000001");

	type count_state is (Idle, command, Data);
	signal present_state : count_state;
begin

BaudRate: entity baud_rate_gen generic map(clk_freq, baud_rate) port map(clk => clk, reset => reset, clk_out => clk_baud);

	LEDR(counter) <= '1';


process (present_state, rxvalid)
	begin
		case present_state is
			when Idle =>
				if rxvalid = '1' then
					present_state <= command;
				end if;
			when command =>
				LCD_RS <= '1';
				if ( counter <= 3 ) then
					lcd_DATA <= commandArray(counter);	
				elsif (counter > 4) then 
					present_state <= Data;
				end if;
			when Data =>
				LCD_RS <= '0';
				if ( counter < 15 ) then 
					lcd_DATA <= writeArray(counter);
					present_state <= Idle;
				end if;
		end case;
end process;

process(clk_baud)
	begin
		if ( rising_edge(clk_baud)) then
			if (present_state /= Idle) then 
				counter := counter + 1;
			elsif (present_state = idle) then
				counter := 0;
			end if;
		end if;
end process;

writeArray <= armed;

LCD_RW <= '0';
LCD_BLON <= '1';
LCD_ON <= '1';

end LCD;
