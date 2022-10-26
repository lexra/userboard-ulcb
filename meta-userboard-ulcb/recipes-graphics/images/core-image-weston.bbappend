
IMAGE_INSTALL_append = " packagegroup-qt5-examples "

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
	mkfs-helper \
	vspfilter-init \
	flash-writer \
	omxh264-decode-test \
	vsp2driver-tp vsp-tests fdp1test kms-tests \
	ffmpeg \
"

IMAGE_INSTALL_append = " ${@oe.utils.conditional("CHROMIUM", "1", " \
				chromium-ozone-wayland \
				", "", d)}"
