
IMAGE_INSTALL_append = " \
        tslib nfs-utils e2fsprogs e2fsprogs-resize2fs udev curl bc usbutils wget \
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
	vsp-tests fdp1test \
	ffmpeg \
"

