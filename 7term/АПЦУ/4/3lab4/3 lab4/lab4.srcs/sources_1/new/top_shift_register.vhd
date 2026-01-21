library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

-- Top-level модуль с делителем частоты для работы регистра на 1 Гц
entity ShiftRegister_1Hz_Top is
    Port ( 
        -- Системный тактовый сигнал 200 МГц (дифференциальный)
        sysclk_p : in STD_LOGIC;
        sysclk_n : in STD_LOGIC;

        -- Управляющие сигналы от кнопок
        MR_n     : in STD_LOGIC;     -- Master Reset (active low) - P6
        PE_n     : in STD_LOGIC;     -- Parallel Enable (active low) - U6
        J        : in STD_LOGIC;     -- Serial Input - U5
        K_n      : in STD_LOGIC;     -- Serial Input (active low) - T5

        -- Параллельные входные данные от DIP-переключателей
        D        : in STD_LOGIC_VECTOR(3 downto 0);

        -- Выходы на светодиоды
        Q        : out STD_LOGIC_VECTOR(3 downto 0); -- Основная плата
        Q3_n     : out STD_LOGIC     -- Внешняя плата (инверсия Q3)
    );
end ShiftRegister_1Hz_Top;

architecture Behavioral of ShiftRegister_1Hz_Top is

    -- Компонент сдвигового регистра
    COMPONENT ParallelShiftRegister
    Port ( 
        MR_n : in STD_LOGIC;
        CP   : in STD_LOGIC;
        PE_n : in STD_LOGIC;
        J    : in STD_LOGIC;
        K_n  : in STD_LOGIC;
        D    : in STD_LOGIC_VECTOR(3 downto 0);
        Q    : out STD_LOGIC_VECTOR(3 downto 0);
        Q3_n : out STD_LOGIC
    );
    END COMPONENT;

    -- Компонент для преобразования дифференциального сигнала
    COMPONENT IBUFDS
    generic (
        DIFF_TERM    : boolean := FALSE;
        IBUF_LOW_PWR : boolean := TRUE;
        IOSTANDARD   : string  := "DEFAULT"
    );
    port (
        O  : out std_ulogic;
        I  : in  std_ulogic;
        IB : in  std_ulogic
    );
    END COMPONENT;

    -- Сигналы
    signal clk_200MHz : STD_LOGIC;  -- Тактовый сигнал 200 МГц
    signal clk_1Hz    : STD_LOGIC := '0';  -- Тактовый сигнал 1 Гц
    signal counter    : unsigned(27 downto 0) := (others => '0');

    -- Константа для деления частоты: 200 000 000 / 2 = 100 000 000
    -- (делим на 2, потому что переключаем сигнал каждые 100M тактов)
    constant COUNT_MAX : unsigned(27 downto 0) := to_unsigned(100_000_000, 28);

begin

    -- Преобразование дифференциального тактового сигнала в single-ended
    IBUFDS_inst : IBUFDS
    generic map (
        DIFF_TERM    => FALSE,
        IBUF_LOW_PWR => TRUE,
        IOSTANDARD   => "LVDS_25"
    )
    port map (
        O  => clk_200MHz,
        I  => sysclk_p,
        IB => sysclk_n
    );

    -- Делитель частоты: 200 МГц ? 1 Гц
    process(clk_200MHz, MR_n)
    begin
        if MR_n = '0' then
            -- Асинхронный сброс
            counter <= (others => '0');
            clk_1Hz <= '0';
        elsif rising_edge(clk_200MHz) then
            if counter >= COUNT_MAX - 1 then
                counter <= (others => '0');
                clk_1Hz <= not clk_1Hz;  -- Переключаем каждые 100M тактов
            else
                counter <= counter + 1;
            end if;
        end if;
    end process;

    -- Инстанцирование сдвигового регистра
    ShiftReg_inst : ParallelShiftRegister
    port map (
        MR_n => MR_n,
        CP   => clk_1Hz,      -- Используем 1 Гц вместо кнопки
        PE_n => PE_n,
        J    => J,
        K_n  => K_n,
        D    => D,
        Q    => Q,
        Q3_n => Q3_n
    );

end Behavioral;
