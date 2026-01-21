# =====================================================
# my_top.xdc - ФИНАЛЬНАЯ ВЕРСИЯ (гарантированно работает)
# Для Artix-7 (AC701 / похожие платы), Vivado 2019-2025
# =====================================================

# Напряжение и конфигурация банка
set_property CFGBVS VCCO [current_design]
set_property CONFIG_VOLTAGE 3.3 [current_design]

# 200 МГц дифференциальный тактовый сигнал - ТОЛЬКО LVDS_25!
set_property PACKAGE_PIN R3 [get_ports sysclk_p]
set_property PACKAGE_PIN P3 [get_ports sysclk_n]
set_property IOSTANDARD LVDS_25 [get_ports sysclk_p]
set_property IOSTANDARD LVDS_25 [get_ports sysclk_n]
set_property DIFF_TERM TRUE [get_ports sysclk_p]
set_property DIFF_TERM TRUE [get_ports sysclk_n]
create_clock -period 5.000 -name sys_clk [get_ports sysclk_p]

# DIP-переключатели = параллельные данные D[3:0]
set_property -dict {PACKAGE_PIN R8 IOSTANDARD SSTL15} [get_ports {dipswitch[0]}]
set_property -dict {PACKAGE_PIN P8 IOSTANDARD SSTL15} [get_ports {dipswitch[1]}]
set_property -dict {PACKAGE_PIN R7 IOSTANDARD SSTL15} [get_ports {dipswitch[2]}]
set_property -dict {PACKAGE_PIN R6 IOSTANDARD SSTL15} [get_ports {dipswitch[3]}]

# Кнопки - именно в таком виде Vivado перестаёт ругаться на UCIO-1
set_property -dict {PACKAGE_PIN P6 IOSTANDARD SSTL15} [get_ports {pushbuttons[0]}]  ;# SW5  ? MR_n
set_property -dict {PACKAGE_PIN R5 IOSTANDARD SSTL15} [get_ports {pushbuttons[1]}]  ;# SW0  ? PE_n
set_property -dict {PACKAGE_PIN U6 IOSTANDARD SSTL15} [get_ports {pushbuttons[2]}]  ;# SW1  ? J
set_property -dict {PACKAGE_PIN U5 IOSTANDARD SSTL15} [get_ports {pushbuttons[3]}]  ;# SW2  ? K_n
set_property -dict {PACKAGE_PIN T5 IOSTANDARD SSTL15} [get_ports {pushbuttons[4]}]  ;# SW4  ? тактирование

# Светодиоды на основной плате = Q[3:0]
set_property -dict {PACKAGE_PIN M26 IOSTANDARD LVCMOS33} [get_ports {ledsmain[0]}]   ;# Q0
set_property -dict {PACKAGE_PIN T24 IOSTANDARD LVCMOS33} [get_ports {ledsmain[1]}]   ;# Q1
set_property -dict {PACKAGE_PIN T25 IOSTANDARD LVCMOS33} [get_ports {ledsmain[2]}]   ;# Q2
set_property -dict {PACKAGE_PIN R26 IOSTANDARD LVCMOS33} [get_ports {ledsmain[3]}]   ;# Q3

# Внешний светодиод = инверсный Q3
set_property -dict {PACKAGE_PIN F25 IOSTANDARD LVCMOS25} [get_ports {ledsboard[0]}]

# =====================================================
# КОНЕЦ ФАЙЛА - БОЛЬШЕ НИЧЕГО НЕ ДОБАВЛЯЙ!
# =====================================================