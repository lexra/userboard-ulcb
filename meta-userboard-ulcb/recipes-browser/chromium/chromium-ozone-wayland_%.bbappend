FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI_append = " \
	file://0013-Only-DRI-for-x11.patch \
"

DEPENDS_remove = " \
	${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'virtual/libgl', '', d)} \
"
