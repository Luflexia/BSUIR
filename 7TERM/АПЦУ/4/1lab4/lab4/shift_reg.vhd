----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 09/25/2025 03:34:45 PM
-- Design Name: 
-- Module Name: shift_reg - Behavioral
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

entity ParallelShiftRegister is
    Port ( 
        MR_n : in STD_LOGIC;     -- Master Reset (active low)
        CP   : in STD_LOGIC;     -- Clock Pulse
        PE_n : in STD_LOGIC;     -- Parallel Enable (active low)  
        J    : in STD_LOGIC;     -- Serial Input
        K_n  : in STD_LOGIC;     -- Serial Input (active low)
        D    : in STD_LOGIC_VECTOR(3 downto 0);  -- Parallel Data
        Q    : out STD_LOGIC_VECTOR(3 downto 0); -- Direct outputs Q0-Q3
        Q3_n : out STD_LOGIC     -- Inverted output Q3
    );
end ParallelShiftRegister;

architecture Structural of ParallelShiftRegister is
    
    COMPONENT JK_trigger
    Port ( 
        J, K, C, R_n, S_n : in STD_LOGIC;
        Q, Q_n : out STD_LOGIC
    );
    END COMPONENT;
    
    signal q0, q1, q2, q3 : STD_LOGIC;
    signal j0, k0, j1, k1, j2, k2, j3, k3 : STD_LOGIC;
    
    
begin
    k0 <= not((not q0 and j and pe_n)OR(pe_n and k_n and q0)OR(not pe_n and d(0)));
    j0 <= not k0;
    
    JK0: JK_trigger port map (
    J => j0,
    K => k0,
    C => CP,
    R_n => MR_n,
    S_n => '1',
    Q => q0,
    Q_n => open
    );
    
    Q(0) <= q0;
    
    k1 <= not((q0 and pe_n)OR(not pe_n and d(1)));
    j1 <= not k1;
    
    JK1: JK_trigger port map (
    J => j1,
    K => k1,
    C => CP,
    R_n => MR_n,
    S_n => '1',
    Q => q1,
    Q_n => open
    );
    
    Q(1) <= q1;
    
    
    k2 <= not((q1 and pe_n)OR(not pe_n and d(2)));
    j2 <= not k2;
    
    JK2: JK_trigger port map (
    J => j2,
    K => k2,
    C => CP,
    R_n => MR_n,
    S_n => '1',
    Q => q2,
    Q_n => open
    );
    
    Q(2) <= q2;
    
    
    k3 <= not((q2 and pe_n)OR(not pe_n and d(3)));
    j3 <= not k3;
    
    JK3: JK_trigger port map (
    J => j3,
    K => k3,
    C => CP,
    R_n => MR_n,
    S_n => '1',
    Q => q3,
    Q_n => open
    );
    
    Q(3) <= q3;
    Q3_n <= not q3;
    
    
end Structural;
    