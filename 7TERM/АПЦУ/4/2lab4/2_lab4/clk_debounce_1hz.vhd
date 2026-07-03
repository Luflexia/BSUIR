-- =====================================================
-- Файл: clk_debounce_1hz.vhd
-- Назначение: генератор 1 Гц + антидребезг кнопки тактирования
-- Используется в проекте универсального сдвигового регистра
-- =====================================================

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity clk_debounce_1hz is
    port (
        clk_200mhz : in  std_logic;                     -- системный тактовый сигнал 200 МГц (sysclk_p)
        btn_raw    : in  std_logic;                     -- кнопка SW4 (пин T5) напрямую от платы
        cp_manual  : out std_logic;                     -- чистый одиночный импульс от кнопки (антидребезг)
        clk_1hz    : out std_logic                      -- медленный тактовый сигнал 1 Гц для красивой демонстрации
    );
end entity;

architecture rtl of clk_debounce_1hz is
    -- 200 МГц ? 1 Гц: делитель на 200 000 000
    signal cnt_1hz   : unsigned(26 downto 0) := (others => '0');
    signal tick_1hz  : std_logic := '0';

    -- антидребезг кнопки
    signal btn_sync  : std_logic_vector(2 downto 0) := (others => '1');
    signal btn_stable: std_logic := '1';
    signal btn_prev  : std_logic := '1';
begin

    -- Генератор 1 Гц
    process(clk_200mhz)
    begin
        if rising_edge(clk_200mhz) then
            if cnt_1hz = 199_999_999 then
                cnt_1hz  <= (others => '0');
                tick_1hz <= not tick_1hz;
            else
                cnt_1hz <= cnt_1hz + 1;
            end if;
        end if;
    end process;
    clk_1hz <= tick_1hz;

    -- Антидребезг + однократный импульс от кнопки SW4
    process(clk_200mhz)
    begin
        if rising_edge(clk_200mhz) then
            btn_sync <= btn_sync(1 downto 0) & btn_raw;

            -- стабильное состояние кнопки
            if btn_sync(2 downto 1) = "00" then
                btn_stable <= '0';
            elsif btn_sync(2 downto 1) = "11" then
                btn_stable <= '1';
            end if;

            -- один импульс по нажатию (переход 1?0)
            if btn_prev = '1' and btn_stable = '0' then
                cp_manual <= '1';
            else
                cp_manual <= '0';
            end if;

            btn_prev <= btn_stable;
        end if;
    end process;

end rtl;