FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
LICENSE = "CLOSED"

SRC_URI = " \
	file://samsung-utils.tgz \
"

DEPENDS += " \
	libdrm \
	v4l-utils \
"

PARALLEL_MAKE = "-j 1"

S = "${WORKDIR}/git"

do_compile () {
	oe_runmake -C ${S}/v4l2-dmabuf-test
	oe_runmake LIBDRM_DIR=${STAGING_DIR_TARGET} -C ${S}/v4l2-drm-example
	oe_runmake -C ${S}/v4l2-fimc-m2m-example
	oe_runmake -C ${S}/v4l2-jpeg-codec-test
	oe_runmake KERNELHEADERS_PREFIX=${STAGING_KERNEL_DIR}/include -C ${S}/v4l2-mfc-encoder
	#oe_runmake KERNELHEADERS_PREFIX=${STAGING_KERNEL_DIR}/include LIBDRM_PREFIX=${STAGING_DIR_TARGET}/usr/include -C ${S}/v4l2-mfc-example
	cd ${S}/v4l2-hdmi-example
	${CC} main.c -o v4l2-hdmi -L../../recipe-sysroot/usr/lib -ldrm -lm -ldl
	cd -
}

do_install () {
	install -d ${D}/home/root/samsung-utils
	install -m 755 ${S}/v4l2-dmabuf-test/v4l2-dbuf ${D}/home/root/samsung-utils
	install -m 755 ${S}/v4l2-drm-example/dmabuf-sharing ${D}/home/root/samsung-utils
	install -m 755 ${S}/v4l2-fimc-m2m-example/fimc-m2m-f02 ${D}/home/root/samsung-utils
	install -m 755 ${S}/v4l2-fimc-m2m-example/*.sh ${D}/home/root/samsung-utils
	install ${S}/v4l2-fimc-m2m-example/*.raw ${D}/home/root/samsung-utils
	install -m 755 ${S}/v4l2-jpeg-codec-test/test-jpeg ${D}/home/root/samsung-utils
	install -m 755 ${S}/v4l2-mfc-encoder/mfc-encode ${D}/home/root/samsung-utils
	#install -m 755 ${S}/v4l2-mfc-example/v4l2-decode ${D}/home/root/samsung-utils
	install -m 755 ${S}/v4l2-hdmi-example/v4l2-hdmi ${D}/home/root/samsung-utils
}

FILES_${PN} += " \
	/home/root/samsung-utils \
"

INSANE_SKIP_${PN}_append = " file-rdeps ldflags "
INSANE_SKIP_${PN}-dev_append = " file-rdeps ldflags "
