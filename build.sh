#!/bin/bash -e

BOARD_LIST=("h3ulcb" "m3ulcb")
TARGET_BOARD=$1
SCRIPT_DIR=`pwd`
PROPRIETARY_DIR=`pwd`/proprietary
WORK=`pwd`/${TARGET_BOARD}

POKY_COMMIT=74b22db6879b388d700f61e08cb3f239cf940d18
META_OE_COMMIT=814eec96c2a29172da57a425a3609f8b6fcc6afe
META_RENESAS_COMMIT=13fd24957b9acc29a235ee0c7f398fd867f38b47
META_RCAR_BRANCH=v5.9.0

GFX_MMP_LIB=R-Car_Gen3_Series_Evaluation_Software_Package_for_Linux-20220121.zip
GFX_MMP_DRIVER=R-Car_Gen3_Series_Evaluation_Software_Package_of_Linux_Drivers-20220121.zip

Usage () {
    echo "Usage: $0 \${TARGET_BOARD_NAME}"
    echo "BOARD_NAME list: "
    for i in ${BOARD_LIST[@]}; do echo "  - $i"; done
    exit
}

# Check Param.
if ! `IFS=$'\n'; echo "${BOARD_LIST[*]}" | grep -qx "${TARGET_BOARD}"`; then
    Usage
fi
sudo chown -R ${USER}.${USER} meta-userboard-ulcb
sudo find meta-userboard-ulcb -name "*.conf" | xargs chmod -x

mkdir -p ${WORK}
cd ${WORK}

# Clone basic Yocto layers in parallel
(git clone git://git.yoctoproject.org/poky || true) &
(git clone git://git.openembedded.org/meta-openembedded || true) &
(git clone https://github.com/renesas-rcar/meta-renesas || true) &
(git clone https://github.com/CogentEmbedded/meta-rcar.git || true) &

# Wait for all clone operations
wait

# Switch to proper branches/commits
cd ${WORK}/poky
git checkout -b tmp ${POKY_COMMIT} || true
cd ${WORK}/meta-openembedded
git checkout -b tmp ${META_OE_COMMIT} || true
cd ${WORK}/meta-renesas
git checkout -b tmp ${META_RENESAS_COMMIT} || true
cd $WORK/meta-rcar
git checkout -b ${META_RCAR_BRANCH} remotes/origin/${META_RCAR_BRANCH} || true

# Populate meta-renesas with proprietary software packages
WORK_PROP_DIR=${WORK}/proprietary
mkdir -p ${WORK_PROP_DIR}
unzip -qo ${PROPRIETARY_DIR}/${GFX_MMP_LIB} -d ${WORK_PROP_DIR}
unzip -qo ${PROPRIETARY_DIR}/${GFX_MMP_DRIVER} -d ${WORK_PROP_DIR}
cd ${WORK}/meta-renesas
sh meta-rcar-gen3/docs/sample/copyscript/copy_proprietary_softwares.sh -f ${WORK_PROP_DIR}

cd ${WORK}
source poky/oe-init-build-env ${WORK}/build

cp ${WORK}/../meta-userboard-ulcb/docs/sample/conf/${TARGET_BOARD}/poky-gcc/mmp/*.conf ./conf/

cd ${WORK}/build
cp conf/local-wayland.conf conf/local.conf

bitbake-layers show-layers
bitbake core-image-weston -v

cd ${SCRIPT_DIR}
ls -l --color ${TARGET_BOARD}/build/tmp/deploy/images/${TARGET_BOARD}
exit 0
