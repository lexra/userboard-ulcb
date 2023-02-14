FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

LIC_FILES_CHKSUM = "file://${WORKDIR}/git/README;md5=26299b7c703b03364b0304d96148665f"
SRCREV = "0c12fe65434455d18b8a94382766ddc81ab81bba"
LICENSE = "COPYING"
SRC_URI = " \
	git://github.com/CogentEmbedded/utest-adas.git;branch=master;protocol=https \
	file://sview_engine_process.patch \
"

PACKAGE_ARCH = "${MACHINE_ARCH}"

DEPENDS = " \
	cairo \
	virtual/egl \
	virtual/libgles2 \
	glib-2.0 \
	gstreamer1.0 \
	libpcap \
	libpthread-stubs \
	wayland-protocols \
	libspnav \
	sv \
"

inherit cmake pkgconfig

S = "${WORKDIR}/git"
B = "${WORKDIR}/build"

#PARALLEL_MAKE = "-j 8"
#OECMAKE_GENERATOR = "Unix Makefiles"

CFLAGS += " \
	-Wno-maybe-uninitialized \
	-Wno-implicit-function-declaration \
	-Wno-unused-variable \
	-Wno-missing-field-initializers \
	-Wno-incompatible-pointer-types \
	-Wno-unused-result \
	-Wno-discarded-qualifiers \
"

EXTRA_OECMAKE += " \
	-DCMAKE_FIND_ROOT_PATH=${STAGING_DIR_TARGET} \
	-DGSTREAMER_INCLUDE_DIR=${STAGING_DIR_TARGET}${includedir}/gstreamer-1.0 \
	-DGSTREAMER_LIBRARIES=${STAGING_DIR_TARGET}${libdir} \
	-DSV_INCLUDE_DIRS=${STAGING_DIR_TARGET}${includedir} \
	-DSV_TRACE=1 \
"


do_install_class-target () {
	#install -d ${D}${bindir}
	#install ${B}/utest/sview ${D}${bindir}
	install -d ${D}/home/root/sv/bin
	install ${B}/utest/sview ${D}/home/root/sv/bin
}

FILES_${PN} += " \
	/home/root/sv/bin/sview \
"

INSANE_SKIP_${PN} += " dev-deps "
INSANE_SKIP_${PN}-dev += " dev-deps "
