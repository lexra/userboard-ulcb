BRANCH = "v5.10.41/rcar-5.1.4.1"
SRCREV = "b07e803814538740492426121617386cbb3e2a48"

SRC_URI_append = " \
	${@bb.utils.contains('DISTRO_FEATURES', 'color-managerment', 'file://0001-drm-rcar-du-Add-DU-CMM-support.patch', '', d)} \
"
