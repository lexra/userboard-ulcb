#!/bin/bash -e

RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

BOARD_LIST=("h3ulcb" "m3ulcb" "m3nulcb" "ebisu" "salvator-x")
TARGET_BOARD=$1
SCRIPT_DIR=`pwd`
WORK=`pwd`/${TARGET_BOARD}

SDDEV=${WORK}/build/tmp/deploy/images/${TARGET_BOARD}/SDMMC.img
TOTAL=8192
PART1=2560

function print_boot_example() {
        echo ""
        echo ">> FOR SD BOOT"
	echo -e "${YELLOW} => setenv bootargs rw rootwait ipv6.disable=1 earlycon root=/dev/mmcblk1p2 ${NC}"
	if [ "${TARGET_BOARD}" == "h3ulcb" ]; then
		echo -e "${YELLOW} => setenv bootcmd 'ext4load mmc 0:1 0x48080000 Image; ext4load mmc 0:1 0x48000000 r8a779m1-ulcb-kf.dtb; booti 0x48080000 - 0x48000000' ${NC}"
	elif [ "${TARGET_BOARD}" == "m3ulcb" ]; then
		echo -e "${YELLOW} => setenv bootcmd 'ext4load mmc 0:1 0x48080000 Image; ext4load mmc 0:1 0x48000000 r8a77961-ulcb-kf.dtb; booti 0x48080000 - 0x48000000' ${NC}"
	elif [ "${TARGET_BOARD}" == "m3nulcb" ]; then
		echo -e "${YELLOW} => setenv bootcmd 'ext4load mmc 0:1 0x48080000 Image; ext4load mmc 0:1 0x48000000 r8a77965-ulcb-kf.dtb; booti 0x48080000 - 0x48000000' ${NC}"
	elif [ "${TARGET_BOARD}" == "ebisu" ]; then
		echo -e "${YELLOW} => setenv bootcmd 'ext4load mmc 0:1 0x48080000 Image; ext4load mmc 0:1 0x48000000 r8a77990-ebisu-4d.dtb; booti 0x48080000 - 0x48000000' ${NC}"
	elif [ "${TARGET_BOARD}" == "salvator-x" ]; then
		echo -e "${YELLOW} => setenv bootcmd 'ext4load mmc 0:1 0x48080000 Image; ext4load mmc 0:1 0x48000000 r8a779m1-salvator-xs.dtb; booti 0x48080000 - 0x48000000' ${NC}"
	fi
	echo -e "${YELLOW} => saveenv ${NC}"
	echo ""
}

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
if [ ! -e ${WORK}/build/tmp/deploy/images/${TARGET_BOARD}/core-image-weston-${TARGET_BOARD}.tar.gz ]; then
	Usage
fi
if [ ! -e ${WORK}/build/tmp/deploy/images/${TARGET_BOARD}/modules-${TARGET_BOARD}.tgz ]; then
	Usage
fi
if [ ! -e ${WORK}/build/tmp/deploy/images/${TARGET_BOARD}/Image ]; then
	Usage
fi

##############################
sudo umount mnt || true
sudo rm -rfv mnt && mkdir -p mnt
sudo losetup -D
PART1=$(echo "${PART1} - 1" | bc)

dd if=/dev/zero of=${SDDEV} bs=1M count=${TOTAL} status=progress
sed -e 's/\s*\([\+0-9a-zA-Z]*\).*/\1/' << EOF | sudo fdisk ${SDDEV}
 n
 p
 1

 +${PART1}M
 n
 p
 2


 t
 1
 83
 t
 2
 83
 p
 w
 q
EOF

##############################
sudo losetup -Pf ${SDDEV}

LOOP=$(losetup | grep SDMMC | awk '{print $1}')
echo y | sudo mkfs.ext4 -E lazy_itable_init=0,lazy_journal_init=1 ${LOOP}p1 -L boot -U 391bec0c-e584-4282-b0ee-6f65b00fbde6 -jDv
sudo mount ${LOOP}p1 mnt
sudo cp -Rpf ${WORK}/build/tmp/deploy/images/${TARGET_BOARD}/Image* mnt
sudo cp -Rpf ${WORK}/build/tmp/deploy/images/${TARGET_BOARD}/*.dtb mnt
sudo cp -Rpf ${WORK}/build/tmp/deploy/images/${TARGET_BOARD}/core-image-weston-${TARGET_BOARD}*.tar.gz mnt
sudo cp -Rpf ${WORK}/build/tmp/deploy/images/${TARGET_BOARD}/modules-*.tgz mnt
sudo umount mnt

echo y | sudo mkfs.ext4 -E lazy_itable_init=0,lazy_journal_init=1 ${LOOP}p2 -L root -U 614e0000-0000-4b53-8000-1d28000054a9 -jDv
sudo mount ${LOOP}p2 mnt
sudo tar zxf ${WORK}/build/tmp/deploy/images/${TARGET_BOARD}/core-image-weston-${TARGET_BOARD}.tar.gz -C mnt
sudo tar zxf ${WORK}/build/tmp/deploy/images/${TARGET_BOARD}/modules-${TARGET_BOARD}.tgz -C mnt
sudo umount mnt

sudo losetup -D

##############################
ls -l --color ${SDDEV}
print_boot_example
exit 0
