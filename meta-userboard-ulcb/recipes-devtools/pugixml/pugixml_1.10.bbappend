
EXTRA_OECMAKE = " \
	-DBUILD_SHARED_LIBS=ON \
	-DCMAKE_BUILD_TYPE=Release \
"

EXTRA_OECMAKE += " -DCMAKE_VERBOSE_MAKEFILE=TRUE "

FILES_${PN}-dev  = " \
	${includedir} \
	${libdir}/libpugixml.so \
	${libdir}/pkgconfig \
	${libdir}/cmake/pugixml \
"

BBCLASSEXTEND += "native nativesdk"
