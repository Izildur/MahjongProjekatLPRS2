#  Simulation Model Generator
#  Xilinx EDK 14.6 EDK_P.68d
#  Copyright (c) 1995-2012 Xilinx, Inc.  All rights reserved.
#
#  File     top_level_ports_wave.tcl (Tue May 15 09:12:22 2018)
#
if { [info exists PathSeparator] } { set ps $PathSeparator } else { set ps "/" }
if { ![info exists tbpath] } { set tbpath "${ps}system" }

wave add $tbpath${ps}RS232_Uart_1_sout -into $id 
wave add $tbpath${ps}RS232_Uart_1_sin -into $id 
wave add $tbpath${ps}RESET -into $id 
wave add $tbpath${ps}CLK_P -into $id 
wave add $tbpath${ps}CLK_N -into $id 
wave add $tbpath${ps}my_peripheral_0_DIP_Data_pin -into $id 
wave add $tbpath${ps}my_peripheral_0_LED_Data_pin -into $id 
wave add $tbpath${ps}vga_periph_0_clk_i_pin -into $id 
wave add $tbpath${ps}vga_periph_0_vga_vsync_o_pin -into $id 
wave add $tbpath${ps}vga_periph_0_blank_o_pin -into $id 
wave add $tbpath${ps}vga_periph_0_vga_hsync_o_pin -into $id 
wave add $tbpath${ps}vga_periph_0_pix_clock_o_pin -into $id 
wave add $tbpath${ps}vga_periph_0_psave_o_pin -into $id 
wave add $tbpath${ps}vga_periph_0_sync_o_pin -into $id 
wave add $tbpath${ps}vga_periph_0_red_o_pin -into $id 
wave add $tbpath${ps}vga_periph_0_green_o_pin -into $id 
wave add $tbpath${ps}vga_periph_0_blue_o_pin -into $id 

