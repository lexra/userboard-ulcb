FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

LIC_FILES_CHKSUM = "file://${WORKDIR}/git/COPYING;md5=286c11fcdfc48d46796277f98503a526"
SRCREV = "3509cf2b7757d2c25bb28ad0762770bbee3720ca"
LICENSE = "COPYING"
SRC_URI = "git://github.com/CogentEmbedded/utest-cam-imr-drm.git;branch=rcar_gen3;protocol=http"

PACKAGE_ARCH = "${MACHINE_ARCH}"

DEPENDS = " \
	libdrm \
	gstreamer1.0-plugins-base \
	glib-2.0 \
	cairo \
	pango \
	mmngr-user-module \
	mmngrbuf-user-module \
	libspnav \
	libinput \
"

S = "${WORKDIR}/git"
inherit cmake

EXTRA_OECMAKE += " -DCMAKE_SYSROOT=${WORKDIR}/recipe-sysroot"

do_configure_append() {
	cd ${S}
	git checkout utest/CMakeLists.txt
	sed 's|-Werror ||' -i utest/CMakeLists.txt
	cd -
}
do_install_class-target () {
	${HOST_PREFIX}strip ${WORKDIR}/build/utest/utest-cam-imr-drm
	install -d ${D}/usr/bin
	install -m 755 ${WORKDIR}/build/utest/utest-cam-imr-drm ${D}/usr/bin
}

ALLOW_EMPTY_${PN} = "1"

FILES_${PN} += " \
	/usr/bin/utest-cam-imr-drm \
"

INSANE_SKIP_${PN}_append = " already-stripped"
INSANE_SKIP_${PN}-dev_append = " already-stripped"
