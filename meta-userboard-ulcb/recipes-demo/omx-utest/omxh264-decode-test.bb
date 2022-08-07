FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
LICENSE = "CLOSED"

SRC_URI = " \
        file://omxh264-decode-test.c \
"

DEPENDS = " \
	omx-user-module \
"

PACKAGE_ARCH = "${MACHINE_ARCH}"

S="${WORKDIR}"

do_compile () {
	wget https://github.com/adafruit/pi_hello_video/raw/master/hello_video/test.h264 -O test.h264
	rm -rfv omxh264-decode-test
	${CC} omxh264-decode-test.c -o omxh264-decode-test -lomxr_mc_h264d -lomxr_core -lpthread
}

do_install () {
        install -d ${D}/usr/bin
        install -d ${D}/usr/share/tests
	install -m 755 ${WORKDIR}/omxh264-decode-test ${D}/usr/bin
	install -m 644 ${WORKDIR}/test.h264 ${D}/usr/share/tests
}

FILES_${PN} += " \
	/usr/bin/omxh264-decode-test \
	/usr/share/tests/test.h264 \
"

INSANE_SKIP_${PN}_append = " ldflags "
INSANE_SKIP_${PN}-dev_append = " ldflags "
