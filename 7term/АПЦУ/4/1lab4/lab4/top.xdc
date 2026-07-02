# Конфигурация напряжения
set_property CFGBVS VCCO [current_design]
set_property CONFIG_VOLTAGE 3.3 [current_design]

# Тактовый сигнал 200 МГц (дифференциальный)
set_property PACKAGE_PIN R3 [get_ports sysclk_p]
set_property IOSTANDARD LVDS_25 [get_ports sysclk_p]
set_property PACKAGE_PIN P3 [get_ports sysclk_n]
set_property IOSTANDARD LVDS_25 [get_ports sysclk_n]
create_clock -add -name sys_clk_pin -period 5.00 -waveform {0 2.5} [get_ports sysclk_p]

# DIP-переключатели (для данных D[3:0])
set_property PACKAGE_PIN R8 [get_ports {dipswitch[0]}]
set_property IOSTANDARD SSTL15 [get_ports {dipswitch[0]}]
set_property PACKAGE_PIN P8 [get_ports {dipswitch[1]}]
set_property IOSTANDARD SSTL15 [get_ports {dipswitch[1]}]
set_property PACKAGE_PIN R7 [get_ports {dipswitch[2]}]
set_property IOSTANDARD SSTL15 [get_ports {dipswitch[2]}]
set_property PACKAGE_PIN R6 [get_ports {dipswitch[3]}]
set_property IOSTANDARD SSTL15 [get_ports {dipswitch[3]}]

# Кнопки (для управляющих сигналов)
set_property PACKAGE_PIN P6 [get_ports {pushbuttons[0]}]
set_property IOSTANDARD LVCMOS15 [get_ports {pushbuttons[0]}]
set_property PACKAGE_PIN R5 [get_ports {pushbuttons[1]}]
set_property IOSTANDARD SSTL15 [get_ports {pushbuttons[1]}]
set_property PACKAGE_PIN U6 [get_ports {pushbuttons[2]}]
set_property IOSTANDARD SSTL15 [get_ports {pushbuttons[2]}]
set_property PACKAGE_PIN U5 [get_ports {pushbuttons[3]}]
set_property IOSTANDARD SSTL15 [get_ports {pushbuttons[3]}]
set_property PACKAGE_PIN T5 [get_ports {pushbuttons[4]}]
set_property IOSTANDARD SSTL15 [get_ports {pushbuttons[4]}]

# Светодиоды на основной плате (выходы Q[3:0])
set_property PACKAGE_PIN M26 [get_ports {ledsmain[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {ledsmain[0]}]
set_property PACKAGE_PIN T24 [get_ports {ledsmain[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {ledsmain[1]}]
set_property PACKAGE_PIN T25 [get_ports {ledsmain[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {ledsmain[2]}]
set_property PACKAGE_PIN R26 [get_ports {ledsmain[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {ledsmain[3]}]

# Светодиод на внешней плате (выход Q3_n)
set_property PACKAGE_PIN F25 [get_ports {ledsboard[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {ledsboard[0]}]