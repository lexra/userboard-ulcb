FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

LIC_FILES_CHKSUM = "file://${WORKDIR}/git/COPYING;md5=efdbcfc220985456e1f1964b70558591"
SRCREV = "940b33d0c28af2d4df97326453e5d570b7dac146"
LICENSE = "COPYING"
SRC_URI = "git://github.com/CogentEmbedded/sv-utest.git;branch=master;protocol=https"

PACKAGE_ARCH = "${MACHINE_ARCH}"

SRC_URI_append = " \
	file://libopencv_calib3d.so.2.4.11 \
	file://libopencv_core.so.2.4.11 \
	file://libopencv_features2d.so.2.4.11 \
	file://libopencv_highgui.so.2.4.11 \
	file://libopencv_imgproc.so.2.4.11 \
	file://libopencv_flann.so.2.4.11 \
	file://CMakeLists.patch \
	file://libmediactl-v4l2.so.0.0.1 \
"

DEPENDS = " \
	libmediactl-v4l2 \
	mmngr-user-module \
	mmngrbuf-user-module \
	virtual/libgles2 \
	virtual/egl \
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
OECMAKE_GENERATOR = "Unix Makefiles"

EXTRA_OECMAKE = " -DCMAKE_SYSROOT=${STAGING_DIR_TARGET} -DSV_TARGET_PLATFORM=GEN3 -DCMAKE_SKIP_RPATH=TRUE \
	-DGSTREAMER_INCLUDE_DIRS=${STAGING_DIR_TARGET}${includedir}/gstreamer-1.0 \
	-DCAIRO_INCLUDE_DIRS=${STAGING_DIR_TARGET}${includedir}/cairo \
"

do_configure_prepend() {
	cp -f ${WORKDIR}/libmediactl-v4l2.so.0.0.1 ${STAGING_DIR_TARGET}${libdir}/libmediactl-v4l2.so.0.0.0
	cp -Rpfv ${WORKDIR}/libopencv_*.so.2.4.11 ${STAGING_DIR_TARGET}${libdir}
	ln -sf libopencv_calib3d.so.2.4.11 ${STAGING_DIR_TARGET}${libdir}/libopencv_calib3d.so
	ln -sf libopencv_calib3d.so.2.4.11 ${STAGING_DIR_TARGET}${libdir}/libopencv_calib3d.so.2.4
	ln -sf libopencv_core.so.2.4.11 ${STAGING_DIR_TARGET}${libdir}/libopencv_core.so
	ln -sf libopencv_core.so.2.4.11 ${STAGING_DIR_TARGET}${libdir}/libopencv_core.so.2.4
	ln -sf libopencv_features2d.so.2.4.11 ${STAGING_DIR_TARGET}${libdir}/libopencv_features2d.so
	ln -sf libopencv_features2d.so.2.4.11 ${STAGING_DIR_TARGET}${libdir}/libopencv_features2d.so.2.4
	ln -sf libopencv_highgui.so.2.4.11 ${STAGING_DIR_TARGET}${libdir}/libopencv_highgui.so
	ln -sf libopencv_highgui.so.2.4.11 ${STAGING_DIR_TARGET}${libdir}/libopencv_highgui.so.2.4
	ln -sf libopencv_imgproc.so.2.4.11 ${STAGING_DIR_TARGET}${libdir}/libopencv_imgproc.so
	ln -sf libopencv_imgproc.so.2.4.11 ${STAGING_DIR_TARGET}${libdir}/libopencv_imgproc.so.2.4
	ln -sf libopencv_flann.so.2.4.11 ${STAGING_DIR_TARGET}${libdir}/libopencv_flann.so
	ln -sf libopencv_flann.so.2.4.11 ${STAGING_DIR_TARGET}${libdir}/libopencv_flann.so.2.4
}

do_install() {
	${HOST_PREFIX}strip ${S}/bin/sv-utest
	${HOST_PREFIX}strip ${S}/libs/gen3/libsv.so
	install -d ${D}${libdir}
	install -d ${D}${includedir}
	install -d ${D}${includedir}/sv
	install ${S}/libs/gen3/libsv.so ${D}${libdir}
	install ${STAGING_DIR_TARGET}${libdir}/libopencv_calib3d.so.2.4.11 ${D}${libdir}
	install ${STAGING_DIR_TARGET}${libdir}/libopencv_core.so.2.4.11 ${D}${libdir}
	install ${STAGING_DIR_TARGET}${libdir}/libopencv_features2d.so.2.4.11 ${D}${libdir}
	install ${STAGING_DIR_TARGET}${libdir}/libopencv_highgui.so.2.4.11 ${D}${libdir}
	install ${STAGING_DIR_TARGET}${libdir}/libopencv_imgproc.so.2.4.11 ${D}${libdir}
	install ${STAGING_DIR_TARGET}${libdir}/libopencv_flann.so.2.4.11 ${D}${libdir}
	install -d ${D}/home/root/sv/bin
	install -d ${D}/home/root/sv/calib
	install ${S}/include/sv/svlib.h ${D}${includedir}/sv
	install ${S}/include/sv/time.h ${D}${includedir}/sv
	install ${S}/include/sv/trace.h ${D}${includedir}/sv
	install ${S}/include/sv/types.h ${D}${includedir}/sv
	install ${S}/resources/calib/* ${D}/home/root/sv/calib
	install ${S}/resources/*.bmp ${D}/home/root/sv
	install ${S}/resources/*.yaml ${D}/home/root/sv
	install ${S}/resources/Car.obj ${D}/home/root/sv
	install ${S}/resources/Car.mtl ${D}/home/root/sv
	install ${S}/bin/* ${D}/home/root/sv/bin
	#install ${WORKDIR}/libmediactl-v4l2.so.0.0.1 ${D}${libdir}
	#/usr/bin/patchelf --replace-needed libmediactl-v4l2.so.0 libmediactl-v4l2.so.0.0.1 ${D}/home/root/sv/bin/sv-utest
	#/usr/bin/patchelf --replace-needed libopencv_calib3d.so.2.4 libopencv_calib3d.so.2.4.11 ${D}${libdir}/libsv.so
}

FILES_${PN} += " \
	${libdir}/lib*.so* \
	/home/root/sv \
	${includedir}/sv \
"

INSANE_SKIP_${PN}_append = " already-stripped dev-elf file-rdeps dev-deps "
INSANE_SKIP_${PN}-dev_append = " already-stripped dev-elf file-rdeps dev-deps "

