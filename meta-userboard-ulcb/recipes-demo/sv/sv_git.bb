FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

LIC_FILES_CHKSUM = "file://${WORKDIR}/git/COPYING;md5=efdbcfc220985456e1f1964b70558591"
SRCREV = "940b33d0c28af2d4df97326453e5d570b7dac146"
LICENSE = "COPYING"
SRC_URI = "git://github.com/CogentEmbedded/sv-utest.git;branch=master;protocol=https"

PACKAGE_ARCH = "${MACHINE_ARCH}"

# patchelf --replace-needed libopencv_calib3d.so.2.4 libopencv_calib3d.so libsv.so
# ld-linux-aarch64.so.1

SRC_URI_append = " \
	file://libopencv_calib3d.so.2.4.11 \
	file://libopencv_core.so.2.4.11 \
	file://libopencv_features2d.so.2.4.11 \
	file://libopencv_highgui.so.2.4.11 \
	file://libopencv_imgproc.so.2.4.11 \
	file://libopencv_flann.so.2.4.11 \
	file://libmediactl-v4l2.so.0.0.0 \
	file://CMakeLists.patch \
"

DEPENDS = " \
	libmediactl-v4l2 \
	mmngr-user-module \
	mmngrbuf-user-module \
	gles-user-module \
	libpcap \
	glib-2.0 \
	libegl \
	glm \
	zlib \
	cairo \
	nlopt \
	libeigen \
	pango \
	libspnav \
"

DEPENDS += "gstreamer1.0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good"
DEPENDS += "gstreamer1.0-plugins-bad"
DEPENDS += "gstreamer1.0-plugins-ugly"
DEPENDS += "${@oe.utils.conditional("USE_OMX_COMMON", "1", "gstreamer1.0-omx", "", d)}"

RDEPENDS_${PN} = " \
	mmngr-user-module \
	mmngrbuf-user-module \
	gles-user-module \
	libspnav \
"

S = "${WORKDIR}/git"
inherit cmake

EXTRA_OECMAKE = " -DCMAKE_SYSROOT=${STAGING_DIR_TARGET} -DSV_TARGET_PLATFORM=GEN3 -DCMAKE_SKIP_RPATH=TRUE \
	-DGSTREAMER_INCLUDE_DIRS=${STAGING_DIR_TARGET}/usr/include/gstreamer-1.0 \
	-DCAIRO_INCLUDE_DIRS=${STAGING_DIR_TARGET}/usr/include/cairo \
"

#OECMAKE_GENERATOR="Unix Makefiles"

do_configure_prepend() {
	cp -Rpfv ${WORKDIR}/libopencv_*.so.2.4.11 ${STAGING_DIR_TARGET}/usr/lib
	cp -Rpfv ${WORKDIR}/libmediactl-v4l2.so.0.0.0 ${STAGING_DIR_TARGET}/usr/lib
	ln -sf libopencv_calib3d.so.2.4.11 ${STAGING_DIR_TARGET}/usr/lib/libopencv_calib3d.so
	ln -sf libopencv_calib3d.so.2.4.11 ${STAGING_DIR_TARGET}/usr/lib/libopencv_calib3d.so.2.4
	ln -sf libopencv_core.so.2.4.11 ${STAGING_DIR_TARGET}/usr/lib/libopencv_core.so
	ln -sf libopencv_core.so.2.4.11 ${STAGING_DIR_TARGET}/usr/lib/libopencv_core.so.2.4
	ln -sf libopencv_features2d.so.2.4.11 ${STAGING_DIR_TARGET}/usr/lib/libopencv_features2d.so
	ln -sf libopencv_features2d.so.2.4.11 ${STAGING_DIR_TARGET}/usr/lib/libopencv_features2d.so.2.4
	ln -sf libopencv_highgui.so.2.4.11 ${STAGING_DIR_TARGET}/usr/lib/libopencv_highgui.so
	ln -sf libopencv_highgui.so.2.4.11 ${STAGING_DIR_TARGET}/usr/lib/libopencv_highgui.so.2.4
	ln -sf libopencv_imgproc.so.2.4.11 ${STAGING_DIR_TARGET}/usr/lib/libopencv_imgproc.so
	ln -sf libopencv_imgproc.so.2.4.11 ${STAGING_DIR_TARGET}/usr/lib/libopencv_imgproc.so.2.4
	ln -sf libopencv_flann.so.2.4.11 ${STAGING_DIR_TARGET}/usr/lib/libopencv_flann.so
	ln -sf libopencv_flann.so.2.4.11 ${STAGING_DIR_TARGET}/usr/lib/libopencv_flann.so.2.4
}

do_install() {
	${HOST_PREFIX}strip ${S}/bin/sv-utest
	install -d ${D}/usr/lib
	install -d ${D}/usr/include
	install -d ${D}/usr/include/sv
	install ${S}/libs/gen3/libsv.so ${D}/usr/lib
	install ${WORKDIR}/recipe-sysroot/usr/lib/libopencv_calib3d.so.2.4 ${D}/usr/lib
	install ${WORKDIR}/recipe-sysroot/usr/lib/libopencv_calib3d.so.2.4.11 ${D}/usr/lib
	install ${WORKDIR}/recipe-sysroot/usr/lib/libopencv_core.so.2.4 ${D}/usr/lib
	install ${WORKDIR}/recipe-sysroot/usr/lib/libopencv_core.so.2.4.11 ${D}/usr/lib
	install ${WORKDIR}/recipe-sysroot/usr/lib/libopencv_features2d.so.2.4 ${D}/usr/lib
	install ${WORKDIR}/recipe-sysroot/usr/lib/libopencv_features2d.so.2.4.11 ${D}/usr/lib
	install ${WORKDIR}/recipe-sysroot/usr/lib/libopencv_highgui.so.2.4 ${D}/usr/lib
	install ${WORKDIR}/recipe-sysroot/usr/lib/libopencv_highgui.so.2.4.11 ${D}/usr/lib
	install ${WORKDIR}/recipe-sysroot/usr/lib/libopencv_imgproc.so.2.4 ${D}/usr/lib
	install ${WORKDIR}/recipe-sysroot/usr/lib/libopencv_imgproc.so.2.4.11 ${D}/usr/lib
	install ${WORKDIR}/recipe-sysroot/usr/lib/libopencv_flann.so.2.4 ${D}/usr/lib
	install ${WORKDIR}/recipe-sysroot/usr/lib/libopencv_flann.so.2.4.11 ${D}/usr/lib
	install -d ${D}/home/root/sv
	cp -rfv ${S}/include/sv/svlib.h ${D}/usr/include/sv
	cp -rfv ${S}/include/sv/time.h ${D}/usr/include/sv
	cp -rfv ${S}/include/sv/trace.h ${D}/usr/include/sv
	cp -rfv ${S}/include/sv/types.h ${D}/usr/include/sv
	cp -rfv ${S}/resources/* ${D}/home/root/sv
	cp -rfv ${S}/bin ${D}/home/root/sv
}

FILES_${PN} += " \
	/usr/lib/libopencv_*.so.2.4* \
	/home/root/sv \
	/usr/include/sv \
"

INSANE_SKIP_${PN}_append = " already-stripped dev-elf file-rdeps dev-deps "
INSANE_SKIP_${PN}-dev_append = " already-stripped dev-elf file-rdeps dev-deps "

