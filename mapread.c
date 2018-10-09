#include <stdlib.h>
#include <elf.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
unsigned long load_elf_image (unsigned long addr)
{
printf(" __UMA__ %s %s %d",__FILE__,__func__,__LINE__); 
    Elf64_Ehdr *ehdr;       /* Elf header structure pointer     */
    Elf64_Shdr *shdr;       /* Section header structure pointer */
    unsigned char *strtab = 0;  /* String table pointer             */
    unsigned char *image;       /* Binary image pointer             */
    int i;              /* Loop counter                     */

    /* -------------------------------------------------- */

    ehdr = (Elf64_Ehdr *) addr;

    /* Find the section header string table for output info */
    shdr = (Elf64_Shdr *) (addr + ehdr->e_shoff +
                   (ehdr->e_shstrndx * sizeof (Elf64_Shdr)));

    if (shdr->sh_type == SHT_STRTAB)
        strtab = (unsigned char *) (addr + shdr->sh_offset);

    /* Load each appropriate section */
    for (i = 0; i < ehdr->e_shnum; ++i) {
        shdr = (Elf64_Shdr *) (addr + ehdr->e_shoff +
                       (i * sizeof (Elf64_Shdr)));

        if (!(shdr->sh_flags & SHF_ALLOC)
           || shdr->sh_addr == 0 || shdr->sh_size == 0) {
            continue;
        }

        if (strtab) {
            printf ("%sing %s @ 0x%08lx (%ld bytes)\n",
                (shdr->sh_type == SHT_NOBITS) ?
                    "Clear" : "Load",
                &strtab[shdr->sh_name],
                (unsigned long) shdr->sh_addr,
                (long) shdr->sh_size);
        }
#if 0                                                                                 
        if (shdr->sh_type == SHT_NOBITS) {                                       
            memset ((void *)shdr->sh_addr, 0, shdr->sh_size);                    
        } else {                                                                 
            image = (unsigned char *) addr + shdr->sh_offset;                    
            memcpy ((void *) shdr->sh_addr,                                      
                (const void *) image,                                            
                shdr->sh_size);                                                  
        }      
#endif
    }

    return ehdr->e_entry;
}


int main(int argc, const char *argv[])
{    
    const char *filepath = "/bin/rm";

    int fd = open(filepath, O_RDONLY, (mode_t)0600);
    
    if (fd == -1)
    {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }        
    
    struct stat fileInfo = {0};
    
    if (fstat(fd, &fileInfo) == -1)
    {
        perror("Error getting the file size");
        exit(EXIT_FAILURE);
    }
    
    if (fileInfo.st_size == 0)
    {
        fprintf(stderr, "Error: File is empty, nothing to do\n");
        exit(EXIT_FAILURE);
    }
    
    printf("File size is %ji\n", (intmax_t)fileInfo.st_size);
    
    char *map = mmap(0, fileInfo.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED)
    {
        close(fd);
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }
    
	load_elf_image(map);
    // Don't forget to free the mmapped memory
    if (munmap(map, fileInfo.st_size) == -1)
    {
        close(fd);
        perror("Error un-mmapping the file");
        exit(EXIT_FAILURE);
    }

    // Un-mmaping doesn't close the file, so we still need to do that.
    close(fd);
    
    return 0;
}
