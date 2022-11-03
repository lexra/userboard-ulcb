SRCREV = "268df1d862bc564458aee43f8ec9e71d7ec794bc"

ATFW_OPT_r8a7796 = "LOG_LEVEL=40 RCAR_SA6_TYPE=1 LSI=M3 RCAR_DRAM_SPLIT=2 ${ATFW_OPT_LOSSY}"

o_compile() {
	oe_runmake distclean
	oe_runmake bl2 bl31 rcar_layout_tool rcar_srecord PLAT=${PLATFORM} SPD=opteed MBEDTLS_COMMON_MK=1 ${ATFW_OPT}
}
