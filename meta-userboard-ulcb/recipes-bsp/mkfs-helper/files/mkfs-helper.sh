#!/bin/sh -e

MACHINE=m3ulcb
MMCBLK=/dev/mmcblk0

[ `mount | grep ' on / type nfs' | wc -l` -eq 0 ] && exit 0
[ ! -f /boot/core-image-weston-${MACHINE}.tar.gz ] && exit 0

umount ${MMCBLK}p1 || true
umount ${MMCBLK}p2 || true
umount ${MMCBLK}p3 || true
umount ${MMCBLK}p4 || true
umount ${MMCBLK}p5 || true
umount ${MMCBLK}p6 || true
umount ${MMCBLK}p7 || true
umount ${MMCBLK}p8 || true
sed -e 's/\s*\([\+0-9a-zA-Z]*\).*/\1/' << EOF | fdisk ${MMCBLK}
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
 c
 t
 2
 83
 w
EOF
sync
echo yes | mkfs.vfat -n BOOT ${MMCBLK}p1
echo yes | mkfs.ext4 -E lazy_itable_init=1,lazy_journal_init=1 ${MMCBLK}p2 -L rootfs -U 614e0000-0000-4b53-8000-1d28000054a9 -jDv

mount ${MMCBLK}p2 /mnt
rm -rfv /mnt/*
tar zxvf /boot/core-image-weston-${MACHINE}.tar.gz -C /mnt
tar zxvf /boot/modules-${MACHINE}.tgz -C /mnt

umount /mnt
mount ${MMCBLK}p1 /mnt
rm -rfv /mnt/*
/bin/cp -fv /boot/Image /mnt
/bin/cp -fv /boot/*.dtb /mnt
umount /mnt

reboot
