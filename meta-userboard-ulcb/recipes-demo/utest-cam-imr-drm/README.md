## Camera demo for VIN-IMR-DRM capture pipe

Unit test application demonstrates IMR-DRM usage on systems that have no GPU. The test application name is 'utest-cam-imr-drm'. The unit test is provided in sources and built within yocto.

<p>

The unit test is located on rootfs for `core-image-minimal` build. To launch the application issue the following command with 4 cameras connected:

```bash
utest-cam-imr-drm
```

NOTE: unit test assumes use of 4 cameras at resolution 1280x1080

## CAMERAS_NUMBER

To use other then 4 amount of cameras, then modify `CAMERAS_NUMBER` definition in source code (meta-rcar/meta-rcar-gen3-adas/recipes-bsp/utest-apps/files/utest-cam-imr-drm.tar.gz)
To use other resolution use parameters 'h' and 'w':

```bash
utest-cam-imr-drm -w 1280 -h 800
```

## Camera low level tests

It is possible to run low level tests to validate LVDS cameras.
The test application name is 'capture'. The 'capture' is provided in sources and build within yocto.

<p>

The test examples are located on rootfs path:
```bash
ls -1 /usr/share/tests/
```
```bash
test_lvds_2cameras_on_display1920x1080.sh
test_lvds_4cameras_on_display1920x1080.sh
test_lvds_8cameras_on_display1920x1080.sh
test_lvds_camera_0-3.sh
test_lvds_camera_0.sh
test_lvds_camera_4-7.sh
test_lvds_camera_4.sh
```
## Features of IMR-LX4 on `R-Car M3`

```
1. Two Channels of IMR-LX4
2. Pixel rate @400MHz
3. Drawing space: 2048x2048 for input and for output
4. Picture format: YUV422
```

