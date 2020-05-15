library ieee;
use ieee.std_logic_1164.all;
use work.all;

entity LCD_Driver is
	generic(
		clk_freq		: integer;
		baud_rate	: integer
	);
	port(
		clk, reset	: in std_logic;
		rxvalid 	: in std_logic;     
		rxdata	: in std_logic_vector(7 downto 0);
		lcddata	: out std_logic_vector(7 downto 0);
		RS, RW, LCDon, BLon	: out std_logic;
		LEDR		: out std_logic_vector(17 downto 1);
		EN			: buffer std_logic
	);
end LCD_Driver;

architecture logic of LCD_Driver is
	type lcdDataArray is array (0 to 10) of std_logic_vector(7 downto 0);
	signal armed : lcdDataArray := ("01000001", "01101011", "01110100", "01101001", "01110110", "01100101", "01110010", "01100101", "01110100", "00000000", "00000000");
	signal disarmed : lcdDataArray := ("01000100", "01100101", "01100001", "01101011", "01110100", "01101001", "01110110", "01100101", "01110010", "01100101", "01110100");
	signal writeArray : lcdDataArray;
	signal beginWrite : std_logic;
	signal clk_baud : std_logic;
	signal stepCounter : integer range 0 to 38 := 0;
	--type state is (Idle, ClearScreen, DataBit); signal lcdstate : state := Idle;

begin
RW <= '0'; -- Always write
BLon <= '1'; --Backlight on
LCDon <= '1'; --LCD on

BaudRate: entity baud_rate_gen
		generic map(clk_freq, baud_rate)
		port map(clk => clk, reset => reset, clk_out => clk_baud);

		
		
ArrayProcess : process(rxvalid)
begin
	if(rising_edge(rxvalid)) then	
		if(rxdata = "00000001") then
			writeArray <= armed;
		elsif(rxdata = "10000000") then
			writeArray <= disarmed;
		end if;
	end if;
end process;


BeginWriteProcess : process(rxvalid)
begin
	if (rising_edge(rxvalid)) then
		beginWrite <= '1';
	end if;
	if (stepCounter >= 35) then 
		beginWrite <= '0';
	end if;
end process;
	
	

LCDWrite : process(clk_baud, reset, rxvalid)
begin

	if(reset = '0') then
		stepCounter <= 0;
		LEDR(17) <= '0';
	elsif(rising_edge(clk_baud)) then
		if(beginWrite = '1') then
			stepCounter <= stepCounter + 1;
		end if;
	end if;

	
	case stepCounter is
	-- Clear Screen
		when 1 =>
			EN <= '1';
			RS <= '0';
		LEDR(17) <= '1';
		when 2 =>
			lcddata <= "00111000";
		LEDR(16) <= '1';
		when 3 =>
			EN <= '0';
		when 4 =>
			EN <= '1';
			lcddata <= "00001111";
		LEDR(15) <= '1';
		when 5 =>
			EN <= '0';
		when 6 =>
			EN <= '1';
			lcddata <= "00000110";	
		LEDR(14) <= '1';	
		when 7 =>
			EN <= '0';
		when 8 =>
			EN <= '1';
			lcddata <= "00000001";	
		LEDR(13) <= '1';	
		when 9 =>
			EN <= '0';
		when 10 =>
			EN <= '1';
			RS <= '1';
			lcddata <= "01000001";	
		LEDR(12) <= '1';
		when 11 =>
			EN <= '0';
		when 12 =>
			EN <= '1';
			lcddata <= "01101011";			
		when 13 =>
			EN <= '0';
		when 14 =>
			EN <= '1';
			lcddata <= "01110100";
		when 15 =>
			EN <= '0';
		when 16 =>
			EN <= '1';
			lcddata <= writeArray(stepCounter -16);
		when 17 =>
			EN <= '0';
		when 18 =>
			EN <= '1';
			lcddata <= writeArray(stepCounter -18);
		when 19 =>
			EN <= '0';
		when 20 =>
			EN <= '1';
			lcddata <= writeArray(stepCounter -20);
		when 21 =>
			EN <= '0';
		when 22 =>
			EN <= '1';
			lcddata <= writeArray(stepCounter -22);
		when 23 =>
			EN <= '0';
		when 24 =>
			EN <= '1';
			lcddata <= writeArray(stepCounter -24);
		when 25 =>
			EN <= '0';
		when 26 =>
			EN <= '1';
			lcddata <= writeArray(stepCounter -26);
		when 27 =>
			EN <= '0';
		when 28 =>
			EN <= '1';
			lcddata <= writeArray(stepCounter -28);
		when 29 =>
			EN <= '0';
		when 30 =>
			EN <= '1';
			lcddata <= writeArray(stepCounter -30);
		when 31 =>
			EN <= '0';
		when 32 =>
			EN <= '1';
			lcddata <= writeArray(stepCounter -32);
		when 33 =>
			EN <= '0';
		when 34 =>
			EN <= '1';
			LEDR(11) <= '1';
		when others =>
			EN <= '1';
	end case;
end process;

end;
