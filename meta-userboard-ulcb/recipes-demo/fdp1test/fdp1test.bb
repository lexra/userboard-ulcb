LIC_FILES_CHKSUM = "file://${WORKDIR}/git/COPYING;md5=d32239bcb673463ab874e80d47fae504"
SRCREV = "c8a88b99bac4954d665223650facab9f0b19cf39"
LICENSE = "GPLv3"

SRC_URI = " \
	git://github.com/kbingham/fdp1test.git;branch=master;protocol=https \
"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit autotools pkgconfig

S = "${WORKDIR}/git"

PARALLEL_MAKE = "-j 1"

do_install_class-target () {
        cd ${WORKDIR}/build
        install -d ${D}
        make DESTDIR=${D} install
	cd -
}

FILES_${PN} += " \
        /usr/bin \
"

INSANE_SKIP_${PN} += " ldflags dev-elf dev-deps "
INSANE_SKIP_${PN}-dev += " ldflags dev-elf dev-deps "
