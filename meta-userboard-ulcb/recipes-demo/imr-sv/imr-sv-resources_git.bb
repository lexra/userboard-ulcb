LIC_FILES_CHKSUM = "file://${WORKDIR}/git/COPYING;md5=e7a24134d3239ca714083eac642bf737"
SRCREV = "dba7e11088b72e6303120267d9214f2800d82e6d"
LICENSE = "COPYING"
SRC_URI = "git://github.com/CogentEmbedded/imr-sv-utest-resourses.git;branch=master;protocol=https"

PACKAGE_ARCH = "${MACHINE_ARCH}"

S = "${WORKDIR}/git"

do_install () {
	install -d ${D}/home/root/imr-sv/data
	install ${S}/*.png ${D}/home/root/imr-sv
	install ${S}/*.obj ${D}/home/root/imr-sv
	install ${S}/data/*.png ${D}/home/root/imr-sv/data
}

do_configure[noexec] = "1"
do_patch[noexec] = "1"
do_compile[noexec] = "1"

FILES_${PN} += " \
	/home/root/imr-sv \
"

INSANE_SKIP_${PN}_append = " already-stripped "
INSANE_SKIP_${PN}-dev_append = " already-stripped "

