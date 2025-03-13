IMAGE_TYPEDEP:biosimg += "kernelimg"
IMAGE_TYPEDEP:gptimg  += "kernelimg"
IMAGE_TYPEDEP:sdimg   += "kernelimg"
IMAGE_TYPEDEP:uefiimg += "kernelimg"

# Remove kernel from bootfiles because we are going to place it in a separate partition
IMAGE_BOOT_FILES:remove = "${KERNEL_IMAGETYPE}"