DEPENDS = "python3 python3-numpy libtool swig swig-native bzip2 zlib glib-2.0"
RDEPENDS_python-opencv = "python3-core python3-numpy"

OECMAKE_GENERATOR = "Unix Makefiles"
#PACKAGECONFIG ??= "python3 eigen jpeg png tiff v4l libv4l gstreamer opengl neon ${@bb.utils.contains( 'DISTRO_FEATURES', 'qt5', 'qt5','', d)}"
PACKAGECONFIG = "eigen jpeg png tiff v4l libv4l neon ${@bb.utils.contains( 'DISTRO_FEATURES', 'qt5', 'qt5','', d)}"
#PYTHON_DIR = "python3.8"

#m3ulcb/build/tmp/work/aarch64-poky-linux/opencv/2.4.11+gitAUTOINC+707d10f115-r0/recipe-sysroot/usr/include/c++
#export CPLUS_INCLUDE_PATH="${STAGING_DIR_TARGET}/usr/include/c++"
#export PYTHON_CSPEC="-I${STAGING_INCDIR}/${PYTHON3_DIR}"
CFLAGS += " \
	-Wno-maybe-uninitialized \
	-Wno-implicit-fallthr \
	-Wno-implicit-function-declaration \
"

PARALLEL_MAKE = "-j 1"
#	-DCMAKE_NO_SYSTEM_FROM_IMPORTED=ON 

EXTRA_OECMAKE = " \
	-DCMAKE_SKIP_RPATH=ON \
	-DBUILD_PYTHON_SUPPORT=OFF \
	-DWITH_GSTREAMER=OFF \
	-DBUILD_EXAMPLES=OFF \
"

addtask libpng_link after do_patch before do_configure

do_libpng_link () {
	cd ${STAGING_DIR_TARGET}/usr/include
	[ -e libpng16 ] && ln -sf libpng16 libpng
	[ -e libv4l1-videodev.h ] && cp -fv libv4l1-videodev.h linux/videodev.h
	cd -
}
