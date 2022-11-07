FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

LIC_FILES_CHKSUM = "file://LICENSE.md;md5=28cf448cf0b66bf220b7939834f76eaf"
LICENSE="BSD-3-Clause"
PV = "1.04+git${SRCPV}"

PACKAGE_ARCH = "${MACHINE_ARCH}"
FLASH_WRITER_URL = "git://github.com/renesas-rcar/flash_writer.git;protocol=https"
BRANCH = "rcar_gen3"

SRC_URI = "${FLASH_WRITER_URL};branch=${BRANCH}"
SRCREV = "89e18eaa543e548b4a49c7731dd998c4898eacd9"
SRC_URI[md5sum] = "e89158cba035b5ab11b04440291dadc4"
SRC_URI[sha256sum] = "3171fa74ab5a8780cd05ac18c9dae75fe769c4a74d6612bcdc45aee0e07cf0ee"

SRC_URI_append = " \
	file://m3-hyperFlash-writer-helper \
	file://m3n-hyperFlash-writer-helper \
	file://h3-hyperFlash-writer-helper \
	file://m3-eMMC-writer-helper \
	file://m3n-eMMC-writer-helper \
	file://h3-eMMC-writer-helper \
"

#DEPENDS += " libunistring"
inherit deploy
ALLOW_EMPTY_${PN} = "1"

S = "${WORKDIR}/git"

do_compile() {
	if [ "${MACHINE}" == "ebisu" ]; then
		BOARD="EBISU4D";
	elif [ "${MACHINE}" == "draak" ]; then
		BOARD="DRAAK";
	elif [ "${MACHINE}" == "salvator" ]; then
		BOARD="SALVATOR";
	elif [ "${MACHINE}" == "m3ulcb" -o "${MACHINE}" == "h3ulcb" -o "${MACHINE}" == "m3nulcb" ]; then
		BOARD="ULCB";
	fi
	cd ${S}

	git checkout makefile
	sed 's|^INCLUDE_DIR = include$|INCLUDE_DIR = include -I../recipe-sysroot/usr/include|' -i makefile
        if [ "${MACHINE}" == "ebisu" ]; then
		oe_runmake BOARD=${BOARD} USB=DISABLE AArch=64 CROSS_COMPILE=${TARGET_PREFIX}
	else
		oe_runmake BOARD=${BOARD} AArch=64 CROSS_COMPILE=${TARGET_PREFIX} clean all
	fi
}

do_install[noexec] = "1"

do_deploy() {
	install -d ${DEPLOYDIR}
	install -m 644 ${S}/AArch64_output/*.mot ${DEPLOYDIR}
	if [ "${MACHINE}" == "h3ulcb" ]; then
		cp -Rpfv ${WORKDIR}/h3-hyperFlash-writer-helper ${DEPLOYDIR}/hyperFlash-writer-helper
		cp -Rpfv ${WORKDIR}/h3-eMMC-writer-helper ${DEPLOYDIR}/eMMC-writer-helper
	fi
	if [ "${MACHINE}" == "m3ulcb" ]; then
		cp -Rpfv ${WORKDIR}/m3-hyperFlash-writer-helper ${DEPLOYDIR}/hyperFlash-writer-helper
		cp -Rpfv ${WORKDIR}/m3-eMMC-writer-helper ${DEPLOYDIR}/eMMC-writer-helper
	fi
	if [ "${MACHINE}" == "m3nulcb" ]; then
		cp -Rpfv ${WORKDIR}/m3n-hyperFlash-writer-helper ${DEPLOYDIR}/hyperFlash-writer-helper
		cp -Rpfv ${WORKDIR}/m3n-eMMC-writer-helper ${DEPLOYDIR}/eMMC-writer-helper
	fi
}
PARALLEL_MAKE = "-j 1"
addtask deploy after do_compile
