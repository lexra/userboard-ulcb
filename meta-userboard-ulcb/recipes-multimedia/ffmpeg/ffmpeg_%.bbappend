
PACKAGE_ARCH = "${MACHINE_ARCH}"

DEPENDS += " \
	kernel-module-gles \
	gles-user-module \
	omx-user-module \
"

EXTRA_OEMAKE_append = " V=1"

EXTRA_OECONF += " \
	--enable-ffplay \
"

PACKAGECONFIG[omx] = "--enable-omx,--disable-omx"

PACKAGECONFIG = " \
        avdevice avfilter avcodec avformat swresample swscale postproc avresample \
        alsa bzlib gpl lzma theora x264 zlib \
        ${@bb.utils.contains('DISTRO_FEATURES', 'x11', 'xv xcb', '', d)} \
        ${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'sdl2', '', d)} \
        omx \
        mp3lame \
        speex \
        openssl \
"

#PACKAGECONFIG += " fdk-aac "
