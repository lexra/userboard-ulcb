## Flash writer helper overview

<img src="https://github.com/lexra/userboard-rcar/blob/master/assets/flash_writer_helper.png" />

As the picture displayed above, the `flash writer helper` can help quickly program the `BOOT-PARAMS`, `BL2`, `CERT`, `BL31`, and `U-BOOT` for the `Starter Kit` . 

### Build the project

Please run the `./build.sh` in the working folder. 
```
./build.sh
```

After the `./build.sh` script had been successfully completed, it will prompt for usages as follows:  

```
...
>> all succeeded

>> FOR MMC BOOT
 => setenv bootmmc 'setenv bootargs rw rootwait earlycon root=/dev/mmcblk0p2; fatload mmc 1:1 0x48080000 Image; fatload mmc 1:1 0x48000000 r8a7796-m3ulcb-tp.dtb; booti 0x48080000 - 0x48000000'
 => run bootmmc

>> FOR SD BOOT
 => setenv bootsd 'setenv bootargs rw rootwait earlycon root=/dev/mmcblk1p2; fatload mmc 0:1 0x48080000 Image; fatload mmc 0:1 0x48000000 r8a7796-m3ulcb-tp.dtb; booti 0x48080000 - 0x48000000'
 => run bootsd

>> FOR USB BOOT
 => setenv bootusb 'setenv bootargs rw rootwait earlycon root=/dev/sda2; usb reset; fatload usb 0:1 0x48080000 Image; fatload usb 0:1 0x48000000 r8a7796-m3ulcb-tp.dtb; booti 0x48080000 - 0x48000000'
 => setenv bootusb 'setenv bootargs rw rootwait earlycon root=/dev/sda2; usb reset; fatload usb 0:1 0x48080000 Image; fatload usb 0:1 0x48000000 r8a7796-vcp2106a-avm.dtb; booti 0x48080000 - 0x48000000'
 => run bootusb

>> FOR NFS BOOT
 => setenv ethact ravb
 => setenv ethaddr 2E:09:0A:00:BE:11
 => setenv ipaddr 192.168.1.133
 => setenv serverip 192.168.1.171
 => setenv bootnfs 'tftp 0x48080000 Image; tftp 0x48000000 r8a7796-m3ulcb-tp.dtb; setenv bootargs rw rootwait earlycon root=/dev/nfs nfsroot=${serverip}:/work/userboard-rcar/rootfs,nfsvers=3 ip=${ipaddr}; booti 0x48080000 - 0x48000000'
 => run bootnfs
```

### Cable setup

<img src="https://github.com/lexra/userboard-rcar/blob/master/assets/cable-setup.png" />

| Baudrate     | 115200              |
| :----------: | :------------------ |
| Data bits    | 8-bit               |
| Parity       | None                |
| Stop bits    | 1-bit               |
| Flow control | None                |

### Related buttons and switches

<img src="https://github.com/lexra/userboard-rcar/blob/master/assets/switches.png" />

| Button | Function            |
| :----: | :------------------ |
| SW8    | Power               |
| SW7    | Reset               |
| SW9    | Reset               |

| SW1    | Hyperflash / QSPI   |
| :----: | :------------------ |
|      1 | Hyper Flash         |
|      0 | QSPI                |

| SW6-1  | SW6-2 | SW6-3 | SW6-4 | Selects boot device                              |
| :----: | :---: | :---: | :---: | :-----------------------------------------       |
|      0 |     0 |     0 |     0 | A57 boot(AArch64), from SCIF, (DDR1600)          |
|      1 |     0 |     0 |     0 | A57 boot(AArch64), from SCIF, (DDR3200)          |
|      0 |     1 |     0 |     0 | CR7 boot(AArch32), from SCIF, (DDR1600)          |
|      1 |     1 |     0 |     0 | CR7 boot(AArch32), from SCIF, (DDR3200)          |
|      0 |     0 |     1 |     0 | A57 boot(AArch64), from eMMC 50M (DDR1600)       |
|      1 |     0 |     0 |     0 | not active                                       |
|      0 |     1 |     1 |     0 | A57 boot(AArch64), from eMMC 50M (DDR2133)       |
|      1 |     1 |     1 |     0 | A57 boot(AArch64), from eMMC 50M (DDR3200)       |
|      0 |     0 |     0 |     1 | A57 boot(AArch64), from QSPI, (DDR1600)          |
|      1 |     0 |     0 |     1 | not active                                       |
|      0 |     1 |     0 |     1 | A57 boot(AArch64), from QSPI, (DDR2133)          |
|      1 |     1 |     0 |     1 | A57 boot(AArch64), from QSPI, (DDR3200)          |
|      0 |     0 |     1 |     0 | A57 boot(AArch64), from HyperFlash 80M (DDR1600) |
|      1 |     0 |     1 |     1 | not active                                 |
|      0 |     1 |     1 |     1 | A57 boot(AArch64), from HyperFlash 80M (DDR2133) |
|      1 |     1 |     1 |     1 | A57 boot(AArch64), from HyperFlash 80M (DDR3200) |

### `BOOT-PARAMS`, `BL2`, `CERT`, `BL31`, `TEE`, and `U-BOOT` download

##### 1. Change to the deploy folder . 

```bash
cd build/tmp/deploy/images/m3ulcb
```

##### 2. Switches settings to `A57 boot(AArch64), from HyperFlash 80M (DDR1600)` . 

```
SW1: ON
```
```
SW6: 0010
```

##### 3. Power on `Starter Kit` with `SW8` . 

Attention: Use the 5V-DC power adapter for `Starter Kit`'s power supply . 

##### 4. Run the Hyper Flash Writer Helper.

```bash
sudo ./hyperFlash-writer-helper /dev/ttyUSB0
```

Note that the `Hyper Flash Writer Helper` had speeded up with `SUP baudrate = 921600` . 

##### 5. Reset.

Push `SW7` or `SW9` to reset; and wait for the `flash writer helper` prompting `all succeeded` . 

##### 6. Switches settings to `A57 boot(AArch64), from HyperFlash 80M (DDR3200)` . 

```
SW6: 1111
```

### Prepare the exported rootfs

#####  1. Edit the `/etc/exports` for the exported rootfs on host-linux side (Ubuntu 18) . 
```
/work/userboard-ulcb/rootfs                   *(rw,sync,no_root_squash,no_subtree_check)
```
#####  2. Restart the `nfs-kernel-server` service. 
```
sudo /etc/init.d/nfs-kernel-server restart
```

### U-boot settings
Reset the `Starter Kit` and enter the u-boot for the following settings with debug-serial console (for example: `Tera Term`) . 
```
=> setenv ethaddr 2E:09:0A:00:BE:11
=> setenv ipaddr 192.168.1.133
=> setenv serverip 192.168.1.171
=> setenv NFSROOT ${serverip}:/work/userboard-rcar/rootfs
=> setenv bootargs rw rootwait earlycon root=/dev/mmcblk0p2
=> setenv bootcmd 'fatload mmc 1:1 0x48080000 Image; fatload mmc 1:1 0x48000000 r8a7796-m3ulcb.dtb; booti 0x48080000 - 0x48000000'
=> setenv bootnfs 'tftp 0x48080000 Image; tftp 0x48000000 r8a7796-m3ulcb.dtb; setenv bootargs rw rootwait earlycon root=/dev/nfs nfsroot=${serverip}:/work/userboard-rcar/rootfs,nfsvers=3 ip=${ipaddr}; booti 0x48080000 - 0x48000000
=> saveenv
```

### NFS boot
The `core-image` update procedure is started form a given `NFS boot` . Please enter the u-boot environment by the `debug serial console` and input `run bootnfs` to start the procedure. 

```bash
=> run bootnfs
```
```bash
...
Using ethernet@e6800000 device
TFTP from server 192.168.1.171; our IP address is 192.168.1.133
Filename 'Image'.
Load address: 0x48080000
Loading: #################################################################
         #################################################################
...
[   15.438184] [drm] Supports vblank timestamp caching Rev 2 (21.10.2013).
[   15.444881] [drm] No driver support for vblank timestamp query.
[  OK  ] Started NFS Mount Daemon.
         Starting NFS server and services...
[   15.494668] init enter
[   15.497360] found AVB device is eth0@avb_ctrl
[   15.501935] init finish
         Starting Target Communication Framework agent...
[   15.526526] QoS: install v2.06
[  OK  ] Started Permit User Sessions[   15.534558] QoS Driver is Successfully loaded
.
[   15.548416] [drm] Supports vblank timestamp caching Rev 2 (21.10.2013).
[   15.555130] [drm] No driver support for vblank timestamp query.
[  OK  ] Started rc.mm.service.
[  OK  ] Started Target Communication Framework agent.
[  OK  ] Stopped NFS server and services.
[  OK  ] Started Authorization Manager.
[  OK  ] Started Network Name Resolution.
[  OK  ] Reached target Host and Network Name Lookups.
[  OK  ] Started NFS status monitor for NFSv2/3 locking..
         Stopping NFS Mount Daemon...
         Starting uim-sysfs.service...
[  OK  ] Started Getty on tty1.
[  OK  ] Started Seri[   15.772269] kim kim: failed to get shutdown gpio
al Getty on ttyS[   15.777869] kim kim: Platform Data is missing
C0.
[  OK  ] Reached target Login Prompts.
[  OK  ] Stopped NFS Mount Daemon.
[  OK  ] Started Modem Manager.
[  OK  ] Started uim-sysfs.service.
         Starting pulseaudio.service...
[  OK  ] Started pulseaudio.service.
[  OK  ] Reached target Multi-User System.
[  OK  ] Started Weston Wayland compositor.
         Starting Update UTMP about System Runlevel Changes...
         Starting Bluetooth service...
[  OK  ] Started Update UTMP about System Runlevel Changes.
[  OK  ] Started Bluetooth service.
[  OK  ] Crea[   16.539661] Bluetooth: BNEP (Ethernet Emulation) ver 1.3
ted slice User S[   16.546078] Bluetooth: BNEP socket layer initialized
lice of root.
         Starting User Manager for UID 0...
[   16.586616] audit: type=1006 audit(1644215045.703:2): pid=3667 uid=0 old-auid=4294967295 auid=0 tty=(none) old-ses=4294967295 ses=1 res=1
         Starting Hostname Service...
[  OK  ] Started Session c1 of user root.
[  OK  ] Started User Manager for UID 0.
[  OK  ] Started Hostname Service.

Poky (Yocto Project Reference Distro) 2.4.3 m3ulcb ttySC0

m3ulcb login: [   72.238210] audit: type=1006 audit(1644215101.355:3): pid=3708 uid=0 old-auid=4294967295 auid=0 tty=(none) old-ses=4294967295 ses=2 res=1
[   72.300195] audit: type=1006 audit(1645444549.584:4): pid=3711 uid=0 old-auid=4294967295 auid=0 tty=(none) old-ses=4294967295 ses=3 res=1

Poky (Yocto Project Reference Distro) 2.4.3 m3ulcb ttySC0

m3ulcb login: root
Last login: Mon Feb  7 14:24:05 CST 2022 on tty2
[   79.271638] audit: type=1006 audit(1645444556.555:5): pid=3714 uid=0 old-auid=4294967295 auid=0 tty=(none) old-ses=4294967295 ses=4 res=1
root@m3ulcb:~# [   84.359539] audit: type=1006 audit(1645444561.643:6): pid=3749 uid=0 old-auid=4294967295 auid=0 tty=(none) old-ses=4294967295 ses=5 res=1

root@m3ulcb:~#
```
As we can read from the following output, the Linux OS of `HopeRun HiHope RZ/G2M` is booting from a given remote NFS server, and mount the root file system with that given NFS server. 

### Flash `core-image` to the `Starter Kit`'s flash memory

Please login to the `Starter Kit` with serial console, then run the `./mmc_download.sh` shell script. 
```bash
root@m3ulcb:~# ./mmc_download.sh
```

It will update the `core-image` image to the flash memory. 
