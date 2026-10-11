// SPDX-License-Identifier: BSD-3-Clause

#include <boot.h>
#include <debug.h>
#include <libfdt.h>
#include <malloc.h>
#include <string.h>

#include "device.h"

/*
 * The Amazon bootloader passes IDME (serial number, MAC addresses, board ID...)
 * as /idme in the device tree. Keep a copy and pass it on to downstream kernels.
 */

static void *idme_dtb;

static int copy_node(const void *src, int src_node, void *dst, int dst_node)
{
	const char *name;
	const void *val;
	int prop, child, node, len, ret;

	fdt_for_each_property_offset(prop, src, src_node) {
		val = fdt_getprop_by_offset(src, prop, &name, &len);
		if (!val)
			return len;

		ret = fdt_setprop(dst, dst_node, name, val, len);
		if (ret)
			return ret;
	}

	fdt_for_each_subnode(child, src, src_node) {
		node = fdt_add_subnode(dst, dst_node, fdt_get_name(src, child, NULL));
		if (node < 0)
			return node;

		ret = copy_node(src, child, dst, node);
		if (ret)
			return ret;
	}

	return 0;
}

static int amazon_idme(const void *dtb, int node)
{
	int idme = fdt_path_offset(dtb, "/idme");

	if (idme < 0) {
		dprintf(CRITICAL, "No IDME from previous bootloader: %d\n", idme);
		return idme;
	}

	idme_dtb = memalign(8, fdt_totalsize(dtb));
	if (!idme_dtb)
		return -1;

	memcpy(idme_dtb, dtb, fdt_totalsize(dtb));
	return 0;
}
LK2ND_DEVICE_INIT("amazon,idme", amazon_idme);

static int amazon_idme_update_dt(void *dtb, const char *cmdline,
				 enum boot_type boot_type)
{
	int idme, node, ret;

	if (!idme_dtb || !(boot_type & BOOT_DOWNSTREAM))
		return 0;

	idme = fdt_path_offset(idme_dtb, "/idme");
	node = fdt_add_subnode(dtb, 0, "idme");
	if (node < 0) {
		dprintf(CRITICAL, "Failed to add /idme node: %d\n", node);
		return 0;
	}

	ret = copy_node(idme_dtb, idme, dtb, node);
	if (ret)
		dprintf(CRITICAL, "Failed to copy IDME: %d\n", ret);

	return 0;
}
DEV_TREE_UPDATE(amazon_idme_update_dt);
