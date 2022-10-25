### Introduction

![image](https://user-images.githubusercontent.com/33512027/197713937-065f0a16-5a92-41cd-b2c7-a87a435a6560.png)

The R-Car Series, 3rd Generation are SoCs that feature the basic functions for next-generation car navigation systems. 

The M3-N includes: 
- Two 1.5-GHz Arm® Cortex®-A57 MPCore™ cores,
- Memory controller for LPDDR4-3200/DDR3-2133/DDR3L-1866 with 32 bits × 1 channel,
- 1 channel for HDMI1.4b output (option) and 1channel for RGB888 output and 1channel for LVDS,
- 2 channels MIPI-CSI2 Video Input, 2channels digital Video Input,
- Serial ATA interface,
- USB3.0 × 1ch and USB2.0 × 2ch interfaces, 
- 800-MHz Arm® Cortex®-R7 core (option),
- Three-dimensional graphics engines,
- Sound processing units,
- MediaLB interface,
- SD card host interface (option),
- PCI Express interface, 
- Video processing units,
- CAN interface, and
- EthernetAVB interface. 

### Arm Core

- System CPU Cortex-A57
  - Arm Cortex-A57 Dual MPCore 1.5 GHz – 1.8 GHz (with DVFS control)
  - L2 cache 1 Mbytes (ECC)
  - NEON™/VFPv4 supported
  - Security extension supported
  - Virtualization supported
  - Armv8 architecture
- Debug and Trace
  - JTAG/SWD I/F supported
  - ETM-A57/A53 supported (each CPU)
  - ETF 16 KBytes for program flow trace (each cluster)

### Arm Realtime Core

- Arm Realtime Core Cortex-R7
  - Arm Cortex-R7 800 MHz
  - L1 I/D cache 32/32 Kbytes (ECC)
  - I-TCM/D-TCM (32/32 Kbytes) (ECC)
  - Dual Lock-Step supported (safety function option)
  - VFPv3 supported
  - Armv7 architecture
- Debug and Trace
  - JTAG/SWD I/F supported
  - ETM-R7 supported
  - ETF 4 KBytes for program flow trace

### CPU Core Peripherals

- Clock Pulse Generator (CPG)
  - Maximum Cortex-A57 clock: 1.5 GHz ~ 1.8 GHz
  - Maximum 3DGE clock: 700 MHz
  - Maximum Cortex-R7 clock: 800 MHz
  - Maximum AXI-bus clock: 400 MHz
  - Maximum SDRAM bus clock: 1600 MHz (LPDDR4-3200)
  - Maximum media clock: 400 MHz
  - System-CPU shut down mode control supported
- System Controller (SYSC)
  - Cortex-A57 (with independent shutting down of CPUs 0, 1 and SCU+L2 cache)
  - Cortex-R7: 
  - Low leakage standby mode supported. 
- Reset
  - Includes one reset-signal external output port for external modules
  - Includes Boot Address Register etc. 
- Pin function controller (PFC)
  - Setting multiplexed pin functions for LSI pins. Function of the LSI pin selectable by setting the registers in the PFC module
  - Module selection: Enable and disable the functions of LSI pins to which pin functions from multiple pin groups are assigned by setting the registers in the PFC module.
  - Pull-up/down control for each LSI pin: On/off and up/down of the pull register on each LSI pin can be controlled by setting the registers in the PFC module. 
  - Control of SDIO functions: SDIO functions, including the driving ability of pins for the SDIF, can be controlled by setting registers of the PFC.
- General-purpose I/O (GPIO)
  - General-purpose I/O ports
  - Supports GPIO interrupts
- Thermal sensor / Chip Internal Voltage Monitor (THS/CIVM) 
  - 3 channels of thermal sensor
  -  Programmable 3 temperature level for the sensor, to indicate the temperature level
  -  Interrupt when the temperature reaches programmed

### External Bus Module

- External Bus Controller for EXBus (LBSC)
  - EX-BUS interface: max. 16-bit bus
  - Frequency: 66 MHz
  - External area divided into several areas and managed
    - Allocation to space of area 0, area 1.
    - Area 0 supports 1-MByte memory space (startup mode).
    - I/F settings, bus width settings, and wait state insertion are possible for each area
  - SRAM interface
    - Wait states can be inserted through register settings. Period of waiting is set in cycle unit, and the maximum value is 15.
    - EX_WAIT pin can be used for wait state insertion
    - Connectable bus widths: 16 bits or 8 bits
  - Supports external buffer enable/direction control
  - Supports Burst ROM interface
  - Supports Byte-control SRAM interface
- External Flash Controller 
  - Supports RPC-IF (Reduced Pin Count interface) flash memory or QSPI flash memory. Maximum Frequency 160 MHz (320MB/s) for RPC-IF, 80 MHz (80MB/s)* for QSPI (QSPI0) . Dual QSPI operation for two 4-bit serial flash memories is also available; 80 MHz (160MB/s)
for Dual QSPI (QSPI0+QSPI1). 
- External Bus Controller for LPDDR4/DDR3/DDR3L SDRAM (DBSC4) 
  - 1 channel (32-bit bus mode)
  - LPDDR4-3200 and DDR3L-1866 and DDR3-2133 can be connected directly.
  - Memory Size: Up to 4GB
  - Auto Refresh/Self Refresh/Partial Array Self Refresh supported
  - Auto Pre-charge Mode
  - DDR Back Up supported
  - Cache memory for DDR-Memory access efficiency
  - Decompression of visual near lossless compressed image
  - Memory access protection for secure/safety regions

