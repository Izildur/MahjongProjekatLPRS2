#  Simulation Model Generator
#  Xilinx EDK 14.6 EDK_P.68d
#  Copyright (c) 1995-2012 Xilinx, Inc.  All rights reserved.
#
#  File     vga_periph_mem_0_wave.tcl (Tue May 15 09:12:22 2018)
#
#  Module   system_vga_periph_mem_0_wrapper
#  Instance vga_periph_mem_0
#  Because EDK did not create the testbench, the user
#  specifies the path to the device under test, $tbpath.
#
if { [info exists PathSeparator] } { set ps $PathSeparator } else { set ps "/" }
if { ![info exists tbpath] } { set tbpath "${ps}system" }

# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_ACLK -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_ARESETN -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_AWADDR -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_AWVALID -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_WDATA -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_WSTRB -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_WVALID -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_BREADY -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_ARADDR -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_ARVALID -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_RREADY -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_ARREADY -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_RDATA -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_RRESP -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_RVALID -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_WREADY -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_BRESP -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_BVALID -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_AWREADY -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_AWID -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_AWLEN -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_AWSIZE -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_AWBURST -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_AWLOCK -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_AWCACHE -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_AWPROT -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_WLAST -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_BID -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_ARID -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_ARLEN -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_ARSIZE -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_ARBURST -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_ARLOCK -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_ARCACHE -into $id
# wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_ARPROT -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_RID -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}S_AXI_RLAST -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}intr_s -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}clk_i -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}reset_n_i -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}direct_mode_i -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}display_mode_i -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}vga_hsync_o -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}vga_vsync_o -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}blank_o -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}pix_clock_o -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}psave_o -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}sync_o -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}red_o -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}green_o -into $id
  wave add $tbpath${ps}vga_periph_mem_0${ps}blue_o -into $id

