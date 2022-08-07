FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
LICENSE = "CLOSED"

SRC_URI = " \
	file://vsp-tests.tgz \
"

PACKAGE_ARCH = "${MACHINE_ARCH}"

PARALLEL_MAKE = "-j 1"

S = "${WORKDIR}/git"
DEST_DIR = "/home/root/vsp-tests"

do_compile () {
	mkdir -p ${WORKDIR}/build
	make -C ${S}
	INSTALL_DIR=${WORKDIR}/build make -C ${S} install
	${HOST_PREFIX}strip ${WORKDIR}/build/gen-image
	${HOST_PREFIX}strip ${WORKDIR}/build/monotonic-ts
}

do_install () {
	install -d ${D}${DEST_DIR}/frames
	install -m 755 ${WORKDIR}/build/gen-image ${D}${DEST_DIR}
	install -m 755 ${WORKDIR}/build/monotonic-ts ${D}${DEST_DIR}
	install -m 755 ${WORKDIR}/build/*.sh ${D}${DEST_DIR}
	install ${WORKDIR}/build/frames/* ${D}${DEST_DIR}/frames
}

do_configure[noexec] = "1"

FILES_${PN} += " \
	${DEST_DIR} \
"

INSANE_SKIP_${PN}_append = " already-stripped "
INSANE_SKIP_${PN}-dev_append = " already-stripped "
