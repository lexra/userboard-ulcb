FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

LIC_FILES_CHKSUM = "file://${WORKDIR}/git/COPYING.MIT;md5=cd840bdf883ea9ab354dcf9b4c5b24a3"
LICENSE = "MIT"

SRCREV = "182f76b8d23e0a38c2f084b478296be963713c86"
SRC_URI = " \
        git://github.com/renesas-rcar/du_cmm_tp.git;protocol=https;branch=rcar-gen3 \
"

DEPENDS += " \
	libdrm \
"

S = "${WORKDIR}/git"

do_compile () {
	oe_runmake DUCMM_DIR=. SDKTARGETSYSROOT=${STAGING_DIR_TARGET} -C ${S}
}

do_install_class-target () {
	install -d ${D}${bindir}
	install -m 755 ${S}/du_cms_tp ${D}${bindir} || true
}

FILES_${PN} += "${bindir}/du_cms_tp"

INSANE_SKIP_${PN} += " ldflags "
INSANE_SKIP_${PN}-dev += " ldflags "
