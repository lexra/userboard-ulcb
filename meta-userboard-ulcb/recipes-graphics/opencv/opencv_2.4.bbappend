FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI_append = " \
	file://chamfermatching.patch \
"

DEPENDS = " \
	python3 \
	python3-numpy \
	libtool \
	swig \
	swig-native \
	bzip2 \
	zlib \
	glib-2.0 \
"

RDEPENDS_python-opencv = "python3-core python3-numpy"
OECMAKE_GENERATOR = "Unix Makefiles"

PACKAGECONFIG = "eigen jpeg png tiff v4l libv4l neon ${@bb.utils.contains( 'DISTRO_FEATURES', 'qt5', 'qt5','', d)}"
#PYTHON_DIR = "python3.8"

CFLAGS += " \
	-Wno-maybe-uninitialized \
	-Wno-implicit-fallthr \
	-Wno-implicit-function-declaration \
"

EXTRA_OECMAKE = " \
	-DCMAKE_SKIP_RPATH=ON \
	-DBUILD_PYTHON_SUPPORT=OFF \
	-DWITH_GSTREAMER=OFF \
	-DBUILD_EXAMPLES=OFF \
	-DBUILD_PERF_TESTS=OFF \
	-DWITH_OPENCL=OFF \
	-DBUILD_SHARED_LIBS=ON \
"

addtask header_fix after do_patch before do_configure

do_header_fix () {
	cd ${STAGING_DIR_TARGET}/usr/include
	[ -e libpng16 ] && ln -sf libpng16 libpng
	[ -e libv4l1-videodev.h ] && cp -fv libv4l1-videodev.h linux/videodev.h
	cd -
}

#do_configure_prepend () {
#	export CFLAGS=" --sysroot=${STAGING_DIR_TARGET}"
#	export CC="aarch64-poky-linux-gcc --sysroot=${STAGING_DIR_TARGET}"
#	export CXXFLAGS=" --sysroot=${STAGING_DIR_TARGET}"
#	export CCLD="aarch64-poky-linux-gcc --sysroot=${STAGING_DIR_TARGET}"
#	export FC="aarch64-poky-linux-gfortran --sysroot=${STAGING_DIR_TARGET}"
#	export CXX="aarch64-poky-linux-g++ --sysroot=${STAGING_DIR_TARGET}"
#	export CPP="aarch64-poky-linux-gcc -E --sysroot=${STAGING_DIR_TARGET}" 
#}

do_configure_append () {
	mkdir -p ${S}/../build
	cp -Rpfv ${STAGING_DIR_TARGET}/usr/include/c++/9.3.0/* ${S}/../build
}
