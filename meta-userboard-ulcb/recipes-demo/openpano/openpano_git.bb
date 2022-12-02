LIC_FILES_CHKSUM = "file://${WORKDIR}/git/LICENSE;md5=0fc95fd5e4da1c4b79b98e0993eac38b"
SRCREV = "8d97af7193210e478c5a28013bdf63e38ebffc7e"
LICENSE = "MIT"
SRC_URI = "git://github.com/ppwwyyxx/OpenPano.git;branch=master;protocol=https"

DEPENDS = " \
	python3 \
	libeigen \
	libjpeg-turbo \
	libpng \
"

S = "${WORKDIR}/git"
inherit cmake

EXTRA_OECMAKE = " -DCMAKE_FIND_ROOT_PATH=${STAGING_DIR_TARGET} -DEIGEN3_INCLUDE_DIRS=${STAGING_DIR_TARGET}${includedir}/eigen3 -DBUILD_SHARED_LIBS=0 -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake"

do_configure_append () {
	sed 's|-march=native||g' -i ${S}/CMakeLists.txt
}

do_install() {
	${HOST_PREFIX}strip ${WORKDIR}/build/src/image-stitching
	install -d ${D}/home/root/openpano
	install -m 755 ${WORKDIR}/build/src/image-stitching ${D}/home/root/openpano
	install -m 644 ${S}/src/config.cfg ${D}/home/root/openpano
}

FILES_${PN} += " \
	/home/root/openpano/image-stitching \
	/home/root/openpano/config.cfg \
"

INSANE_SKIP_${PN}_append = " already-stripped "
INSANE_SKIP_${PN}-dev_append = " already-stripped "

