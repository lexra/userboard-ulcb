### VSP

The VSP is the successor of the VIO6 video processing engine found in the first generation R-Car SoCs. I provides per-frame video processing such as color management, up/down scaling and blending. The first version of the VSP, named VSP1, is available on the second generation R-Car SoCs. The third generation R-Car SoCs introduced an updated VSP named VSP2 with support for more video processing functions.

The VSP operates in memory-to-memory mode, with some instances supporting direct output to the Display Unit (DU). All instances have one or more input channels to read data from memory, one or more output channels to write data to memory, and a variable number of video processing blocks that can be arranged freely to create processing pipelines.

- The Read Pixel Formatter (RPF) reads images from memory and unpacks them with optional pixel encoding conversion and alpha channel support. 
- The Write Pixel Formatter (WPF) packs and writes images to memory with optional dithering, mirroring/flipping and pixel encoding conversion support. 
- The Blend ROP Unit (BRU) blends up to four (Gen2) or five (Gen3) input frames into a single output using ROP and/or alpha blending.
- The Blend ROP Sub unit (BRS) is an additional two inputs blend/ROP unit.
- The Super Resolution Unit (SRU) performs super-resolution processing with optional x2 upscaling.
- The Up/Down Scaler (UDS) scales the input frame up or down by a factor of x1/4 to x4.
- The Look Up Table (LUT) converts the input frame using a per-component look-up table.
- The Cubic Look-up Table (CLU) converts the input frame using a 2-dimensional or 3-dimensional look-up table that takes all pixel components into account simultaneously.
- The Hue Saturation value Transform (HST) converts a frame from RGB to HSV.
- The Hue Saturation value Inverse transform (HSI) converts a frame from HSV to RGB.
- The Histogram Generator One dimension (HGO) computes per-component one-dimensional histograms on a frame.
- The Histogram Generator Two dimension (HGT) computes a two-dimensional histogram on a frame.
- The Lcdc InterFace (LIF) transfers frames to the Display Unit (DU).
- The SHarPness (SHP) sharpens or unsharpens frames. 

The following table shows the blocks available in each VSP variant.


<table class="wikitable">
<caption>Renesas R-Car VSP variants
</caption>
<tbody><tr>
<th>
</th>
<th># RPF
</th>
<th># WPF
</th>
<th>Other Blocks
</th></tr>
<tr>
<th colspan="4">R-Car Gen2
</th></tr>
<tr>
<th>VSPD
</th>
<td>4
</td>
<td>1
</td>
<td>BRU, HGO, HSI, HST, LIF, LUT, UDS (x1)
</td></tr>
<tr>
<th>VSPR
</th>
<td>5
</td>
<td>4
</td>
<td>BRU, HSI, HST, SRU, UDS (x3)
</td></tr>
<tr>
<th>VSPS (H2)
</th>
<td>5
</td>
<td>4
</td>
<td>BRU, CLU, HGO, HGT, HSI, HST, LUT, SRU, UDS (x3)
</td></tr>
<tr>
<th>VSPS (M2)
</th>
<td>5
</td>
<td>4
</td>
<td>BRU, CLU, HGO, HGT, HSI, HST, LUT, SRU, UDS (x1)
</td></tr>
<tr>
<th colspan="4">R-Car Gen3
</th></tr>
<tr>
<th>VSPBC (H3, M3)
</th>
<td>5
</td>
<td>1
</td>
<td>BRU, CLU, HGO, LUT
</td></tr>
<tr>
<th>VSPBD (H3)
</th>
<td>5
</td>
<td>1
</td>
<td>BRU
</td></tr>
<tr>
<th>VSPD (H3, M3)
</th>
<td>5
</td>
<td>2
</td>
<td>BRU, LIF
</td></tr>
<tr>
<th>VSPD (V3)
</th>
<td>5
</td>
<td>1
</td>
<td>BRS, BRU, LIF
</td></tr>
<tr>
<th>VSPI (H3, M3)
</th>
<td>1
</td>
<td>1
</td>
<td>CLU, HGO, HGT, HSI, HST, LUT, SHP, SRU, UDS (x1)
</td></tr>
</tbody></table>


### Usage

Both the VSP1 and VSP2 are supported in mainline Linux by a V4L2 driver named vsp1 (the BRS and SHP blocks are currently not supported). The driver uses the Media Controller API. It creates one video device node for each input and output, and exposes one V4L2 subdev device node to userspace for each hardware processing block. Applications use the Media Controller link setup API to connect the blocks into a video pipelines, the V4L2 subdev API to configure formats on the pipeline and the V4L2 API to stream video in and out of the VSP.

The following list describes the API exposed by each supported hardware block. 

##### RPF

The RPF exposes one subdev node and one video device node to userspace. The video node is a video output device named rpf.[0-9] input implementing the V4L2 multiplane video API with V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE buffers. It reads data from memory and provides it to the RPF. The subdev is named rpf.[0-9] and has two pads, a sink pad connected to the RPF video node and a source pad connectable to any of the other non-RPF subdevs.

##### WPF

Similarly to the RPF, the WPF exposes one subdev node and one video device node to userspace. The video node is a video capture device named wpf.[0-9] implementing the V4L2 multiplane video API with V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE buffers. It stores data provided by the WPF to memory. The subdev is named wpf.[0-9] and has two pads, a sink pad connectable to any of the other non-WPF subdevs and a source pad connected to the WPF video node. 

##### HGO

The HGO exposes one subdev node and one video device node to userspace. The video node is a video capture device named hgo histo implementing the V4L2 metadata API with V4L2_BUF_TYPE_META_CAPTURE buffers. It stores histogram data computed by the HGO to memory and operates with a V4L2_BUF_TYPE_META_CAPTURE buffers queue. The subdev is named hgo and has two pads, a sink pad connectable to any of the non-WPF subdevs and a source pad connected to the HGO video node.
The HGO subdev supports the crop and compose selection rectangles on its sink pad. The crop rectangle controls the portion of the input frame that is used for histogram computation. The compose rectangle controls subsampling through skipping, with supported skipping values of no skipping, 1/2 and 1/4 skipping, independently in the horizontal and vertical directions. The compose rectangle top left corner is thus hardwired to (0,0), only the rectangle size can be modified.
The HGO subdev exposes two controls to configure the Maximum RGB Mode (Gen2 and Gen3) and the number of bins (Gen3 only). When Maximum RGB Mode is enabled the HGO computes the minimum, maximum, sums and histogram values on the maximum of the R, G and B components for each pixel. Otherwise it computes the statistics on the R, G and B components separately. Maximum RGB Mode can only be enabled when operating on RGB pixels. The number of bins can be configured to 64 or 256 on Gen3 and is fixed to 64 on Gen2. 

##### HGT

The HGT exposes one subdev node and one video device node to userspace. The video node is a video capture device named hgt histo implementing the V4L2 metadata API with V4L2_BUF_TYPE_META_CAPTURE buffers. It stores histogram data computed by the HGT to memory and operates with a V4L2_BUF_TYPE_META_CAPTURE buffers queue. The subdev is named hgt and has two pads, a sink pad connectable to any of the non-WPF subdevs and a source pad connected to the HGT video node.
The HGT subdev supports the crop and compose selection rectangles on its sink pad. The crop rectangle controls the portion of the input frame that is used for histogram computation. The compose rectangle controls subsampling through skipping, with supported skipping values of no skipping, 1/2 and 1/4 skipping, independently in the horizontal and vertical directions. The compose rectangle top left corner is thus hardwired to (0,0), only the rectangle size can be modified.
The HGT subdev exposes one control to configure the Boundary Values for Hue Area. The HGT operates on a HSV image and computes the minimum, maximum and sum of the S component and a histogram based on the H and S components of each pixel. The histogram can be weighted for different Hue areas using the Boundary Values for Hue Area control. 

#### References

[elinux.org/R-Car/Devices](https://elinux.org/R-Car/Devices) . 



