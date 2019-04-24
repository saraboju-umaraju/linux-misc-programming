#include <stdlib.h>
#include <stdio.h>
#include <sys/auxv.h>

struct elf_fhdr_64
{
    uint32_t magic;
    uint8_t ei_class;
    uint8_t ei_data;
    uint8_t ei_version;
    uint8_t ei_osabi;
    uint8_t ei_abiversion;
    uint8_t pad[7];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff; // program header offset
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum; // number of program headers
    uint16_t e_shentsize;
    uint16_t e_shnum;
    // ...
};

struct elf_phdr_64
{
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset; // offset in file
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;  // size in file
    // ...
};

struct elf_shdr_64
{
    uint32_t sh_name;
    uint32_t sh_type;
    uint64_t sh_flags;
    uint64_t sh_addr; // virtual addr when loaded
    uint64_t sh_offset; // offset in file
    uint64_t sh_size; // size in file
    // ...
};

int main(int argc, char **argv)
{
    unsigned long vdso_hdr = getauxval(AT_SYSINFO_EHDR);

    uint32_t elf_magic = * (uint32_t *)vdso_hdr;
    if (elf_magic == 0x464C457F) {
        printf("has elf magic, proceeding...\n");
    }
    else {
        printf("no elf magic.\n");
        exit(1);
    }

    struct elf_fhdr_64 * fhdrp = (struct elf_fhdr_64 *) vdso_hdr;

    int num_phdrs = fhdrp->e_phnum;
    uint16_t phentsize = fhdrp->e_phentsize;

    long max_offs = 0;

    for (int i = 0; i < num_phdrs; i++) {
        struct elf_phdr_64 * phdr = (struct elf_phdr_64 *)(vdso_hdr
            + fhdrp->e_phoff + i * phentsize);
        long file_offs = phdr->p_offset + phdr->p_filesz;
        if (max_offs < file_offs) max_offs = file_offs;
    }

    int num_shdrs = fhdrp->e_shnum;
    int shentsize = fhdrp->e_shentsize;

    for (int i = 0; i < num_shdrs; i++) {
        struct elf_shdr_64 * shdr = (struct elf_shdr_64 *)(vdso_hdr
            + fhdrp->e_shoff + i * shentsize);
        long file_offs = shdr->sh_offset + shdr->sh_size;
        if (max_offs < file_offs) max_offs = file_offs;
    }

    // section table:
    int section_table_max = fhdrp->e_shoff + (num_shdrs * shentsize);
    if (max_offs < section_table_max) max_offs = section_table_max;

    // phdrs table:
    int phdr_table_max = fhdrp->e_phoff + (num_phdrs * phentsize);
    if (max_offs < phdr_table_max) max_offs = section_table_max;

    FILE * outfile = fopen("test-vdso.so", "wb");
    if (outfile) {
        fwrite((void *) vdso_hdr, 1, max_offs, outfile);
        fclose(outfile);
    }

    return 0;
}
