### Using dmabuf-sharing

dmabuf-sharing sources frames from a V4L2 device and pushes them to a DRM/KMS device. It shares buffers between the two devices using dma-buf to achieve zero-copy operation. In the examples below we will use the vivid driver as a frame source to avoid depending on a particular video capture hardware. The command lines assume that the vivid device is accessible through /dev/video0. If other V4L2 devices exist in the system the device node might need to be changed.

### V4L2 as an exporter

The following example captures frames in 640x480 resolution from vivid and displays them full screen on the R-Car DU. The -o parameter takes the IDs of the VGA connector and the associated CRTC. You might need to change the values depending on the target system. The modest application (part of libdrm) can be used to list available connectors and CRTCs and retrieve their IDs. 

```bash
$ dmabuf-sharing -i /dev/video0 -f YUYV -S 640,480 -s 640,480@0,0 \
          -M rcar-du -o 70:68:640x480 -F YUYV -t 640,480@0,0 \
          -b 4 -e v4l2
```

[R-Car/Tests:KMS-dmabuf](https://elinux.org/R-Car/Tests:KMS-dmabuf)
