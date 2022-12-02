
IMAGE_INSTALL_append = " \
	dosfstools \
	tslib nfs-utils e2fsprogs e2fsprogs-mke2fs e2fsprogs-resize2fs udev curl bc usbutils wget \
	mmc-utils squashfs-tools iputils sqlite3 libevent \
	devmem2 i2c-tools libgpiod sysbench \
	\
	libdrm-tests libdrm-kms libdrm \
	libpng libjpeg-turbo pv fbida yavta \
	\
	mpg123 libexif giflib \
"

IMAGE_INSTALL_append = " \
	glmark2 i2c-tools spitools ethtool usbutils pciutils can-utils mtd-utils \
	bayer2raw \
	mkfs-helper \
	vspfilter-init \
	flash-writer \
	omxh264-decode-test \
	vsp2driver-tp vsp-tests fdp1test kms-tests \
	ffmpeg \
	openpano \
"

IMAGE_INSTALL_append = " \
	${@oe.utils.conditional("CHROMIUM", "1", "chromium-ozone-wayland", "", d)} \
	${@oe.utils.conditional("DEMO_VIDEOS", "1", "demo-videos", "", d)} \
	drm2png \
	packagegroup-qt5-examples \
"

IMAGE_INSTALL_append = " \
	${@bb.utils.contains('DISTRO_FEATURES', 'surroundview', 'sv', '', d)} \
	${@bb.utils.contains('DISTRO_FEATURES', 'surroundview', 'imr-sv', '', d)} \ 
	${@bb.utils.contains('DISTRO_FEATURES', 'surroundview', 'utest-cam-imr-drm', '', d)} \ 
	${@bb.utils.contains('DISTRO_FEATURES', 'vivid', 'samsung-utils', '', d)} \
"

IMAGE_INSTALL_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'xbmc', 'kodi-addon-pvr-hts', '', d)} "
#IMAGE_INSTALL_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'xbmc', 'kodi-addon-inputstream-adaptive', '', d)} "
#IMAGE_INSTALL_append = " ${@bb.utils.contains('DISTRO_FEATURES', 'xbmc', 'kodi-addon-peripheral-joystick', '', d)} "

# Kernel modules of CMEMDRV
# IMAGE_INSTALL_remove = " kernel-module-cmemdrv"
# IMAGE_INSTALL_remove = " kernel-module-cmemdrv-dev"
