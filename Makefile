#create on 2017/3/21
#by wangcheng11
#[Carbon_OS team]

SUBDIRS=kernel

define make_subdir
 @for subdir in $(SUBDIRS) ; do \
 ( cd $$subdir && make $1) \
 done;
endef

all:
	$(call make_subdir , all)

	
	
TMP_MOUNT_DIR=./out/tmp/CarbonOS/
OUT_DIR=./out/

install :
	echo '将kernel拷贝到软盘镜像..'
	@if [ -d "$(TMP_MOUNT_DIR)" ]; then umount $(TMP_MOUNT_DIR); fi;
	mkdir -p $(TMP_MOUNT_DIR)
	mkdir -p $(OUT_DIR)
	cp lucasOS.img $(OUT_DIR)/CarbonOS.img
	mount $(OUT_DIR)/CarbonOS.img $(TMP_MOUNT_DIR)
	cp config/grub.conf $(TMP_MOUNT_DIR)/boot/grub/grub.conf
	cp config/menu.lst $(TMP_MOUNT_DIR)/boot/grub/menu.lst
	cp kernel/kernel $(TMP_MOUNT_DIR)/kernel
	sleep 1
	umount $(TMP_MOUNT_DIR)
	rm -rf $(TMP_MOUNT_DIR)


clean:
	rm -rf ./out
	$(call make_subdir , clean)
