library ieee;
use ieee.std_logic_1164.all, ieee.numeric_std.all;
use work.all;

entity LCDautoSend2L is
	port (CLOCK_50, writeNow, reset : in std_logic;
			rxdata : in std_logic_vector (7 downto 0);
			LCD_RW, LCD_ON, LCD_BLON : out std_logic;
			LCD_RS, LCD_EN : buffer std_logic;
			LCD_DATA : out std_logic_vector(7 downto 0));
end;


Architecture LCDautoV2 of LCDautoSend2L is
signal datasignal : std_logic_vector(7 downto 0);
signal clk_baud : std_logic;
constant Max_command : integer := 4;
constant Max_data : integer := 16;
constant Max_welc : integer := 16;


--- Array Definition ----
subtype DATA is std_logic_vector (7 downto 0);
type commandDATA is array (0 to 3) of DATA;
type dataDATA is array (0 to 15) of DATA;
type welcomeDATA is array (0 to 15) of DATA;
constant commandArray : commandDATA := ("00111000", "00001111", "00000001", "00000110");
constant armedArray : dataDATA := ("01000001", "01101011", "01110100", "01101001", "01110110", "01100101", "01110010", "01100101", "01110100", "00100000", "00100000", "00100000", "00100000", "00100000", "00100000", "00100000");
constant disarmedArray : dataDATA := ("01000100", "01100101", "01100001", "01101011", "01110100", "01101001", "01110110", "01100101", "01110010", "01100101", "01110100", "00100000", "00100000", "00100000", "00100000", "00100000");
constant welcomeArray : welcomeDATA := ("01001000","01101111","01101101","01100101", "00100000","01000001","01110101","01110100","01101111","01101101","01100001","01110100","01101001","01101111","01101110","00100000");
signal writeArray : dataDATA ;
--- Array Definition ----

--- State Definition ---
type state is (Idle, writeWelcome, toggleW1, toggleW2, writeNewLine, toggleLine1, toggleLine2, writeCommand, toggleC1, toggleC2, writeData, toggleD1, toggleD2); 
signal LCD : state := Idle; 
--- State Definition ---

begin 
--- Konstante Indstillinger ---
LCD_RW <= '0';
LCD_BLON <= '1';
LCD_ON <= '1';
--- Konstante Indstillinger ---

BaudRateGen: entity baud_rate_gen generic map(50000000, 5000) port map (clk => CLOCK_50, reset => reset, clk_out => clk_baud);

stateAction : process(clk_baud, LCD, writeNow, rxdata)
	variable stepControl : integer range 0 to Max_data := 0;
	begin
		if rising_edge(clk_baud) then 
	
			case LCD is
				when idle =>
					if writeNow = '1' then
						if (rxdata = "00000001") then
							writeArray <= armedArray;
							stepControl := 0;
							LCD <= writeCommand;
							LCD_RS <= '0';
						elsif (rxdata = "10000000") then
							writeArray <= disarmedArray;
							stepControl := 0;
							LCD <= writeCommand;
							LCD_RS <= '0';	
						end if;
					end if;
			
				--- Command tekst ----
				when writeCommand =>							
					if stepControl <= Max_command then
						LCD_DATA <= commandArray(stepControl);
						LCD <= toggleC1;
					else
						LCD <= writeWelcome;		--- State shift
						LCD_RS <= '1';
						stepControl := 0;
					end if;
					
				when toggleC1 =>
					LCD_EN <= not LCD_EN;
					LCD <= toggleC2;	
					
				when toggleC2 =>
					stepControl := stepControl + 1;
					LCD_EN <= not LCD_EN;
					LCD <= writeCommand;	
			--- Command tekst ----
		
		
		--- Welcome tekst ----
				when writeWelcome =>
					
					if stepControl <= Max_welc then 
						LCD_DATA <= welcomeArray(stepControl);
						LCD <= toggleW1;
					else
						LCD <= writeNewLine;		--- State shift
						stepControl := 0;
						LCD_RS <= '0';
					end if;
					
				when toggleW1 =>	
					LCD_EN <= not LCD_EN;
					LCD <= toggleW2;
				
				when toggleW2 =>	
					stepControl := stepControl + 1;
					LCD_EN <= not LCD_EN;
					LCD <= writeWelcome;
			--- Welcome tekst ----
		
		
		
				--- Command NewLine ----
				when writeNewLine =>							
					if stepControl <= 1 then
						LCD_DATA <= "11000000";			-- 0xC0   Force cursor to beginning to 2nd line
						LCD <= toggleline1;
					else
						LCD <= writeData;		--- State shift
						LCD_RS <= '1';
						stepControl := 0;
					end if;
					
				when toggleline1 =>
					LCD_EN <= not LCD_EN;
					LCD <= toggleline2;	
					
				when toggleline2 =>
					stepControl := stepControl + 1;
					LCD_EN <= not LCD_EN;
					LCD <= writeNewLine;	
			--- Command NewLine ----
		
			--- DATA tekst ----
				when writeData =>
					
					if stepControl <= Max_data then 
						LCD_DATA <= writeArray(stepControl);
						LCD <= toggleD1;
					else
						LCD <= Idle;		--- State shift
						stepControl := 0;
					end if;
					
				when toggleD1 =>	
					LCD_EN <= not LCD_EN;
					LCD <= toggleD2;
				
				when toggleD2 =>	
					stepControl := stepControl + 1;
					LCD_EN <= not LCD_EN;
					LCD <= writeData;
			--- DATA tekst ----
			end case;
		end if;
end process;

end;