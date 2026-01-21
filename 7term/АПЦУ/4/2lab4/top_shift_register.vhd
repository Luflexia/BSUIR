-- =====================================================
-- Файл: my_top.vhd
-- Топ-модуль проекта "Универсальный 4-битный сдвиговый регистр на JK-триггерах"
-- Полностью готов к синтезу и демонстрации на плате
-- =====================================================

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity my_top is
    port (
        -- системный дифференциальный тактовый сигнал 200 МГц
        sysclk_p   : in  std_logic;
        sysclk_n   : in  std_logic;

        -- DIP-переключатели = параллельные данные D[3:0]
        dipswitch  : in  std_logic_vector(3 downto 0);

        -- Кнопки (смотри фото платы и подписи)
        pushbuttons: in  std_logic_vector(4 downto 0);  -- [0]=SW5, [1]=SW0, [2]=SW1, [3]=SW2, [4]=SW4

        -- Светодиоды на основной плате = Q[3:0]
        ledsmain   : out std_logic_vector(3 downto 0);

        -- Светодиод на внешней плате = Q3 инверсный
        ledsboard  : out std_logic_vector(0 downto 0)
    );
end entity;

architecture rtl of my_top is

    -- сигналы от блока тактирования и антидребезга
    signal clk_1hz     : std_logic;
    signal cp_from_btn : std_logic;
    signal cp_final    : std_logic;

    -- внутренний 200 МГц (нужен для делителя)
    signal clk_200mhz  : std_logic;

begin

    -- Буфер для дифференциального тактового сигнала 200 МГц
    ibufgds_inst : IBUFGDS
        port map (
            O  => clk_200mhz,
            I  => sysclk_p,
            IB => sysclk_n
        );

    -- Блок генератора 1 Гц + антидребезг кнопки SW4 (T5)
    u_clk : entity work.clk_debounce_1hz
        port map (
            clk_200mhz => clk_200mhz,
            btn_raw    => pushbuttons(4),   -- SW4 = пин T5
            cp_manual  => cp_from_btn,
            clk_1hz    => clk_1hz
        );

    -- Выбираем источник тактирования:
    -- если SW4 нажата - используем одиночный импульс от кнопки
    -- если отпущена - красивый 1 Гц
    cp_final <= cp_from_btn when pushbuttons(4) = '0' else clk_1hz;

    -- Сам регистр
    u_reg : entity work.ParallelShiftRegister
        port map (
            MR_n  => not pushbuttons(0),   -- SW5 (P6)  = Master Reset (активный 0)
            CP    => cp_final,             -- тактовый сигнал (1 Гц или от кнопки)
            PE_n  => pushbuttons(1),       -- SW0 (R5)  = Parallel Enable (активный 0)
            J     => not pushbuttons(2),   -- SW1 (U6)  = J (инвертируем, чтобы нажатие = 0 ? J=1)
            K_n   => pushbuttons(3),       -- SW2 (U5)  = K_n (нажатие = 0 ? K=1)
            D     => dipswitch,            -- данные с DIP-переключателей
            Q     => ledsmain,             -- Q3 Q2 Q1 Q0 ? светодиоды на основной плате
            Q3_n  => ledsboard(0)          -- инверсный старший бит ? внешний светодиод
        );

end rtl;