FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append = " \
	file://nfsd.cfg \
	${@bb.utils.contains('DISTRO_FEATURES', 'vivid', 'file://vivid.cfg', '', d)} \
	file://0001-drm-rcar-du-Add-DU-CMM-support.patch \
"

PARALLEL_MAKE = "-j 8"

CFLAGS += " \
        -Wno-maybe-uninitialized \
        -Wno-implicit-fallthr \
        -Wno-implicit-function-declaration \
"

EXTRA_OEMAKE_append = " V=1"
