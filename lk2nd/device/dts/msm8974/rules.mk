# SPDX-License-Identifier: BSD-3-Clause
LOCAL_DIR := $(GET_LOCAL_DIR)

QCDTBS += \
	$(LOCAL_DIR)/samsung.dtb \
	$(LOCAL_DIR)/msm8974-amazon-kodiak-evt.dtb \
	$(LOCAL_DIR)/msm8974-amazon-kodiak-dvt.dtb \
	$(LOCAL_DIR)/msm8974-amazon-kodiak-evt-inx.dtb \
	$(LOCAL_DIR)/msm8974-amazon-kodiak-dvt-inx.dtb \
	$(LOCAL_DIR)/msm8974-amazon-kodiak-predvt2-inx.dtb \
	$(LOCAL_DIR)/msm8974pro-ab-pm8941-mtp.dtb \
	$(LOCAL_DIR)/msm8974pro-ac-pm8941-mtp.dtb \
	$(LOCAL_DIR)/msm8974-blackberry-wolverine.dtb \
	$(LOCAL_DIR)/msm8974-htc-m8.dtb \
	$(LOCAL_DIR)/msm8974-lge-d855.dtb \

ADTBS += \
	$(LOCAL_DIR)/msm8974-lge-hammerhead.dtb \
	$(LOCAL_DIR)/msm8974pro-ac-motorola-victara.dtb \

DTBS += \
	$(LOCAL_DIR)/msm8974pro-sony-leo.dtb \
	$(LOCAL_DIR)/msm8974pro-sony-aries.dtb \

# The Amazon bootloader adds its IDME to the DTB in place and needs free space for it
$(BUILDDIR)/$(LOCAL_DIR)/msm8974-amazon-kodiak-%.dtb: DTC_FLAGS := -p 2048
