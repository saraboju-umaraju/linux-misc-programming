/* For the size of the file. */
#include <sys/stat.h>
#include <elf.h>
/* This contains the mmap calls. */
#include <sys/mman.h> 
/* These are for error printing. */
#include <errno.h>
#include <string.h>
#include <stdarg.h>
/* This is for open. */
#include <fcntl.h>
#include <stdio.h>
/* For exit. */
#include <stdlib.h>
/* For the final part of the example. */
#include <ctype.h>

/* "check" checks "test" and prints an error and exits if it is
   true. */
unsigned long load_elf_image (unsigned long addr)                                
{                                                                                
    Elf32_Ehdr *ehdr;       /* Elf header structure pointer     */               
    Elf32_Shdr *shdr;       /* Section header structure pointer */               
    unsigned char *strtab = 0;  /* String table pointer             */           
    unsigned char *image;       /* Binary image pointer             */           
    int i;              /* Loop counter                     */                   
                                                                                 
    /* -------------------------------------------------- */                     
                                                                                 
    ehdr = (Elf32_Ehdr *) addr;                                                  
                                                                                 
    /* Find the section header string table for output info */                   
    shdr = (Elf32_Shdr *) (addr + ehdr->e_shoff +                                
                   (ehdr->e_shstrndx * sizeof (Elf32_Shdr)));                    
                                                                                 
    if (shdr->sh_type == SHT_STRTAB)                                             
        strtab = (unsigned char *) (addr + shdr->sh_offset);                     
                                                                                 
    /* Load each appropriate section */                                          
    for (i = 0; i < ehdr->e_shnum; ++i) {                                        
        shdr = (Elf32_Shdr *) (addr + ehdr->e_shoff +                            
                       (i * sizeof (Elf32_Shdr)));                               
                                                                                 
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
             /*                                                                    
        if (shdr->sh_type == SHT_NOBITS) {                                       
            //memset ((void *)shdr->sh_addr, 0, shdr->sh_size);                    
        } else {                                                                 
            image = (unsigned char *) addr + shdr->sh_offset;                    
            //memcpy ((void *) shdr->sh_addr,                                      
                (const void *) image,                                            
                shdr->sh_size);                                                  
        }       */                                                                 
    }                                                                            
                                                                                 
    return ehdr->e_entry;                                                        
}
static void
check (int test, const char * message, ...)
{
    if (test) {
        va_list args;
        va_start (args, message);
        vfprintf (stderr, message, args);
        va_end (args);
        fprintf (stderr, "\n");
        exit (EXIT_FAILURE);
    }
}

int main ()
{
    /* The file descriptor. */
    int fd;
    /* Information about the file. */
    struct stat s;
    int status;
    size_t size;
    /* The file name to open. */
    const char * file_name = "me.c";
    /* The memory-mapped thing itself. */
    const char * mapped;
    int i;

    /* Open the file for reading. */
    fd = open ("test.elf", O_RDONLY);
    check (fd < 0, "open %s failed: %s", file_name, strerror (errno));

    /* Get the size of the file. */
    status = fstat (fd, & s);
    check (status < 0, "stat %s failed: %s", file_name, strerror (errno));
    size = s.st_size;

    /* Memory-map the file. */
    mapped = mmap (0, size, PROT_READ, 0, fd, 0);
    check (mapped == MAP_FAILED, "mmap %s failed: %s",
           file_name, strerror (errno));

    /* Now do something with the information. */
    for (i = 0; i < size; i++) {
        char c;

        c = mapped[i];
        if (! isalpha (c) && ! isspace (c)) {
            putchar (c);
        }
        if (i % 80 == 79) {
            putchar ('\n');
        }
    }

    return 0;
}
