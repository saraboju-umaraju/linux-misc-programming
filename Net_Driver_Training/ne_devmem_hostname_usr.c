/*
 * file : ne_devmem_hostname_usr.c
 * desc : a demo program that uses "/dev/mem" to "mmap(2)" system RAM
 *        and write a string to the mapped region.
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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define prfmt(fmt) "%s:%d:: " fmt, __func__, __LINE__
#define prinfo(fmt, ...) printf(prfmt(fmt), ##__VA_ARGS__)
#define prerr(fmt, ...) fprintf(stderr, prfmt(fmt), ##__VA_ARGS__)

#define UTS_LEN 64
static struct {
	unsigned long paddr;
	char nodename[UTS_LEN + 1];
} ud;				/* user data */

static const char *prog;

static void usage(void)
{
	fprintf(stderr,
		"\n" "Usage: %s -l PHYS_ADDR [-s NODENAME] [-h]\n\n", prog);
	exit(EXIT_FAILURE);
}

static void bad_option(const char *option)
{
	fprintf(stderr, "\n\tBad option \"%s\"\n", option);
	usage();
}

int main(int argc, char **argv)
{
	struct option long_option[] = {
		{"help", 0, NULL, 'h'},
		{"nodename", 1, NULL, 's'},
		{"physaddr", 1, NULL, 'l'},
		{NULL, 0, NULL, 0},
	};
	int fd;
	void *map = NULL;
	const char *nodename = "simsima";
	prog = strdup(argv[0]);
	char *vaddr, *eptr;
	off_t pa_paddr, pa_offst;
	size_t len;

	memset(&ud, 0, sizeof(ud));
	while (1) {
		int c;
		if ((c =
		     getopt_long(argc, argv, "hs:l:", long_option, NULL)) < 0)
			break;
			switch (c) {

			case 's':
				strncpy(ud.nodename, optarg, UTS_LEN);
				break;

			case 'l':
				ud.paddr = strtoul(optarg, &eptr, 0);
				if (*eptr != '\0')
					bad_option("physaddr");
				break;

			case 'h':
			default:
				usage();
				break;
			}
	}

	if (!ud.paddr)
		bad_option("null physaddr");

	if (!strlen(ud.nodename))
		strncpy(ud.nodename, nodename, UTS_LEN);

	if ((fd = open("/dev/mem", O_RDWR)) < 0) {
		prerr("%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
#define MASK (sysconf(_SC_PAGE_SIZE) - 1)
	pa_paddr = ud.paddr & ~MASK;	/* lower and nearest page aligned phys addr */
	pa_offst = ud.paddr & MASK;	/* ud.paddr - pa_paddr */
	len = pa_offst + UTS_LEN + 1;	/* minimum size of VMA */

	map = mmap(0, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, pa_paddr);
	if (map == MAP_FAILED) {
		prerr("%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* obtain pointer to "nodename" in new VMA mapping */
	vaddr = (char *)(map + pa_offst);

	prinfo("Patching paddr. 0x%lx, vaddr. 0x%lx (nodename \"%s\")\n",
	       (unsigned long)ud.paddr, (unsigned long)vaddr, vaddr);
	prinfo("with string \"%s\"\n",
	       strncpy(vaddr, ud.nodename, UTS_LEN + 1));

	munmap(map, len);
	close(fd);
	exit(EXIT_SUCCESS);
}
