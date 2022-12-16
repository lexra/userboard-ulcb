FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
LICENSE = "CLOSED"

SRC_URI = " \
	file://kms-tests.tgz \
"

DEPENDS += " \
	kmsxx \
"

S = "${WORKDIR}/git"

PARALLEL_MAKE = "-j 1"

do_install_class-target () {
	install -d ${D}/usr/bin
	make INSTALL_DIR=${D}/usr/bin -C ${S} install
}

FILES_${PN} += " \
        /usr/bin \
"

INSANE_SKIP_${PN} += " file-rdeps "
INSANE_SKIP_${PN}-dev += " file-rdeps "
