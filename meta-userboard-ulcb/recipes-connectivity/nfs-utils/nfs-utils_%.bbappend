#EXTRA_OECONF += " --enable-mount --disable-nfsv4 "
EXTRA_OECONF += " --enable-mount "

do_install_append_class-target () {
	echo '#/                     *(rw,sync,no_root_squash,no_subtree_check)' > ${D}${sysconfdir}/exports
}
