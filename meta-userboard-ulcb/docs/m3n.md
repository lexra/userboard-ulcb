### Introduction

![image](https://user-images.githubusercontent.com/33512027/197733514-d6b37727-cfe2-42ee-95ed-4f16f0eb998c.png)

The R-Car Series, 3rd Generation are SoCs that feature the basic functions for next-generation car navigation systems. The M3-N includes: 
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
- Ethernet AVB interface. 

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
  - Cortex-R7
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

#### AXI-bus
- On-chip main bus
  - Bus protocol: AXI3 with QoS control
  - Frequency: 400 MHz
  - Bus width: 512 bits/256 bits/128 bits
- On-chip CPU bus
  - Bus protocol: AXI3
  - Frequency: 800 MHz
  - Bus width: 128 bits
#### Direct Memory Access Controller for System (SYS-DMAC) 
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
#### Boot
  - System startup with selectable boot mode at power-on reset
  - Either external ROM boot (area 0)* or on-chip ROM boot can be selected through MD pin on development chip.
  - In on-chip ROM boot, RPC-IF or QSPI serial ROM boot is supported.
  - Program downloaded to internal memory (System RAM)
  - Autorun function for the downloaded program
  - Secure boot supported. Integrity check of boot image is proceeded before executing.
  - On secure chip, boot operation is restricted to on-chip ROM boot mode.
  - About detail information of BOOT, refer to Section 19 (Boot) and Appendix B (Active sequence).
#### Realtime Direct Memory Access Controller (RT-DMAC)
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
#### Direct Memory Access Controller for Audio (Audio-DMAC)
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
#### Audio-DMACPeripheral-Peripheral
  - Audio-DMAC (for transfer from Peripheral to Peripheral)
  - 4 channels for audio domain
  - Data transfer length: longword (4 Bytes)
  - Transfer count: Transfer count is not specified (DMA transfer is made from the transferstart to transfer-stop settings.)
  - Transfer request: Selectable from on-chip audio peripheral module request
  - Priority: round-robin mode
  - Interrupt request: not supports interrupt request to CPU at the end of data transfer
#### IPMMU
  - An IPMMU is a memory management unit (MMU) which provides address translation and access protection functionalities to processing units and interconnect networks. 

#### Interrupt Controller (INTC) 
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

### Internal Memory

- System RAM
  - RAM of 384 KBytes

### Graphics Units

#### 3D Graphics Engine (3DGE) 
  - Imagination Technologies PowerVR Series 7XE GE7800
  - Max. Freq. 700 MHz with DVFS control
  - Drastically performance improvements for sophisticated graphics and GPU computer
  - Reducing power consumption even further through advanced power saving mechanisms
  - Lowest memory bandwidth in the industry with compression technologies
  - Ultra HD deep color GPU Support APIs: OpenGL ES 3.1, (OpenCL 1.2 EP)
  - 2.8Gpix/s 233Mpoly/s 44.8GFLOPS

#### 2D Graphics Engine (D/AVE HD) 
  - TES Corporation D/AVE HD
  - Max. Freq. 266/400 MHz as core clock frequency
  - R-Car H3, R-Car M3-W, R-Car M3-W+: 266 MHz
  - R-Car D3, M3-N, and E3: 400 MHz
    - Supporting high quality 2D rendering for displays up to 4K × 4K
    - High render quality
    - Hardware accelerated primitives
  - Fast clear/rectangle fill
  - Lines
  - Triangles and Quadrangles
  - Beziers
  - Advanced Blit operations supporting scaling, stretching, rotating, coloring and alpha-blending
  - Convolution Filtering

#### Display Unit (DU)
  - Display channel
    - 3 independently controllable channels
  - Interface
    - HDMI 1channel
    - LVDS 2 channels
    - Digital RGB 1 channel
  - LVDS interface (per channel)
    - Output: compliant with TIA/EIA-644; five pairs of differential output (four pairs of data and one pair of clock)
    - Operating frequency: Dotclk 148.5 MHz
  - HDMI
    - Support HDMI 1.4 class transfer rate, up to 3D format 1080p60/4Kp30
    - Dotclk 297 MHz
  - Screen size and number of composite planes per channel
    - Maximum screen size: 3840 × 2160
    -  Number of planes specifiable: 5 (VSP2 processing)
    -  Number of planes specifiable: 1 (DU)
  - CRT scanning method
    - Non-interlaced 
  - Synchronization method
    - Master
  - Internal color palette (VSP2)
    - Includes four color palette planes which can display 256 of 260 thousands colors at the same time. 
  - Output display numbers
    - Three output channels (resolutions for different displays)
    - Output on rising and falling edges of the synchronizing signal (resolution for the same display)
    - 8-bit precision for each RGB color
  - Blending ratio settings (VSP2)
    - Number of color palette planes with blending ratio: 4
  - Dot clock
    - Switchable between external input and internal clock
  - Color management (CMM)
    - 1D LUT, 3D LUT, 1D Histogram
    - Color space conversion. Following functions will be supported by Renesas software portfolio.
    - γ correction, gain correction
    - Applies correction of color (skin color adjustment and color correction set in memory) in terms of color phase, brightness, and chromaticity for a specified range of colors or for the full range of colors

#### TCON
  - 2 channels (TCON0, TCON1) (TCON0 is output via LVDS interface)
  - Modulate the various timing signal for LCD panel driver with RGB parallel output interface

#### Display Output Checker (DOC) 
  - The DOC allows to check whether the display content (warning, light, and so on) is correctly output by the Display Unit (DU). 

#### Video Input Module (VIN)
- MIPI-CSI2 interface
  - 2 channels (4lane × 1channel, 2lane × 1channel)
  - Interleaving by 4 VC (virtual channel) supported
  - Filtering by DT (data type) supported
  - YUV422 8/10bit, RGB888, Embedded 8bit, User Defined 8bit are supported
  - 1.5Gbps/Lane
- Digital interface
  - 2 channels (RGB/YCbCr)
  - Dotclk 100 MHz
  - ITU-R BT.601 interface: 8-, 10- (same size only (not scaling)), 12- (same size only (not scaling)) 16-, 20- (same size only (not scaling)) or 24-bit (same size only (not scaling)), YCbCr422, 18-bit RGB666, 24-bit RGB888
  - ITU-R BT.656 interface: 8-, 10- (same size only (not scaling)), or 12-bit (same size only (not scaling)) YCbCr422
  - ITU-R BT.1358 interface: 16-, 20- (same size only (not scaling)), or 24-bit (same size only (not scaling)) YCbCr422
  - ITU-R BT.709 interface: 8-, 10- (same size only (not scaling)), 12- (same size only (not scaling)) 16-, 20- (same size only (not scaling)) or 24-bit (same size only (not scaling)) YCbCr422, 18-bit RGB666, 24-bit RGB888 About Digital RGB channel usage combination, Refer as follows cases.
    - CASE1 VIN-A8bit + VIN-B8/12/16
    - CASE2 VIN-A12bit + VIN-B8/12/16
    - CASE3 VIN-A16bit + VIN-B8/12
- Capturing function
  - Up to 8 input images can be captured (using VC, DT filtering)
- Clipping function
  - Up to 4096 × 4096
- Horizontal scaling
  - Up to two times, but only scaling down is possible for HD1080i or HD1080p data. (one input only)
- Vertical scaling
  - Up to three times, but only scaling down is possible for HD1080i or HD1080p data. (one input only)
- Output format
  - RGB-565, ARGB-1555, ARGB8888, YCbCr422, RGB888, YCbCr420 YC separation, and extraction of the Y component

#### IMR-LX4

- IMR-LX4-M3W/M3N
- 2 Image Renders for camera image distortion correction application
- Pixel Performance: 400Mpixel/sec (peak, @400 MHz) per one IMR-LX4-M3W/M3N

### Video Processing

#### Video Signal Processor (VSPI) 

VSPI has the following features: 1 set of VSPI is integrated. 500 Mpix/s process rate per 1 VSPI. Supports 4K (3840 pixels × 2160 lines) processing. 

- Supports Various Data Formats and Conversion
  - Supports YCbCr444/422/420, RGB, αRGB, αplane
  - Color space conversion and changes to the number of colors by dithering
  - Color keying
  - Supports combination between pixel alpha and global alpha
  - Supports generating pre multiplied alpha
- Video Processing
  - Up and down scaling with arbitrary scaling ratio
  - Super resolution processing
  - Image rotation/reversal function: Reverses an image vertically/horizontally or rotates it by 90°/270°
- Picture Quality/Color Correction with 1D/3D Look Up Table (LUT)
  - Hue, brightness, and saturation adjustment
  - 1D and 2D histogram
    - Following functions will be supported by Renesas software portfolio.
  - Dynamic γ correction and gain correction
  - Correction of color (to adjust skin tones or colors in memory)
- Visual near lossless image compression supported
  - 50% of bandwidth is diminished 

#### Video Signal Processor (VSPD)

VSPD has the following features. 1 set of VSPD is integrated. Supports 4K (3840 pixels × 2160 lines) resolution

- Supports Various Data Formats and Conversion
  - Supports YCbCr444/422/420, RGB, αRGB, αplane
  - Color space conversion and changes to the number of colors by dithering
  - Color keying
  - Supports combination between pixel alpha and global alpha
  - Supports generating pre multiplied alpha.
- Video processing
  - Blending of 5 picture layers and raster operations (ROPs)
  - Vertical flipping in case of output to memory
- Direct connection to display module
  - Supports 4096 pixels in horizontal direction
  - Writing back image data which is transferred to Display Unit (DU) to memory
- Supports DISCOM function

#### Video Signal Processor (VSPDL)

VSPDL has the following features. 1 set of VSPDL is integrated. 

- Supports various data formats and conversion
  - Supports YCbCr444/422/420, RGB, αRGB, αplane
  - Color space conversion and changes to the number of colors by dithering
  - Color keying
  - Supports combination between pixel alpha and global alpha
  - Supports generating pre multiplied alpha
- Video processing
  - Blending of five picture layers and raster operations (ROPs)
- Direct connection to display module
  - Supports two display output interfaces
  - Supports 2048 pixels in horizontal direction
  - Writing back image data which is transferred to Display Unit (DU) to memory
- Supports DISCOM function

#### Video Codec Processor (VCP4) 

The VCP4 is a multi-codec module which provides encoding and decoding capabilities
on the basis of multiple video coding schemes, e.g., H.265/HEVC, H.264/AVC, MPEG4, MPEG-2 and VC-1. 

This IP (Intellectual Property) is a multi codec that processes the frame or each field by
controlling software for VCP4 executed on host CPU. 

- The VCP4 supported for multiple codecs: 
  - H.265/HEVC MP (Main Profile) decoding
  - H.264/MPEG-4 AVC HP (High Profile) and MVC SHP (Stereo High Profile) encoding and decoding
  - H.262/MPEG-2 MP (Main Profile) decoding
  - MPEG-4 ASP (Advanced Simple Profile) decoding
  - VC-1 SP/MP/AP (Simple, Main, Advanced Profile) decoding
  - H.263 Baseline decoding
  - DivX decoding (Home Theater*, HD1080*, and Plus HD Profiles)
  - RealVideo8/9/10 decoding
  - VP8 encoding and decoding
  - VP9 (Profile 0) decoding 
- Supported for up to 4K resolutions (H.265, H.264 and VP9)
  - Multiple channel processing:
    - (H.264/H.265 1920 × 1080p × 120 fps),
    - or (H.264/H.265 1920 × 1080p x 30 fps) + (RealVideo8/9/10 1920 × 1080p × 30 fps),
    - or (H.264/H.265 1920 × 1080p × 60 fps) + (Others 1920 × 1080p × 30 fps),
    - or (H.264/H.265 1920 × 1080p × 30 fps) + (Others 1920 × 1080p × 60 fps) 
    - (VP9 1920 × 1080p × 60 fps),
    - or (VP9 1920 × 1080p × 30 fps) + (H.264 1920 × 1080p × 60 fps),
    - or (VP9 1920 × 1080p × 30 fps) + (Others 1920 × 1080p × 30 fps)
- Data handling on a picture-by-picture basis
  - Encodes/decodes data one picture (frame or field) at a time.
- High picture quality
  - Supports the H.264 high-efficiency coding tools (CABAC, 8 × 8 frequency conversion, and quantization matrix). 
  - High-efficiency motion vector detection by a combination of discrete search and trace search
  - Optimal-mode selection by Rate-Distortion (RD) cost evaluation
  - Picture quality control based on activity analysis results which match visual models
- Low power dissipation
  - Dynamically disables the clocks for the entire VCP4.
  - Dynamically disables the clocks for individual submodules.
- Includes its own reference data cache
- Lossless image compression for reference picture is supported

Use the software from Renesas to handle VCP4 functions. 

#### Video Decoding Processor for inter-device video transfer (iVDP1C)

- Low-latency decoder H.264/AVC, JPEG
- Color format 4:2:0/4:2:2
- Bit depth 8/10/12bits
- Performance:
  - 1280 pixels × 960 lines × 30 frames/second × 4 channels
- Max resolution:
  - 1920 pixels × 1088 lines
- Includes its own reference data cache
- Lossless image compression for reference picture is supported if bit depth is 8 bits

Use the software from Renesas to handle iVDP1C functions.

#### Fine Display Processor (FDP1) 

The FDP1 is the de-interlacing module which converts the interlaced video to progressive video, and has the following features. 

- Supports 1 channel
  - 500 Mpix/s for output performance per 1 FDP1
- Supports various data formats
  - Input: YCbCr444/422/420
  - Output: YCbCr444/422/420 and RGB/αRGB
- 8190 pixels x 8190 lines video processing performance
- High image quality de-interlacing algorithm
  - Motion adaptive de-interlacing
  - Accurate still detection
  - Diagonal line interpolation (DLI)
- Visual near lossless image compression supported and 50% of bandwidth is diminished

### Sound Interface

#### Audio DSP (ADSP)

- On-chip Audio-DSP (Dual MACs can operate as 32 × 16-bit / 24 × 24-bit)
- 32-bit RISC architecture with 7-stage pipeline
- 24-bit audio processing instructions
- Dual-issue, static super-scalar VLIW
- Instruction RAM 128KB, Data RAM 96KB

#### Digital Radio Interface (DRIF) 

- 4 channels of DRIF
- FIFO capacity: 32 bits × 256 stages for reception
- MSB first or LSB first selectable for data reception
- Synchronization by frame synchronization pulse, or level mode, or left/right channel switch
- Supports mode: Slave mode
- DMA transfer
- Serial format: Supports serial format of IIS, SPI (Slave mode)

#### Sampling Rate Converter Unit (SCU) 

##### Overall specification

- Includes ten SRC modules
  - Supports the quality suitable for audio sound (High-soundquality type) (THD+N -132dB): six modules
  - Supports the quality suitable for voice sound (general-soundquality type) (THD+N -96dB): four modules
- The SRC module is capable of correcting phase change and delay (timing jitter) generated during data transfer over external memories or external devices.
- The channel count conversion unit (CTU), mixer (MIX), and digital mute and volume function (DVC) can be used on two fixed output channels. 

##### Sampling rate conversion (SRC) 

- Capable of asynchronous sampling rate conversion
- Supports resolutions up to 24 bits
- Two kinds of filter type for SRC.
  - Supports the quality suitable for audio sound (High-soundquality type) (THD+N -132dB): Realized the filter by passband - 1dB@0.4575FS, cutoff -18dB@0.5FS.
  - Supports the quality suitable for voice sound (general-soundquality type) (THD+N -96dB): Realized the filter by passband - 1dB@0.4561FS, cutoff -72dB@0.5FS. (Characteristics of each filter is written in the equivalent/up-sampling cases.)
- Automatically generates antialiasing filter coefficients
- For monaural to eight-channel sound sources

##### Channel count conversion unit (CTU) 

- Downmixing and splitter functions
  - Conversion of eight input channels into four output channels
  - Conversion of six input channels into two output channels
  - Conversion of two input channels into four sets of two output channels
  - Conversion of one input channel into eight sets of one output channel
  - No conversion

##### Mixer (MIX)

- Mixing (adds) two to four sources into one
- Ratio for adding sources is selectable
- Ratio is dynamically changeable
- Mixing with volume ramp is available (ramp period is selectable)

##### Digital volume and mute function (DVC)

- Volume control function including digital volume, volume ramp, and zero-crossing mute
- The digital volume function is specified by a 24-bit fixed-point value within the range from 0 to 8 times (mute, or -120 to 18 dB)
- The volume ramp function can be used for soft mute, fade-in, fadeout, or desired volume adjustment
- The volume ramp period can be changed within the sampling range from the 0th to 23rd power of 2
- The zero-crossing mute function silences the sound at the zerocrossing point of the audio data

#### Serial Sound Interface Unit (SSIU) 

##### Overall specification

- Includes ten SSI modules functioning as interfaces with external devices.
  - Supports short and long formats for monaural
  - Supports TDM format (six modules of ten modules can be used for this function) 
  - Max. 16 independent monaural sound sources in a TDM format can be in TDM format.

##### Serial sound interface (SSI)

- 10 channels
- Max SCK frequency 15.1 MHz (for slave input) or 12.5 MHz (for master output)
- Operating mode: non-compressed mode (Not support compressed mode)
- Supports versatile serial audio formats (I2S/left justified/right justified)
- Supports master/slave functions
- Programmable word clock, bit clock generation functions
- Multichannel format functions (up to four channels)
- Supports 8-/16-/18-/20-/22-/24-bit data formats
- Supports TDM mode
- Supports WS continue mode
- The DMA controller or interrupts control the transfer of data to and from the SSI module.
- Supports short and long frames for monaural data (valid data lengths are 8 and 16 bits)
- Up to nine independent clock signals can be input.

#### MediaLB I/F (MLBIF)

- MOST interface
  - Media Local Bus (MediaLB)
    - MediaLB 3-pin interface (Not support MediaLB 5-pin/6-pin interface)
    - Up to 50-Mbps data transfer rate is supported.
  - Synchronous transfer
  - Asynchronous transfer
  - Isochronous transfer
  - Control transfer 
- Buffer memory for MLBIF

#### Digital Transmission Content Protection (DTCP)

- AKE and the protection of contents is possible.
- Two lines are simultaneously possible.
- Supports MOST-DTCP only
- Supports audio data only

#### Audio Clock Generator (ADG) 

Selection or division of audio clock signals

### Storage

#### USB2.0 Host (EHCI/OHCI)

- 2 channels (Host only1 channel/Host-Function 1channel)
- USB Host (EHCI/OHCI) 2LINK
- (USB3.0 module also can be used as USB2.0)
- Compliance with USB2.0
- USB Function 1LINK
- Supports On-The-Go (OTG) function Rev2.0 complying with 2 protocols:
  - Session Request Protocol (SRP).
  - Host Negotiation Protocol (HNP).
- Compliance with USB2.0 (High-Speed)
- Interrupt request
- Internal dedicated DMA
- Compliance with Battery Charging function Rev1.2:
  - Charging Port (Host): CDP, SDP are supported (Not support DCP).
  - Portable Device (Function) is supported. 

#### USB3.0 Host Controller

- USB 3.0 DRD 1 channel
- Supports SS/HS/FS/LS. xHCI

#### Serial-ATA Gen3

- Serial ATA Standard Rev3.2 supported
- 6.0-Gbps (Gen3) transfer rate supported
  - PHY is shared with PCIE Controller

#### SD Card Host Interface (SDHI)

- 2 channels
- eMMC 5.0 base, Support HS400 class transfer rate
- eMMC controllable
- Data bus: 1/4/8-bit MMC mode (not support SPI mode)
- Support block transfer (not support stream transfer)
- Block size in multiple block transfer: 512 Bytes

#### rawNAND Controller

- The NAND Flash Memory Interface controller implements the function of a high level interface to one NAND flash device. It supports the functionality of the high speed NAND Flash devices described in the ONFI 1.x specifications.

### Network 

#### Controller Area Network Interface (CAN interface)

- 2 interfaces
- Supports CAN specification 2.0B
- Maximum bit rate: 1 Mbps
- Message box
  - Normal mode: 32 receive-only mailboxes and 32 mailboxes for transmission/reception
  - FIFO mode:
    - 32 receive-only mailboxes and 24 mailboxes for transmission/reception, 4-stage FIFO for transmission, and 4-stage FIFO for reception
- Reception
  - Data frame and remote frame can be received.
  - Selectable receiving ID format
  - Selectable overwrite mode (message overwritten) or overrun mode (message discarded)
- Acceptance filter
- Mask can be enabled or disabled for each mailbox.
- Transmission
  - Data frame and remote frame can be transmitted.
  - Selectable transmitting ID format (only standard ID, only extended ID, or both IDs)
  - Selectable ID priority mode or mailbox number priority mode
- Sleep mode for reducing power consumption 

#### CAN-FD

-  2 interfaces
-  8 Mbps (CAN clock 40 MHz)

#### PCIE Controller

- PCI Express Base Specification Revision 2.0
- PHY integrated
- 1 Lane × 2 channels (one of PHY is shared with Serial ATA)

#### EthernetAVB-IF

- Supports IEEE802.1BA, IEEE802.1AS, IEEE802.1Qav and IEEE1722 functions
- Magic packet detection
- Supports Reception Filtering to separate streaming frames from different sources
- Supports interface conforming to IEEE802.3 PHY RGMII (Reduced Gigabit Media Independent Interface)
- RGMII v1.3
- Supports transfer at 1000 Mbps and 100 Mbps.

### Timer

#### RCLK Watchdog Timer

- 1 channel
- Internal 16-bit watchdog timer operated by RCLK
- Programmable overflow time period: more than 1 hour count capable

#### 16-Bit Timer Pulse Unit (TPU) 

- 4-channel 16-bit timers
- Each channel outputs PWM

#### System Watchdog Timer

- 1 channel
- Internal 16-bit watchdog timer
- Programmable overflow time period: more than 1 hour count capable initial counter value 171[s] 

#### Compare Match Timer Type0 (CMT0)

- 2 channels
- 32-bit timer (16 bits/32 bits can be selected)
- Source clock: RCLK clock
- Compare match function provided
- Interrupt requests

#### Compare Match Timer Type1 (CMT1)

CMT2 and CMT3 were the same as CMT1. 

- 8 channels
- 48-bit timer (16 bits/32 bits/48 bits can be selected)
- Source clock: RCLK/system clock
- Compare match function provided
- Interrupt requests 

#### System Timer

- 32-bit timer, 1channel (16 bits/32 bits can be selected)
- Compare match function provided
- Interrupt requests

#### System up-time clock

- 1 channel
- Internal 32-bit timer
- Programmable overflow time period: maximum 24 hours

#### Timer Unit (TMU) 

- 15 channels
- 32-bit timer
- Auto-reload type 32-bit down counter
- Internal prescaler
- Interrupt request
- 2 channels for input capture

### Peripheral Module

#### Secure Engine

- 3 Cores (1 Core for secure world operation, 1 Core for normal world operation and 1 Core for PKA acceleration)
- PKA Engine (RSA, ECC)
- Hash Engine (SHA1, SHA2, MD5)
- Bulk Encryption Engine (DES, 3DES, AES)
- Internal RAM for Secure Engine

#### IIC Bus Interface for DVFS (IIC for DVFS)

- 1 DVFS channel for dedicated buffer.
- Supports single master transmission/reception
- Interrupt request
- Automatic transfer by wakeup / DVFS control 

#### I2C Bus Interface

- 7 channels
- 4 channels for buffers with a slew rate (channel 0, 3, 4, 5 for dedicated buffers)
- 3 channels for LVTTL buffers (channels 1, 2, 6 for ordinary buffers)
- NXP I2C bus interface method supported
- Master/slave functions
- Multi-master functions
- Transfer rate up to 400 kbps supported
- Programmable clock generation from the system clock
- Master and Slave function DMA supported

#### Serial communication interface with FIFO (SCIF)

- 6 channels
- Asynchronous, clock-synchronized modes
- Asynchronous serial communication mode: The SCIF performs serial data communication based on a characterby-character asynchronous system. This feature enables serial data communication with standard asynchronous communication chips that support Universal Asynchronous Receiver/Transmitter (UART) or Asynchronous Communication Interface Adapter (ACIA). There is a choice of eight serial data transfer formats. 
  - Data length: 7 bits or 8 bits
  - Stop bits: 1 bit or 2 bits
  - Parity: Even/odd/none
  - Receive error detection: Parity, framing, and overrun errors
  - Break detection: A break is detected when a framing error lasts for more than 1 frame length at Space 0 (low level). When a framing error occurs, a break can also be detected by reading the RX pin level directly from the serial port register (SCSPTR).
- Clock synchronous serial communication mode: The SCIF performs serial data communication synchronized with a
clock. This feature enables serial data communication with other LSIs that support synchronous communication. There is a single serial data communication format for clock synchronous serial communication.
  - Data length: 8 bits
  - Receive error detection: Overrun errors
- Full-duplex communication capability: The SCIF has an independent transmitter and receiver that enable simultaneous transmission and reception. The transmitter and
receiver both have a 16-stage FIFO buffer structure, enabling continuous serial data transmission and reception.
- On-chip baud rate generator, enabling any bit rate to be selected. The SCIF enables choice of a clock source for transmission/reception:
  - a clock from the on-chip baud rate generator based on the internal clock or an external clock.
- Eight interrupt sources: The SCIF has eight types of interrupt sources. receive-data-ready, receive-FIFO-data-full, break, transmit-FIFO-data-empty, transmitend, receive-error, overrun-error and time-out and enables any of them to be requested independently. 
- DMA data transfer: When the transmit FIFO register is empty or the receive FIFO register has received data, issuing a DMA transfer request activates the DMA controller (DMAC) to execute a data transfer.
- In asynchronous mode using channels 0, 1, 3, and 4, modem control functions (RTS and CTS) are stored.
- RTS and CTS are not implement for SCIF2 and SCIF5
- The amount of data in the transmit/receive FIFO registers and the number of receive errors in receive data in the receive FIFO register are available. 
- In asynchronous mode, a receive data ready (DR) or a timeout error (TO) can be detected during reception.

#### Clock-Synchronized Serial Interface with FIFO (MSIOF)

- 4 channels
- Internal 32-bit × 64-stage transmit FIFOs/internal 32-bit × 256-stage receive FIFOs
- Supports master and slave modes
- Internal prescaler
- Supports serial formats: IIS, SPI (master and slave modes)
- Interrupt request, DMAC request

#### High Speed Serial Communication Interface with FIFO (HSCIF) 

- 5 channels
- Asynchronous serial communication mode
- Capable of full-duplex communication
- On-chip baud rate generator, enabling any bit rate to be selected
- Eight interrupt sources
- DMA data transfer
- Modem control functions (HRTS# and HCTS#) are stored.
- The amount of data in the transmit/receive FIFO registers and the number of receive errors in receive data in the receive FIFO register are available. 
- A receive data ready (DR) or a timeout error (TO) can be detected during reception.

#### PWM

- 7 channels
- High-level width (10 bits) of PWM output can be set. 
- Output cycle periods (10 bits) of PWM can be set.
- Periods in the range from two to 224 × 1023 cycles of the Pϕ clock can be set.
- Continuous pulse or single pulse output selectable

#### Gyro ADC I/F

- Controls the interface with a 12-bit or 16-bit A/D converter for gyroscopic sensors.
- Applies parallel conversion to the serial data input from the A/D converter and stores the converted data in registers.
- Connectable device (switchable by setting the mode select register)
- MB88101A (FUJITSU), ADCS7476(TI), AD7476 (Analog Devices), or MAX1162 (Maxim)
- Realtime data or 100-ms data (data added every 10 ms ten times) can be selected. 

#### Speed-pulse I/F

- 1 channel
- 16-bit speed pulse counter
- Internal noise filter (low-pass filter) removes chattering and bounce from speed pulses.
- The pulse width to be filtered out by the noise filter is selectable by the user. Passing all pulses without filtering can also be selected.
- Two types of pulse count register are provided: one for indicating the count in realtime and another for latching the count every 100 ms.
- Five types of pulse width measurement registers are provided: two for the elapsed time, one for the pulse width, and two for the observation period.
- Counts are incremented in synchronization with the rising edge of the speed pulse. 

#### TS Interface (TSIF)

- 2 channels
- Serial data input
- Support for TS data transfer by DMA auto request
- Acquisition of TS packets
  - Filters 67 kinds of PIDs (Packet ID) in total (The PID values of PAT and CAT packets are fixed. For PCR, video, and audio packets, the PID values are predefined.)

#### IR Receiver (IR)

- Auto detection of a leader
- Built-in circuit for synchronization protection (noise malfunction prevention) during the detection of a leader
- Up to 64 bits of a code are receivable per frame.
- Capable of receiving a maximum of two frames of code

### Special Edition Modules

#### SIM Card Module (SIM)

- 1 channel
- ISO/IEC7816-3 (Identification Card) supported
- Asynchronous half-duplex character transmission protocol
- Data length of 8 bits
- Bitrate can be selected: the clock generated by the internal baud rate generator.
- Generates and checks a parity bit
- 4 causes of interrupts 

#### FM Multiplex Demodulator

Functions for receiving FM multiplex broadcasting data in the DARC format

Clock: 8.192 MHz ± 200 ppm (VICS)

Features: 
-  Automatic error correction achieved by using on-chip frame memory
-  Continuous reading of one-frame data possible during 360 ms after the second horizontal correction interrupt
-  On-chip descrambler for VICS
-  Two reception channels (main and sub channels) provided
-  Fully automated simultaneous reception of two channels.
  - Two packets of data receivable at a time
  - During simultaneous data reception, control other than tuner selection (notification by an interrupt) and reading of received data (interrupt) is unnecessary.
- Number of synchronization protection steps settable
- On-chip error correction circuit
- On-chip CRC check circuit for hierarchies 4 and 2
- Compatible with the international standard frame formats A, B, and C
- Reception interrupt function after the first horizontal error correction according to the specified SI (service identification)
- Timing interrupts that occur in synchronization with frames: two channels

#### CRC

The cyclic redundancy check (CRC) block is a function block to generate and check CRC
codes of input data via the APB bus. The 32-bit Ethernet polynomial (CRC-32-IEEE 802.3) is
used as the generator polynomial. The CRC codes based on this generator polynomial are
stored in the register. 

#### Failure Self-Detection Output (RFSO)

This module distinguishes between permanent failure and temporary failure detected by
CPU or other important circuit module; moreover, detects time-out error in the process of
detecting failures. The failure of CPU or other important circuit will be detected by other
circuit (Runtime Test). Also, this module will be used for managing FTTI of this LSI.

### Others 

#### Boundary Scan

Boundary scan based on IEEE 1149.1 via JTAG interface is supported. Note that some module pins are not available on this boundary scan.


