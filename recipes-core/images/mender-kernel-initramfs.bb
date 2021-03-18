SUMMARY                    = "mender-kernel example initramfs"
LICENSE                    = "MIT"

export IMAGE_BASENAME      = "mender-kernel-initramfs"
IMAGE_FEATURES             = ""
IMAGE_LINGUAS              = ""
IMAGE_FSTYPES              = "${INITRAMFS_FSTYPES}"

EXTRA_IMAGEDEPENDS         = ""
KERNELDEPMODDEPEND         = ""

INITRAMFS_MAXSIZE        ??= "256000"
IMAGE_ROOTFS_SIZE          = "8192"
IMAGE_ROOTFS_EXTRA_SPACE   = "0"

PACKAGE_INSTALL            = "                               \
                               base-passwd                   \
                               mender-kernel-init-script     \
                               ${ROOTFS_BOOTSTRAP_INSTALL}   \
                               ${VIRTUAL-RUNTIME_base-utils} \
                             "

inherit core-image
deltask do_packagedata

require ${@bb.utils.contains('DISTRO_FEATURES', 'efi-secure-boot', 'conf/include/mender-kernel-initramfs-efi-secure-boot.inc', '', d)}
