#!/bin/sh


#qemu-system-i386 --device e1000,netdev=eth0,mac=aa:bb:cc:dd:ee:ff \
#	--netdev tap,id=eth0,script=custom-scripts/qemu-ifup \
#	--kernel output/images/bzImage \
#	--hda output/images/rootfs.ext2 \
#	--nographic \
#	--append "console=ttyS0 root=/dev/sda" 


# export LINUX_OVERRIDE_SRCDIR=./kernel/linux-4.13.9

# qemu-system-i386 --kernel output/images/bzImage --hda output/images/rootfs.ext2 --hdb sdb.bin --nographic --append "console=ttyS0 root=/dev/sda"

export LINUX_OVERRIDE_SRCDIR=./kernel/linux-4.13.9

qemu-system-i386 --kernel output/images/bzImage --hda output/images/rootfs.ext2 --nographic --append "console=ttyS0 root=/dev/sda"



# ./output/host/bin/i686-buildroot-linux-gnu-gcc -o ./output/target/bin/sched_profiler ./custom-scripts/sched_profiler/sched.c
# make