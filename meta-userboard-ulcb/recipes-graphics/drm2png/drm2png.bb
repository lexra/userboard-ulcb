FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=7c0d7ef03a7eb04ce795b0f60e68e7e1"

DEPENDS += " \
	libdrm \
	libpng \
	cronie \
"

SRC_URI_append = " \
        file://Makefile \
        file://drm2png.c \
        file://fbgrab.c \
        file://i2c_read_data.c \
        file://ntpc.c \
        file://root \
	file://COPYING \
"
S = "${WORKDIR}"

FILES_${PN} += "${bindir}/drm2png ${bindir}/fbgrab ${bindir}/i2c_read_data ${bindir}/ntpc"
FILES_${PN}-dev = ""

do_compile () {
	make -C ${S}
}

do_install_class-target () {
        install -m 770 -d ${D}${localstatedir}/spool/cron
        install -m 600 ${WORKDIR}/root ${D}${localstatedir}/spool/cron
        chown -R root:crontab ${D}${localstatedir}/spool/cron

	install -d ${D}${bindir}
	install ${S}/drm2png ${D}${bindir}
	install ${S}/fbgrab ${D}${bindir}
	install ${S}/i2c_read_data ${D}${bindir}
	install ${S}/ntpc ${D}${bindir}
}

do_configure[noexec] = "1"
