/*
 * file : ne_devmem_hostname_krn.c
 * desc : tiny kernel module that obtains and prints out the physical
 *        address of the "nodename" member of "struct new_utsname".
 *
 * Siro Mugabi, Copyright (c) nairobi-embedded.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/module.h>
#include <linux/utsname.h>

static int ne_nodename_init(void)
{
	struct new_utsname *uts;

	if (IS_ERR_OR_NULL((uts = utsname())))
		return -EFAULT;

	pr_info("nodename: %s, phys addr: 0x%lx\n",
		uts->nodename, __pa(uts->nodename));

	return 0;
}

static void ne_nodename_fini(void)
{
	return;
}

module_init(ne_nodename_init);
module_exit(ne_nodename_fini);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("siro mugabi, nairobi-embedded.org");
