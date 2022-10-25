### Introduction

The R-Car Series, 3rd Generation are SoCs that feature the basic functions for next-generation car navigation systems. 

![image](https://user-images.githubusercontent.com/33512027/197733514-d6b37727-cfe2-42ee-95ed-4f16f0eb998c.png)

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

### Internal Bus Module

- AXI-bus
  - On-chip main bus
    - Bus protocol: AXI3 with QoS control
    - Frequency: 400 MHz
    - Bus width: 512 bits/256 bits/128 bits
  - On-chip CPU bus
    - Bus protocol: AXI3
    - Frequency: 800 MHz
    - Bus width: 128 bits
- Direct Memory Access Controller for System (SYS-DMAC) 
  - Address space: 4 GBytes on architecture
  - Data transfer length: Byte, word (2 Bytes), longword (4 Bytes), 8 Bytes, 16 Bytes, 32 Bytes and 64 Bytes
  - Maximum number of transfer times: 16,777,215 times
  - Transfer request: Selectable from on-chip peripheral module request and auto request
  - Bus mode: Selectable from normal mode and slow mode
  - Priority: Selectable from fixed channel priority mode and round-robin mode
  - Interrupt request: Supports interrupt request to CPU at the end of data transfer
  - Repeat function: Automatically resets the transfer source, destination, and count at the end of DMA transfer (by descriptor function)
  - Descriptor function (each channel) supported
  - MMU (each channel) supported
  - Channel bandwidth arbiter (each channel)
- Boot
  - System startup with selectable boot mode at power-on reset
  - Either external ROM boot (area 0)* or on-chip ROM boot can be selected through MD pin on development chip.
  - In on-chip ROM boot, RPC-IF or QSPI serial ROM boot is supported.
  - Program downloaded to internal memory (System RAM)
  - Autorun function for the downloaded program
  - Secure boot supported. Integrity check of boot image is proceeded before executing.
  - On secure chip, boot operation is restricted to on-chip ROM boot mode.
  - About detail information of BOOT, refer to Section 19 (Boot) and Appendix B (Active sequence).
- Realtime Direct Memory Access Controller (RT-DMAC)
  - 16 channels for Realtime domain
  - Address space: 4 GBytes on architecture
  - Data transfer length: Byte, word (2 Bytes), longword (4 Bytes), 8 Bytes, 16 Bytes, 32 Bytes and 64 Bytes
  - Maximum number of transfer times: 16,777,215 times
  - Transfer request: Selectable from on-chip peripheral module request and auto request
  - Bus mode: Selectable from normal mode and slow mode
  - Priority: Selectable from fixed channel priority mode and round-robin mode
  - Interrupt request: Supports interrupt request to CPU at the end of data transfer
  - Repeat function: Automatically resets the transfer source, destination, and count at the end of DMA transfer (by descriptor function)
  - Descriptor function (each channel) supported
  - MMU (each channel) supported
  - Channel bandwidth arbiter (each channel)
- Direct Memory Access Controller for Audio (Audio-DMAC)
  - 32 channels
  - asdm0: 16 channels
  - asdm1: 16 channels
  - Address space: 4 GBytes on architecture
  - Data transfer length: Byte, word (2 Bytes), longword (4 Bytes), 8 Bytes, 16 Bytes, 32 Bytes and 64 Bytes
  - Maximum number of transfer times: 16,777,215 times
  - Transfer request: Selectable from on-chip peripheral module request and auto request
  - Bus mode: Selectable from normal mode and slow mode
  - Priority: Selectable from fixed channel priority mode and round-robin mode
  - Interrupt request: Supports interrupt request to CPU at the end of data transfer
  - Repeat function: Automatically resets the transfer source, destination, and count at the end of DMA transfer (by descriptor function)
  - Descriptor function (each channel) supported
  - MMU (each channel) supported
  - Channel bandwidth arbiter (each channel)
- Audio-DMACPeripheral-Peripheral
  - Audio-DMAC (for transfer from Peripheral to Peripheral)
  - 4 channels for audio domain
  - Data transfer length: longword (4 Bytes)
  - Transfer count: Transfer count is not specified (DMA transfer is made from the transferstart to transfer-stop settings.)
  - Transfer request: Selectable from on-chip audio peripheral module request
  - Priority: round-robin mode
  - Interrupt request: not supports interrupt request to CPU at the end of data transfer
- IPMMU
  - An IPMMU is a memory management unit (MMU) which provides address translation and access protection functionalities to processing units and interconnect networks. 

- Interrupt Controller (INTC) 
  - INTC-AP (For All Products; for APSystem core CortexA57/Cortex-A53)
    - 7 interrupt pins which can detect external interrupts
    - Max. 480 shared peripheral interrupts supported
    - Fall/rise/high level/low level detection is selectable
    - On-chip peripheral interrupts: Priority can be specified for each module
    - 16 software interrupts that have been generated and 6 private peripheral interrupts supported
    - 32-level priority selectable
    - Trust Zone supported
  - INTC-RT
    - 7 interrupt pins which can detect external interrupts
    - Fall/rise/high level/low level detection is selectable
    - On-chip peripheral interrupts: Priority can be specified for each module
    - Max. 384 peripheral interrupts supported
  - Multifunctional Interface (MFIS)
    -  Interrupt generation between SYS domain and RT domain
    -  Fifteen external source bits for controlling 32-K types of interrupts
    -  Lock function for exclusive access supported
    -  Error checking function control and arbitrate error signal from other module. 








