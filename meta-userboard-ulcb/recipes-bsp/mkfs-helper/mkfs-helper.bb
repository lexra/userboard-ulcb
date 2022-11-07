FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
LICENSE = "CLOSED"

SRC_URI = " \
	file://mkfs-helper.sh \
"

PACKAGE_ARCH = "${MACHINE_ARCH}"

S = "${WORKDIR}"

do_install () {
	install -d ${D}/home/root
	install -m 755 ${WORKDIR}/mkfs-helper.sh ${D}/home/root
	sed "s/MACHINE=m3ulcb/MACHINE=${MACHINE}/" -i ${D}/home/root/mkfs-helper.sh
}

do_configure[noexec] = "1"
do_patch[noexec] = "1"
do_compile[noexec] = "1"

FILES_${PN} = " \
	/home/root/mkfs-helper.sh \
"
