#!/bin/busybox sh

# Install BusyBox symbolic links
/bin/busybox --install

# Mount /proc, /sys, /dev
mount -t proc proc /proc
mount -t sysfs sys /sys
mount -t devtmpfs devtmpfs /dev

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
mount -t vfat -o rw /dev/mmcblk0p1 /boot

# Look for a new root filesystem
if [ ! -e /boot/rootfs.ext2 ]
then
	echo "No root filesystem available!"
	exec sh
fi

# Mount the new root filesystem
mount -t ext2 -o rw /boot/rootfs.ext2 /rootfs

# Look for /sbin/init
if [ ! -e /rootfs/sbin/init ]
then
	echo "/sbin/init not found!"
	exec sh
fi

# Transfer /boot, /dev to the new root filesystem
mkdir -p /rootfs/boot
mount --move /boot /rootfs/boot
mount --move /dev /rootfs/dev

# Unmount /proc, /sys
umount /proc
umount /sys

# Switch to the new root filesystem
exec switch_root /rootfs /sbin/init
