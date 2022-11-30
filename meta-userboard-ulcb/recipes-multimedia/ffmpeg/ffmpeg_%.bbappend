
PACKAGE_ARCH = "${MACHINE_ARCH}"

DEPENDS += " \
	${@bb.utils.contains('DISTRO_FEATURES', 'h264dec_lib', 'omx-user-module', '', d)} \
	${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'libsdl2 libsdl2-image libsdl2-mixer', '', d)} \
	alsa-lib \
	openssl \
"

EXTRA_OEMAKE_append = " V=1"

EXTRA_OECONF += " \
	--enable-ffplay \
"

PREFERRED_PROVIDER_virtual/libsdl2 ?= "libsdl2"

PACKAGECONFIG[wayland] = "--enable-sdl2,--disable-sdl2,virtual/libsdl2"
PACKAGECONFIG[omx] = "--enable-omx,--disable-omx,omx-user-module"
#PACKAGECONFIG[opencv] = "--enable-libopencv,--disable-libopencv"

PACKAGECONFIG = " \
	avdevice avfilter avcodec avformat swresample swscale postproc avresample \
	alsa bzlib lzma pic theora x264 zlib pthreads shared \
	${@bb.utils.contains('DISTRO_FEATURES', 'x11', 'xv xcb', '', d)} \
	${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'sdl2', '', d)} \
	${@bb.utils.contains('DISTRO_FEATURES', 'h264dec_lib', 'omx', '', d)} \
	mp3lame \
	speex \
	openssl \
"

PACKAGECONFIG += " gpl"
PACKAGECONFIG += " x264"
