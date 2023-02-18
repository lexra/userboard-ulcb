#!/bin/bash -e

BOARD_LIST=("h3ulcb" "m3ulcb" "m3nulcb" "ebisu" "salvator-x")
TARGET_BOARD=$1
SCRIPT_DIR=`pwd`
PROPRIETARY_DIR=`pwd`/proprietary
WORK=`pwd`/${TARGET_BOARD}

POKY_COMMIT=74b22db6879b388d700f61e08cb3f239cf940d18
META_OE_COMMIT=814eec96c2a29172da57a425a3609f8b6fcc6afe
META_RENESAS_COMMIT=db07324cef9f09cfccafb36f05b8159421057af5
META_RCAR_COMMIT=v5.9.0

META_PYTHON2_COMMIT=07dca1e54f82a06939df9b890c6d1ce1e3197f75
META_CLANG_COMMIT=e63d6f9abba5348e2183089d6ef5ea384d7ae8d8
META_BROWSER_COMMIT=dcfb4cedc238eee8ed9bd6595bdcacf91c562f67

GFX_MMP_LIB=R-Car_Gen3_Series_Evaluation_Software_Package_for_Linux-20220121.zip
GFX_MMP_DRIVER=R-Car_Gen3_Series_Evaluation_Software_Package_of_Linux_Drivers-20220121.zip
GEN3E_V590_SOFTWARE=Gen3e_v590_Software.zip

function Usage () {
	echo "Usage: $0 \${TARGET_BOARD_NAME}"
	echo "BOARD_NAME list: "
	for i in ${BOARD_LIST[@]}; do echo "  - $i"; done
	exit 0
}

# Check Param.
if ! `IFS=$'\n'; echo "${BOARD_LIST[*]}" | grep -qx "${TARGET_BOARD}"`; then
	Usage
fi
if [[ ( ! -e proprietary/${GFX_MMP_LIB} || ! -e proprietary/${GFX_MMP_DRIVER} ) && ! -e proprietary/${GEN3E_V590_SOFTWARE} ]]; then
	Usage
fi
sudo chown -R ${USER}.${USER} *.sh meta-userboard* proprietary
sudo find meta-userboard* -name "*.conf" | xargs chmod -x

sudo apt-get install -y gawk wget git-core diffstat unzip texinfo gcc-multilib \
	build-essential chrpath socat libsdl1.2-dev xterm python-crypto cpio python python3 \
	python3-pip python3-pexpect xz-utils debianutils iputils-ping libssl-dev p7zip-full libyaml-dev \
	nfs-kernel-server parted ffmpeg patchelf default-jdk iproute2 python3-serial libftdi-dev ccache tmux
echo ""

mkdir -p ${WORK}
cd ${WORK}

# Clone basic Yocto layers in parallel
(git clone git://git.yoctoproject.org/poky.git || true) &
(git clone git://git.openembedded.org/meta-openembedded.git || true) &
(git clone https://github.com/renesas-rcar/meta-renesas.git || true) &
(git clone https://github.com/CogentEmbedded/meta-rcar.git || true) &
(git clone https://github.com/meta-qt5/meta-qt5.git || true) &

# Wait for all clone operations
wait

# Switch to proper branches/commits
cd ${WORK}/poky
git checkout -b mydevelop ${POKY_COMMIT} || true
cd ${WORK}/meta-openembedded
git checkout -b mydevelop ${META_OE_COMMIT} || true
cd ${WORK}/meta-renesas
git checkout -b mydevelop ${META_RENESAS_COMMIT} || true
cd ${WORK}/meta-rcar
git checkout -b mydevelop remotes/origin/${META_RCAR_COMMIT} || true
cd ${WORK}/meta-qt5
git checkout -b mydevelop c1b0c9f546289b1592d7a895640de103723a0305 || true

# meta-python2, meta-clang, meta-browser
cd ${WORK}
git clone git://git.openembedded.org/meta-python2 || true
git -C meta-python2 checkout -b mydevelop ${META_PYTHON2_COMMIT} || true
git clone https://github.com/kraj/meta-clang || true
git -C meta-clang checkout -b mydevelop ${META_CLANG_COMMIT} || true
git clone https://github.com/OSSystems/meta-browser || true
git -C meta-browser checkout -b mydevelop ${META_BROWSER_COMMIT} || true
git clone https://github.com/lexra/meta-rcar-xbmc.git || true
git -C meta-rcar-xbmc checkout -b mydevelop master || true

git clone https://github.com/renesas-rz/meta-renesas-ai.git || true
git -C meta-renesas-ai checkout -b mydevelop 3032939d08a1df7187ab29bab621c3de8b5f3cb7 || true
git -C meta-renesas-ai checkout recipes-benchmark recipes-mathematics recipes-support recipes-core recipes-kernel
git -C meta-renesas-ai apply ../../patches/benchmark.patch ../../patches/mathematics.patch ../../patches/swapfile.patch
rm -rfv meta-renesas-ai/recipes-core/images/core-image-qt.bbappend

git clone https://github.com/renesas-rz/meta-rz-edge-ai-demo.git || true
git -C meta-rz-edge-ai-demo checkout -b mydevelop master || true

# Populate meta-renesas with proprietary software packages
cd ${WORK}
WORK_PROP_DIR=${WORK}/proprietary
mkdir -p ${WORK_PROP_DIR}
if [ "${TARGET_BOARD}" == "m3ulcb" -o "${TARGET_BOARD}" == "h3ulcb" -o "${TARGET_BOARD}" == "salvator-x" ]; then
	unzip -qo ${PROPRIETARY_DIR}/${GFX_MMP_LIB} -d ${WORK_PROP_DIR}
	unzip -qo ${PROPRIETARY_DIR}/${GFX_MMP_DRIVER} -d ${WORK_PROP_DIR}
	#[ -e ${WORK_PROP_DIR}/INFRTM8RC7795ZG300Q10JPL3E_4_2_1.zip -a -e ${WORK_PROP_DIR}/INFRTM8RC7795ZG300Q10JPL3E_4_1_1.zip ] && rm -rfv ${WORK_PROP_DIR}/INFRTM8RC7795ZG300Q10JPL3E_4_1_1.zip
	#[ -e ${WORK_PROP_DIR}/INFRTM8RC7796ZG300Q10JPL3E_4_2_1.zip -a -e ${WORK_PROP_DIR}/INFRTM8RC7796ZG300Q10JPL3E_4_1_1.zip ] && rm -rfv ${WORK_PROP_DIR}/INFRTM8RC7796ZG300Q10JPL3E_4_1_1.zip
fi
if [ "${TARGET_BOARD}" == "m3nulcb" -o "${TARGET_BOARD}" == "ebisu" ]; then
	unzip -qo ${PROPRIETARY_DIR}/${GEN3E_V590_SOFTWARE} -d ${WORK_PROP_DIR}
	mv ${WORK_PROP_DIR}/Software/* ${WORK_PROP_DIR}
	rm -rf ${WORK_PROP_DIR}/Software
fi
cd ${WORK}/meta-renesas
sh meta-rcar-gen3/docs/sample/copyscript/copy_proprietary_softwares.sh -f ${WORK_PROP_DIR}

##############################
cd ${WORK}
source poky/oe-init-build-env ${WORK}/build
cp ${WORK}/../meta-userboard-ulcb/docs/sample/conf/${TARGET_BOARD}/poky-gcc/mmp/*.conf ./conf/
cp ${WORK}/../meta-userboard-ulcb/conf/machine/${TARGET_BOARD}.conf ../meta-renesas/meta-rcar-gen3/conf/machine/
cd ${WORK}/build
cp conf/local-wayland.conf conf/local.conf
bitbake-layers show-layers
bitbake core-image-weston -v
#bitbake core-image-weston -v -c populate_sdk

##############################
cd ${SCRIPT_DIR}
ls -l --color ${WORK}/build/tmp/deploy/images/${TARGET_BOARD}/Image
ls -l --color ${WORK}/build/tmp/deploy/images/${TARGET_BOARD}/core-image-weston-${TARGET_BOARD}.tar.gz
ls -l --color ${WORK}/build/tmp/deploy/images/${TARGET_BOARD}/modules-${TARGET_BOARD}.tgz
echo ""
ls -l --color ${WORK}/build/tmp/deploy/images/${TARGET_BOARD}/*.dtb
echo ""
ls -l --color ${WORK}/build/tmp/deploy/images/${TARGET_BOARD}/*.srec
echo ""
exit 0
