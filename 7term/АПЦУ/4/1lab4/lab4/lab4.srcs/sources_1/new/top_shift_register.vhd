library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

library UNISIM;
use UNISIM.VComponents.all;

entity top_register is
    Port (
        -- Тактовый сигнал (дифференциальный, 200 МГц)
        sysclk_p : in STD_LOGIC;
        sysclk_n : in STD_LOGIC;
        
        -- Управляющие входы (подключены к переключателям)
        dipswitch : in STD_LOGIC_VECTOR(3 downto 0);  -- D[3:0]
        pushbuttons : in STD_LOGIC_VECTOR(4 downto 0); -- MR_n, PE_n, J, K_n, CP_manual
        
        -- Выходы (подключены к светодиодам)
        ledsmain : out STD_LOGIC_VECTOR(3 downto 0);  -- Q[3:0]
        ledsboard : out STD_LOGIC_VECTOR(0 downto 0)  -- Q3_n
    );
end top_register;

architecture Behavioral of top_register is
    signal clk_200mhz : STD_LOGIC;
    signal slow_clk : STD_LOGIC := '0';
    signal clock_divider : unsigned(26 downto 0) := (others => '0');
    signal manual_clock : STD_LOGIC;
    signal clock_source : STD_LOGIC;
    signal MR_n_internal : STD_LOGIC;
    signal PE_n_internal : STD_LOGIC;
    
    -- Входные сигналы для регистра
    signal MR_n : STD_LOGIC;
    signal PE_n : STD_LOGIC;
    signal J : STD_LOGIC;
    signal K_n : STD_LOGIC;
    signal D : STD_LOGIC_VECTOR(3 downto 0);
    signal Q : STD_LOGIC_VECTOR(3 downto 0);
    signal Q3_n : STD_LOGIC;
    
begin
    -- Преобразование дифференциального тактового сигнала
    IBUFDS_inst : IBUFDS
    generic map (IOSTANDARD => "LVDS_25")
    port map (
        I => sysclk_p,
        IB => sysclk_n,
        O => clk_200mhz
    );

    -- Делитель частоты: 200 МГц -> ~1 Гц
    process (clk_200mhz)
    begin
        if rising_edge(clk_200mhz) then
            if clock_divider = 100000000 then
                slow_clk <= not slow_clk;
                clock_divider <= (others => '0');
            else
                clock_divider <= clock_divider + 1;
            end if;
        end if;
    end process;

        -- Инвертируем сигналы от кнопок (активный низкий в регистре)
    MR_n_internal <= not pushbuttons(0);  -- Кнопка 0 -> MR_n
    PE_n_internal <= not pushbuttons(1);  -- Кнопка 1 -> PE_n
    J <= pushbuttons(2);                  -- Кнопка 2 -> J
    K_n <= pushbuttons(3);                -- Кнопка 3 -> K_n
    D <= dipswitch;                       -- DIP-переключатели -> D

    -- Выбор источника тактирования
    clock_source <= slow_clk when pushbuttons(4) = '0' else pushbuttons(4);

    -- Экземпляр регистра с инвертированными сигналами
    reg_inst : entity work.ParallelShiftRegister
    port map (
        MR_n => MR_n_internal, 
        CP => clock_source,
        PE_n => PE_n_internal,
        J => J,
        K_n => K_n,
        D => D,
        Q => Q,
        Q3_n => Q3_n
    );

    -- Подключение выходов к светодиодам
    ledsmain <= Q;
    ledsboard(0) <= Q3_n;

end Behavioral;