#!/bin/busybox sh

# Install BusyBox symbolic links
/bin/busybox --install

# Mount /proc, /sys, /dev, /run
mount -t proc proc /proc
mount -t sysfs sys /sys
mount -t devtmpfs devtmpfs /dev
mount -t tmpfs -o mode=0755,nosuid,nodev tmpfs /run

# Wait (up to 5s) for the SD card to appear
TIMER=0
while [ "$TIMER" -lt 50 -a ! -e /dev/mmcblk0p1 ]; do
	TIMER=$((TIMER+1))
	sleep .1
done
if [ ! -e /dev/mmcblk0p1 ]
then
	echo "SD card not available!"
	exec sh
fi

# Mount the first SD card partition
mount -t vfat -o ro /dev/mmcblk0p1 /boot

# Look for a new root filesystem
if [ ! -e /boot/rootfs.squashfs ]
then
	echo "No root filesystem available!"
	exec sh
fi

# Mount the new root filesystem
mkdir -p /run/squashfs
mount -t squashfs -o ro /boot/rootfs.squashfs /run/squashfs
mkdir -p /run/overlay
mkdir -p /run/work
mount -t overlay -o rw,lowerdir=/run/squashfs,upperdir=/run/overlay,workdir=/run/work overlay /rootfs

# Look for /sbin/init
if [ ! -e /rootfs/sbin/init ]
then
	echo "/sbin/init not found!"
	exec sh
fi

# Transfer /boot, /dev, /run to the new root filesystem
mkdir -p /rootfs/boot
mount --move /boot /rootfs/boot
mount --move /dev /rootfs/dev
mount --move /run /rootfs/run

# Unmount /proc, /sys
umount /proc
umount /sys

# Switch to the new root filesystem
exec switch_root /rootfs /sbin/init
