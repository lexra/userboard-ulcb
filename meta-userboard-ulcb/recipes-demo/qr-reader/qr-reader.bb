FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

LICENSE = "GPLv3"
LIC_FILES_CHKSUM = "file://${WORKDIR}/QR-reader-master/LICENSE;md5=1ebbd3e34237af26da5dc08a4e440464"
SRC_URI = "https://github.com/lue7/QR-reader/archive/refs/heads/master.zip"
SRC_URI[md5sum] = "a26622c1daeafb1e606c4dd57e02eb3e"
SRC_URI[sha256sum] = "9db224511b9bdd122c1da4df6eaad8f26f912bcb262dfc57add086c7939703d4"

SRC_URI_append = " \
	file://QRcapture.patch \
"

DEPENDS = " \
	opencv \
	zbar \
"

inherit autotools pkgconfig

PACKAGES = "${PN}-dbg ${PN}"
INSANE_SKIP_${PN} = "ldflags"

S = "${WORKDIR}/QR-reader-master"

do_compile () {
	sed 's/^CXX = g++/#CXX = g++/' -i ${S}/Makefile
	sed 's/^LIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lzbar -pthread/LIBS = -lopencv_video -lopencv_highgui -lopencv_videoio -lopencv_xobjdetect -lopencv_imgcodecs -lopencv_objdetect -lopencv_imgproc -lopencv_core -lzbar -lpthread/' -i ${S}/Makefile
	sed 's|^INCLUDES = -Isrc$|INCLUDES = -Isrc -I../recipe-sysroot/usr/include/opencv4|' -i ${S}/Makefile
	oe_runmake -C ${S}
}

do_install_class-target () {
	install -d ${D}/usr/bin
	install -m 755 ${WORKDIR}/QR-reader-master/build/bin/QRapp ${D}/usr/bin
}

do_configure[noexec] = "1"
