----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 09/25/2025 03:53:10 PM
-- Design Name: 
-- Module Name: JK_trigger - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity JK_trigger is
    Port ( J : in STD_LOGIC;
           K : in STD_LOGIC;
           C : in STD_LOGIC;
           R_n : in STD_LOGIC;
           S_n : in STD_LOGIC;
           Q : out STD_LOGIC;
           Q_n : out STD_LOGIC);
end JK_trigger;

architecture Behavioral of JK_trigger is
    signal q_temp : STD_LOGIC := '0';
begin
    process(C, R_n, S_n) 
    begin
        if R_n = '0' then
            q_temp <= '0';
        elsif S_n = '0' then
            q_temp <= '1';
        elsif rising_edge(C) then
            if j = '0' and k = '0' then
                q_temp <= q_temp;
            elsif j = '0' and k = '1' then
                q_temp <= '0';
            elsif j = '1' and k = '0' then
                q_temp <= '1';
            else q_temp <= not q_temp;
            end if;
         end if;
     end process;
     Q <= q_temp;
     Q_n <= not q_temp;
     
end Behavioral;
