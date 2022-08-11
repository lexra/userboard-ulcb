#FILESEXTRAPATHS_prepend := "${THISDIR}/environment-setup:"

# Add some necessary tool
TOOLCHAIN_HOST_TASK += " nativesdk-cmake"
TOOLCHAIN_HOST_TASK += " nativesdk-bison nativesdk-flex"

inherit populate_sdk populate_sdk_qt5

# Post process after installed sdk
sdk_post_process () {
    # Set up kernel for building kernel config now
    echo "configuring scripts of kernel source for building .ko file..."
    $SUDO_EXEC bash -c 'source "$0" && cd "${OECORE_TARGET_SYSROOT=}/usr/src/kernel" && make scripts' $target_sdk_dir/environment-setup-@REAL_MULTIMACH_TARGET_SYS@
}
SDK_POST_INSTALL_COMMAND_append = " ${sdk_post_process}"

# Post process after installed sdk
sdk_post_process () {
        # Set up kernel for building kernel config now
        echo "configuring scripts of kernel source for building .ko file..."
        $SUDO_EXEC bash -c 'source "$0" && cd "${OECORE_TARGET_SYSROOT=}/usr/src/kernel" && make scripts' $target_sdk_dir/environment-setup-@REAL_MULTIMACH_TARGET_SYS@
}
SDK_POST_INSTALL_COMMAND_append = " ${sdk_post_process}"

export SOURCE_DIR="${THISDIR}/environment-setup"
fakeroot append_setup () {
	install -d ${SDK_OUTPUT}/${SDKPATH}/sysroots/${SDK_SYS}/environment-setup.d/
	#cp -fv ${SOURCE_DIR}/environment-setup-append.sh ${SDK_OUTPUT}/${SDKPATH}/sysroots/${SDK_SYS}/environment-setup.d/
	script=${SDK_OUTPUT}/${SDKPATH}/sysroots/${SDK_SYS}/environment-setup.d/environment-setup-append.sh
	echo '# Environment setup, support building kernel modules with kernel src in SDK' > $script
	echo 'export KERNELSRC="$SDKTARGETSYSROOT/usr/src/kernel"' >> $script
	echo 'export KERNELDIR="$SDKTARGETSYSROOT/usr/src/kernel"' >> $script
	echo 'export HOST_EXTRACFLAGS="-I${OECORE_NATIVE_SYSROOT}/usr/include/ -L${OECORE_NATIVE_SYSROOT}/usr/lib"' >> $script
	#chmod +x $script
	chmod +x ${SDK_OUTPUT}/${SDKPATH}/sysroots/${SDK_SYS}/environment-setup.d/*.sh
}
SDK_POSTPROCESS_COMMAND_prepend = " append_setup; "

# Add below modules to support self-compile for GStreamer app
IMAGE_INSTALL_append = " \
        gstreamer1.0-plugins-base-app \
        gstreamer1.0-rtsp-server libjpeg-turbo \
"

# qt multimedia needs alsa-dev when self-compiling
IMAGE_INSTALL_append = " alsa-dev"

# weston drm-backend need xkeyboard-config
IMAGE_INSTALL_append = " xkeyboard-config"
