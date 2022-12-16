#LIC_FILES_CHKSUM = "file://${WORKDIR}/git/LICENSE;md5=c670e18272184fc0e86e1648678b4f2a"
#SRCREV = "73d147055b51decf1771c12b00effad4f1d29ea1"
#SRC_URI = "git://github.com/tomba/kmsxx.git;branch=64bit;protocol=https"
LIC_FILES_CHKSUM = "file://${WORKDIR}/git/LICENSE;md5=815ca599c9df247a0c7f619bab123dad"
SRCREV = "524176c33ee2b79f78d454fa621e0d32e7e72488"
SRC_URI = "git://github.com/tomba/kmsxx.git;branch=kmsview;protocol=https"

LICENSE = "MPL-2.0"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit cmake pkgconfig

DEPENDS += " \
	gles-user-module libgbm \
	python-pywbem \
	python3 \
	python3-pybind11 \
	python3-py \
	libdrm \
"

S = "${WORKDIR}/git"

PARALLEL_MAKE = "-j 1"

OECMAKE_GENERATOR = "Unix Makefiles"

EXTRA_OECMAKE = " \
	-DBUILD_SHARED_LIBS=on \
	-DKMSXX_ENABLE_PYTHON=on \
	-DKMSXX_ENABLE_KMSCUBE=off \
	-DKMSXX_PYTHON_VERSION=python3 \
"

do_configure_prepend () {
	cd ${WORKDIR}/git
	git submodule update --init --recursive
	sed 's|^set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")$|#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")|' -i CMakeLists.txt
	sed 's|^set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")$|#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")|' -i CMakeLists.txt
	cd -
}

do_install_class-target () {
	cd ${WORKDIR}/build
	install -d ${D}
	make DESTDIR=${D} install
	rm -rfv ${D}/usr/bin/kmstest
	cd -
}

FILES_${PN} += " \
        /usr/bin \
        /usr/include \
        /usr/lib \
"

INSANE_SKIP_${PN} += " ldflags dev-elf dev-deps "
INSANE_SKIP_${PN}-dev += " ldflags dev-elf dev-deps "
