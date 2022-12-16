FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI += " \
	file://wl18xx-conf.bin \
"

do_install_append_class-target () {
	mkdir -p ${D}${nonarch_base_libdir}/firmware/ti-connectivity
	install ${WORKDIR}/wl18xx-conf.bin ${D}${nonarch_base_libdir}/firmware/ti-connectivity
}

FILES_${PN} += "${nonarch_base_libdir}/firmware/wl18xx-conf.bin"
