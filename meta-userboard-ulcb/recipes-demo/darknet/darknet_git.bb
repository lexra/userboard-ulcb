FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

LIC_FILES_CHKSUM = "file://${WORKDIR}/git/LICENSE;md5=4714f70f7f315d04508e3fd63d9b9232"
LICENSE = "COPYING"

SRC_URI = " \
	git://github.com/AlexeyAB/darknet;branch=master;protocol=https \
	file://Makefile.patch \
"
SRCREV = "0faed3e60e52f742bbef43b83f6be51dd30f373e"

DEPENDS = " \
        opencv \
"

inherit pkgconfig

S = "${WORKDIR}/git"

do_compile () {
	cd ${S}
	PKG_CONFIG_DIR=${STAGING_LIBDIR}/pkgconfig \
	CPP="${TARGET_PREFIX}g++ -march=armv8-a -mtune=cortex-a57.cortex-a53 -std=c++11 -shared --sysroot=${STAGING_DIR_TARGET}" \
	oe_runmake clean all
	cd -
}

do_install () {
        install -d ${D}/home/root/darknet
        install -d ${D}${includedir}
        install -d ${D}${libdir}
        install ${S}/darknet ${D}/home/root/darknet
        #cp -Rfv ${S}/weights ${D}/home/root/darknet
        cp -Rfv ${S}/scripts ${D}/home/root/darknet
        cp -Rfv ${S}/cfg ${D}/home/root/darknet
        cp -Rfv ${S}/data ${D}/home/root/darknet
        #cp -Rfv ${S}/python ${D}/home/root/darknet
        #cp -Rfv ${S}/src/*.cl ${D}/home/root/darknet
        install ${S}/libdarknet.so ${D}${libdir}
        install ${S}/include/darknet.h ${D}${includedir}
}

FILES_${PN} += " \
        ${includedir}/darknet.h \
        ${libdir}/libdarknet.so \
        /home/root/darknet \
"

INSANE_SKIP_${PN} = "dev-elf ldflags file-rdeps"
INSANE_SKIP_${PN}-dev = "dev-elf ldflags file-rdeps"
