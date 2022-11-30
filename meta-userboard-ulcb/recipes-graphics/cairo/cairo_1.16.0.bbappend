PACKAGECONFIG = " \
	${@bb.utils.filter('DISTRO_FEATURES', 'directfb', d)} \
	${@bb.utils.contains('DISTRO_FEATURES', 'x11', 'x11 xcb', '', d)} \
	${@bb.utils.contains('DISTRO_FEATURES', 'x11 opengl', 'opengl', '', d)} \
	trace \
	${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'egl', '', d)} \
	${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'glesv2', '', d)} \
"

PACKAGECONFIG[glesv2] = "--enable-glesv2,--disable-glesv2"
