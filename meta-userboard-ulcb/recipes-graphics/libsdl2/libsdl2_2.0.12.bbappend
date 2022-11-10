FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI_append = " \
	file://makefile.test \
"

PACKAGECONFIG[opengl]     = "--enable-video-opengl,--disable-video-opengl"
PREFERRED_PROVIDER_virtual/libgles2 = "gles-user-module"
PACKAGECONFIG_GL = "${@bb.utils.contains('DISTRO_FEATURES', 'opengl', 'gles2 egl', '', d)}"
EXTRA_OEMAKE_append = " V=1"

PACKAGECONFIG = " \
	${PACKAGECONFIG_GL} \
	${@bb.utils.filter('DISTRO_FEATURES', 'alsa directfb pulseaudio x11', d)} \
	${@bb.utils.contains('DISTRO_FEATURES', 'wayland', 'wayland gles2 egl', '', d)} \
	${@bb.utils.contains("TUNE_FEATURES", "neon","arm-neon","",d)} \
	alsa \
	tslib \
"

PACKAGECONFIG[gles2]      = "--enable-video-opengles,--disable-video-opengles,virtual/libgles2"
PACKAGECONFIG[wayland]    = "--enable-video-wayland,--disable-video-wayland,wayland-native wayland wayland-protocols libxkbcommon"

do_install_append () {
	install -d ${D}/home/root/sdl-tests/shapes
	install ${S}/test/shapes/* ${D}/home/root/sdl-tests/shapes
	install -d ${D}/home/root/sdl-tests/nacl
	install ${S}/test/nacl/* ${D}/home/root/sdl-tests/nacl
	install -d ${D}/home/root/sdl-tests/emscripten
	install ${S}/test/emscripten/* ${D}/home/root/sdl-tests/emscripten

	install ${S}/test/*.bmp ${D}/home/root/sdl-tests
	install ${S}/test/*.dat ${D}/home/root/sdl-tests
	install ${S}/test/*.xbm ${D}/home/root/sdl-tests
	install ${S}/test/*.markdown ${D}/home/root/sdl-tests
	install ${S}/test/*.wav ${D}/home/root/sdl-tests

	cd ${S}/test
	install checkkeys controllermap loopwave loopwavequeue testatomic testaudiocapture testaudiohotplug testaudioinfo testautomation \
		testbounds testcustomcursor testdisplayinfo testdraw2 testdrawchessboard testdropfile testerror testfile testfilesystem \
		testgamecontroller testgesture testgles2 testhaptic testhittesting testhotplug testiconv testime testintersections \
		testjoystick testkeys testloadso testlock testmessage testmultiaudio testnative testoverlay2 testplatform testqsort testrelative testrendercopyex \
		testrendertarget testresample testrumble testscale testsem testsensor testshape testsprite2 testspriteminimal teststreaming \
		testthread testtimer testver testviewport testvulkan testwm2 testyuv torturethread \
		${D}/home/root/sdl-tests
	cd -
}

do_compile_tests () {
	cp ${WORKDIR}/makefile.test ${S}/test/Makefile
	oe_runmake -C ${S}/test
}
addtask compile_tests after do_compile before do_install

FILES_${PN} += " \
        /home/root/sdl-tests \
"

TARGET_CC_ARCH += "${LDFLAGS}"
INSANE_SKIP_${PN} += "ldflags"
