# ==============================================================================
# XDC ФАЙЛ ДЛЯ СДВИГОВОГО РЕГИСТРА С АВТОМАТИЧЕСКОЙ ТАКТОВОЙ ЧАСТОТОЙ 1 Гц
# ==============================================================================
# Модуль: ShiftRegister_1Hz_Top
# Частота работы: 1 Гц (автоматически, без кнопки CP)
# ==============================================================================

# Конфигурация напряжения
set_property CFGBVS VCCO [current_design]
set_property CONFIG_VOLTAGE 3.3 [current_design]

# ==============================================================================
# СИСТЕМНЫЙ ТАКТОВЫЙ СИГНАЛ - 200 МГц (дифференциальный вход)
# ==============================================================================
set_property PACKAGE_PIN R3 [get_ports sysclk_p]
set_property IOSTANDARD LVDS_25 [get_ports sysclk_p]
set_property PACKAGE_PIN P3 [get_ports sysclk_n]
set_property IOSTANDARD LVDS_25 [get_ports sysclk_n]
create_clock -add -name sys_clk_pin -period 5.00 -waveform {0 2.5} [get_ports sysclk_p]

# ==============================================================================
# КНОПКИ (4 штуки) - Управляющие сигналы
# ==============================================================================
# Кнопка P6 (GPIO_SW_N) - Master Reset (MR_n)
# Функция: Асинхронный сброс всего регистра
set_property PACKAGE_PIN P6 [get_ports MR_n]
set_property IOSTANDARD LVCMOS15 [get_ports MR_n]

# Кнопка U6 (SW_W) - Parallel Enable (PE_n)
# Функция: Переключение режима (нажата=загрузка, отпущена=сдвиг)
set_property PACKAGE_PIN U6 [get_ports PE_n]
set_property IOSTANDARD SSTL15 [get_ports PE_n]

# Кнопка U5 (SW_E) - Serial Input J
# Функция: Последовательный ввод данных в режиме сдвига
set_property PACKAGE_PIN U5 [get_ports J]
set_property IOSTANDARD SSTL15 [get_ports J]

# Кнопка T5 (SW_S) - Serial Input K_n (active low)
# Функция: Инверсный последовательный ввод
set_property PACKAGE_PIN T5 [get_ports K_n]
set_property IOSTANDARD SSTL15 [get_ports K_n]

# ПРИМЕЧАНИЕ: Кнопка R5 (SW_N) НЕ ИСПОЛЬЗУЕТСЯ в этой версии
# Тактирование происходит автоматически на частоте 1 Гц

# ==============================================================================
# DIP-ПЕРЕКЛЮЧАТЕЛИ (4 штуки) - Параллельные входные данные D[3:0]
# ==============================================================================
set_property PACKAGE_PIN R8 [get_ports {D[0]}]
set_property IOSTANDARD SSTL15 [get_ports {D[0]}]
set_property PACKAGE_PIN P8 [get_ports {D[1]}]
set_property IOSTANDARD SSTL15 [get_ports {D[1]}]
set_property PACKAGE_PIN R7 [get_ports {D[2]}]
set_property IOSTANDARD SSTL15 [get_ports {D[2]}]
set_property PACKAGE_PIN R6 [get_ports {D[3]}]
set_property IOSTANDARD SSTL15 [get_ports {D[3]}]

# ==============================================================================
# СВЕТОДИОДЫ НА ОСНОВНОЙ ПЛАТЕ (4 штуки) - Выходы Q[3:0]
# ==============================================================================
set_property PACKAGE_PIN M26 [get_ports {Q[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {Q[0]}]
set_property PACKAGE_PIN T24 [get_ports {Q[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {Q[1]}]
set_property PACKAGE_PIN T25 [get_ports {Q[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {Q[2]}]
set_property PACKAGE_PIN R26 [get_ports {Q[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {Q[3]}]

# ==============================================================================
# СВЕТОДИОД НА ВНЕШНЕЙ ПЛАТЕ - Инверсный выход Q3_n
# ==============================================================================
set_property PACKAGE_PIN F25 [get_ports Q3_n]
set_property IOSTANDARD LVCMOS25 [get_ports Q3_n]
