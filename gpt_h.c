#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define LOGICAL_BLOCK_SIZE    512

typedef struct {
    char signature[8];
    int revision;
    int header_size;
    uint32_t crc_header;
    int reserved;
    uint64_t lba_current;
    uint64_t lba_backup;
    uint64_t lba_first_usable;
    uint64_t lba_last_usable;
    uint8_t disk_guid[16];
    uint64_t lba_entry_start;
    int entry_count;
    int entry_size;
    uint32_t crc_entries;
    int reserved1[105];
}__attribute__ ((packed)) gpt_header;

void print_gpt_header(int fd) 
{
    char sz[9];

	gpt_header *gpt = malloc (sizeof (gpt_header) );

    memset(gpt, 0, sizeof(gpt_header));

	lseek(fd, 512, SEEK_SET);

    read(fd, gpt, sizeof(gpt_header));

    memcpy(sz, gpt->signature, 8);

    sz[8] = '\0';

    printf("signature %s\n", sz);

    printf("revision %d\n", gpt->revision);

    printf("header_size %d\n", gpt->header_size);

    printf("crc_header 0x%X\n", gpt->crc_header);

    printf("lba_current %lu\n", gpt->lba_current);

    printf("lba_backup %lu\n", gpt->lba_backup);

    printf("lba_first_usable %lu\n", gpt->lba_first_usable);

    printf("lba_last_usable %lu\n", gpt->lba_last_usable);

    printf("lba_entry_start %lu\n", gpt->lba_entry_start);

    printf("entry_count %d\n", gpt->entry_count);

    printf("entry_size %d\n", gpt->entry_size);

    printf("crc_entries 0x%X\n", gpt->crc_entries);

}
int main (int argc, char *argv[])
{
	if (argc < 2) {
		printf ("Throw me an Disk name boss !!\n");
		return 1;
	}

	int f = open(argv[1], O_RDONLY);

	if ( f == -1 ) {
		printf ("run with root permissions\n");
		return -1;
	}

	print_gpt_header(f);

	return 0;
}

