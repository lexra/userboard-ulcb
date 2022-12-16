LIC_FILES_CHKSUM = "file://${WORKDIR}/git/COPYING;md5=8f84529d052eac7c6b5769406536110f"
SRCREV = "ba0b331823dcf1661089344d26e7d61c1699f475"
LICENSE = "COPYING"
SRC_URI = "git://github.com/CogentEmbedded/imr-sv-utest.git;branch=master;protocol=https"

PACKAGE_ARCH = "${MACHINE_ARCH}"

DEPENDS = " \
	vspmif-user-module \
	libmediactl-v4l2 \
	gstreamer1.0 \
	mmngr-user-module \
	mmngrbuf-user-module \
	virtual/libgles2 \
	libpcap \
	glib-2.0 \
	opencv \
	libegl \
	glm \
	zlib \
	cairo \
	nlopt \
	libeigen \
	pango \
	libspnav \
	imr-sv-resources \
"

S = "${WORKDIR}/git"
inherit cmake

TARGET_CC_ARCH += "${LDFLAGS}"

EXTRA_OECMAKE = " -DCMAKE_FIND_ROOT_PATH=${STAGING_DIR_TARGET} -DIMR_TARGET_PLATFORM=GEN3 -DCMAKE_SKIP_RPATH=TRUE -DGSTREAMER_INCLUDE_DIRS=${STAGING_INCDIR}/gstreamer-1.0"

do_install_class-target () {
	${HOST_PREFIX}strip ${S}/bin/imr-wl
	${HOST_PREFIX}strip ${S}/prebuilt/GEN3/gen
	install -d ${D}/home/root/imr-sv
	install -m 755 ${S}/bin/imr-wl ${D}/home/root/imr-sv
	install -m 755 ${S}/prebuilt/GEN3/gen ${D}/home/root/imr-sv
}

FILES_${PN} += " \
        /home/root/imr-sv \
"

INSANE_SKIP_${PN}_append = " already-stripped "
INSANE_SKIP_${PN}-dev_append = " already-stripped "

