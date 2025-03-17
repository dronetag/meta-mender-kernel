################################################################################
# the kernel recipe bundles and signs the kernel/initramfs. it does this
# in do_deploy:append. Because of this, the expected files need to be copied
# here w/ ROOTFS_POSTPROCESS_COMMAND.
#
# Refer to recipes-kernel/linux/mender-kernel-linux-yocto.inc
################################################################################
DEPENDS += "rsync-native"

do_mender_kernel_deploy_to_image_rootfs() {
  local src_dir="${DEPLOY_DIR_IMAGE}/${MENDER/KERNEL_KERN_BUILD_STAGING_DIR}"
  local dst_dir="${IMAGE_ROOTFS}/${MENDER_KERN_PART_MOUNT_LOCATION}"
  rm      -f "$dst_dir/"* || :
  install -d "$dst_dir/"

  rsync -avqI "$src_dir" "$dst_dir" --exclude "$(basename ${MENDER_BOOT_PART_MOUNT_LOCATION})"
}
ROOTFS_POSTPROCESS_COMMAND:append  = " do_mender_kernel_deploy_to_image_rootfs;"


# Dronetag addition
# Make sure all IMAGE_BOOT_FILES end up in rootfs/boot because it does not
# seem to be the case; why is that? Nobody knows...
do_mender_kernel_ensure_boot_in_rootfs() {
    DEST="${IMAGE_ROOTFS}/boot"

    # Put in variable to avoid expansion and ';' being parsed by shell.
    image_boot_files="${IMAGE_BOOT_FILES}"
    for entry in $image_boot_files; do
        if echo "$entry" | grep -q ";"; then
            dest="$(echo "$entry" | sed -e 's/[^;]*;//')"
            entry="$(echo "$entry" | sed -e 's/;.*//')"
        else
            dest=""
        fi
        if echo "$dest" | grep -q '/$'; then
            dest_is_dir=1
            mkdir -p "$DEST/$dest"
        else
            dest_is_dir=0
            mkdir -p "$(dirname "$DEST/$dest")"
        fi

        # Use extra for loop so we can check conflict for each file.
        for file in ${DEPLOY_DIR_IMAGE}/$entry; do
            if [ $dest_is_dir -eq 1 ]; then
                destfile="$DEST/$dest$(basename $file)"
            else
                destfile="$DEST/$dest"
            fi
            if [ -e  $destfile/$file ]; then
                bbwarn "Exists ${destfile}/${file}"
                continue
            fi
            cp -L --preserve=ownership "$file" "$destfile"
        done
    done
}

# Dronetag addition
# Make sure that there is no kernel image in rootfs/boot because there are only
# files going to boot partition (with u-boot). Dunno who is placing it there
do_mender_kernel_unlink_kernel_from_boot_in_rootfs() {
    basedir="${IMAGE_ROOTFS}/boot"
    kernel_file="${basedir}/${KERNEL_IMAGETYPE}"

    if [ -L $kernel_file ]; then
        rm "${basedir}/$(readlink ${kernel_file})"
    fi
    rm $kernel_file
}

ROOTFS_POSTPROCESS_COMMAND:append = " do_mender_kernel_ensure_boot_in_rootfs;"
ROOTFS_POSTPROCESS_COMMAND:append = " do_mender_kernel_unlink_kernel_from_boot_in_rootfs;"
