EXTRA_OECONF += " \
	--enable-mount \
	--disable-nfsv4 \
"

do_install_append () {
	echo '/                     *(rw,sync,no_root_squash,no_subtree_check)' > ${D}${sysconfdir}/exports
}
