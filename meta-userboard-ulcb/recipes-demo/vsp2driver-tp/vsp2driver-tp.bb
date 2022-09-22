FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
LIC_FILES_CHKSUM = "file://${WORKDIR}/git/COPYING.MIT;md5=cd840bdf883ea9ab354dcf9b4c5b24a3"
LICENSE = "MIT"
SRC_URI = " \
	git://github.com/renesas-rcar/vsp2driver_tp.git;protocol=https;branch=rcar-gen3 \
	file://media_get_entity_by_name.patch \
"
SRCREV = "9364bed9017402d32e74d61d058130ce645f75c0"

PARALLEL_MAKE = "-j 1"

PACKAGE_ARCH = "${MACHINE_ARCH}"
DEPENDS = " \
	v4l-utils \
	libmediactl-v4l2 \
	mmngr-user-module \
	mmngrbuf-user-module \
"

S = "${WORKDIR}/git"

do_compile () {
	make -C bru
	make -C clu
	make -C hgo
	make -C lut
	make -C uds
}

do_install () {
	install -d ${D}${prefix}/bin
	install -d ${D}${prefix}/share/tests
	install ${S}/bru/v4l2_bru_tp ${D}${prefix}/bin
	install ${S}/clu/v4l2_clu_tp ${D}${prefix}/bin
	install ${S}/hgo/v4l2_hgo_tp ${D}${prefix}/bin
	install ${S}/lut/v4l2_lut_tp ${D}${prefix}/bin
	install ${S}/uds/v4l2_uds_tp ${D}${prefix}/bin
	install ${S}/input_image/1280_720_ARGB32.argb ${D}${prefix}/share/tests
}

FILES_${PN} += " \
	${prefix}/bin/v4l2_*_tp \
	${prefix}/share/tests/1280_720_ARGB32.argb \
"
