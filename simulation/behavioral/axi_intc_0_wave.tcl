#  Simulation Model Generator
#  Xilinx EDK 14.6 EDK_P.68d
#  Copyright (c) 1995-2012 Xilinx, Inc.  All rights reserved.
#
#  File     axi_intc_0_wave.tcl (Tue May 15 09:12:22 2018)
#
#  Module   system_axi_intc_0_wrapper
#  Instance axi_intc_0
#  Because EDK did not create the testbench, the user
#  specifies the path to the device under test, $tbpath.
#
if { [info exists PathSeparator] } { set ps $PathSeparator } else { set ps "/" }
if { ![info exists tbpath] } { set tbpath "${ps}system" }

# wave add $tbpath${ps}axi_intc_0${ps}S_AXI_ACLK -into $id
# wave add $tbpath${ps}axi_intc_0${ps}S_AXI_ARESETN -into $id
# wave add $tbpath${ps}axi_intc_0${ps}S_AXI_AWADDR -into $id
# wave add $tbpath${ps}axi_intc_0${ps}S_AXI_AWVALID -into $id
  wave add $tbpath${ps}axi_intc_0${ps}S_AXI_AWREADY -into $id
# wave add $tbpath${ps}axi_intc_0${ps}S_AXI_WDATA -into $id
# wave add $tbpath${ps}axi_intc_0${ps}S_AXI_WSTRB -into $id
# wave add $tbpath${ps}axi_intc_0${ps}S_AXI_WVALID -into $id
  wave add $tbpath${ps}axi_intc_0${ps}S_AXI_WREADY -into $id
  wave add $tbpath${ps}axi_intc_0${ps}S_AXI_BRESP -into $id
  wave add $tbpath${ps}axi_intc_0${ps}S_AXI_BVALID -into $id
# wave add $tbpath${ps}axi_intc_0${ps}S_AXI_BREADY -into $id
# wave add $tbpath${ps}axi_intc_0${ps}S_AXI_ARADDR -into $id
# wave add $tbpath${ps}axi_intc_0${ps}S_AXI_ARVALID -into $id
  wave add $tbpath${ps}axi_intc_0${ps}S_AXI_ARREADY -into $id
  wave add $tbpath${ps}axi_intc_0${ps}S_AXI_RDATA -into $id
  wave add $tbpath${ps}axi_intc_0${ps}S_AXI_RRESP -into $id
  wave add $tbpath${ps}axi_intc_0${ps}S_AXI_RVALID -into $id
# wave add $tbpath${ps}axi_intc_0${ps}S_AXI_RREADY -into $id
  wave add $tbpath${ps}axi_intc_0${ps}Intr -into $id
  wave add $tbpath${ps}axi_intc_0${ps}Irq -into $id
  wave add $tbpath${ps}axi_intc_0${ps}Interrupt_address -into $id
# wave add $tbpath${ps}axi_intc_0${ps}Processor_ack -into $id
  wave add $tbpath${ps}axi_intc_0${ps}Processor_clk -into $id
  wave add $tbpath${ps}axi_intc_0${ps}Processor_rst -into $id
  wave add $tbpath${ps}axi_intc_0${ps}Interrupt_address_in -into $id
# wave add $tbpath${ps}axi_intc_0${ps}Processor_ack_out -into $id

