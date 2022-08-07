FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI_append = " \
	file://nfsd.cfg \
"

PARALLEL_MAKE = "-j 8"

CFLAGS += " \
        -Wno-maybe-uninitialized \
        -Wno-implicit-fallthr \
        -Wno-implicit-function-declaration \
"

EXTRA_OEMAKE_append = " V=1"
