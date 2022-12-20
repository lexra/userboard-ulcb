#!/bin/sh -e

MACHINE=m3ulcb
EMMCBLK=/dev/mmcblk0
SDMMCBLK=/dev/mmcblk1

#[ `mount | grep 'on / ' | grep "${EMMCBLK}" | wc -l` -ne 0 ] && exit 0
#umount /boot || true
#mount ${SDMMCBLK}p1 /boot
[ ! -e /boot/core-image-weston-${MACHINE}.tar.gz ] && exit 0
[ ! -e /boot/modules-${MACHINE}.tgz ] && exit 0
[ ! -e /boot/Image ] && exit 0

umount ${EMMCBLK}p1 || true
umount ${EMMCBLK}p2 || true
umount ${EMMCBLK}p3 || true
umount ${EMMCBLK}p4 || true
umount ${EMMCBLK}p5 || true
umount ${EMMCBLK}p6 || true
umount ${EMMCBLK}p7 || true
umount ${EMMCBLK}p8 || true

wipefs -a -f ${EMMCBLK} || true

sed -e 's/\s*\([\+0-9a-zA-Z]*\).*/\1/' << EOF | fdisk ${EMMCBLK}
 d

 d

 d

 d

 d

 d

 d

 d

 n
 p
 1

 +128M
 n
 p
 2


 t
 1
 83
 t
 2
 83
 w
EOF
sync
echo yes | mkfs.ext4 -E lazy_itable_init=0,lazy_journal_init=1 ${EMMCBLK}p1 -L boot -U 971f7d9a-684d-4f90-80f0-9825f95fcb7a -jDv
echo yes | mkfs.ext4 -E lazy_itable_init=1,lazy_journal_init=1 ${EMMCBLK}p2 -L root -U 62760af2-eac6-4a7b-8b39-ce78365c7d82 -jDv

mount ${EMMCBLK}p2 /mnt
rm -rfv /mnt/*
tar zxvf /boot/core-image-weston-${MACHINE}.tar.gz -C /mnt
tar zxvf /boot/modules-${MACHINE}.tgz -C /mnt
umount /mnt

mount ${EMMCBLK}p1 /mnt
rm -rfv /mnt/*
/bin/cp -Rpfv /boot/Image* /mnt
/bin/cp -Rpfv /boot/*.dtb /mnt
umount /boot
umount /mnt

reboot
