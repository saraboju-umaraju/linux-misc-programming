#ifndef NEW_WINNT_H
#define NEW_WINNT_H

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif

#define PE_EXEC_PAGESIZE          4096

#include "winnt_pe_enums.h"

/*
  terms used:
  EFO  - Executable File Offset     :  This is simply the offset from the beginning of the executable file.
  RVA  - Relative Virtual Address   :  the offset from the base address of the executable image once it has been mapped into memory
  IAT  - Import Address Table
  EAT  - Export Address Table
  COFF - Common Object File Format
*/

/***************** HELPER STRUCTS *****************/

typedef uint32_t vaddr_t; // virtual address
typedef uint32_t rva_t;   // relative virtual address
typedef uint32_t efo_t;   // executable file offset
typedef uint32_t time32_t;

struct s_version  // (4 bytes) for storing version number
{
  uint16_t Major; // major version (e.g. x in "x.3")
  uint16_t Minor; // minor version (e.g. x in "3.x")
};

struct s_characteristic_flags // (2 bytes)  easy access to IMAGE_CHARACTERISTICS enumeration values
{
  uint16_t process_attach         : 1;  // process attachment (DLL load).
  uint16_t thread_detach          : 1;  // thread detachments (thread terminations).
  uint16_t thread_attach          : 1;  // thread attachments (thread creations).
  uint16_t process_detach         : 1;  // process detachment (DLL unload).
  uint16_t                        : 7;
  uint16_t no_bind                : 1;  // Do not bind image
  uint16_t                        : 1;
  uint16_t wdm_driver             : 1;  // Driver is a WDM Driver
  uint16_t terminal_server_aware  : 1;  // Image is Terminal Server aware
};

struct s_file_flags // (2 bytes)  easy access to IMAGE_FILE_FLAGS enumeration values
{
  uint16_t relocs_stripped      : 1;  // Relocation information was stripped from the file. The file must be loaded at its preferred base address. If the base address is not available, the loader reports an error.
  uint16_t executable_image     : 1;  // The file is executable (there are no unresolved external references).
  uint16_t line_nums_stripped   : 1;  // COFF line numbers were stripped from the file.
  uint16_t local_syms_stripped  : 1;  // COFF symbol table entries were stripped from file.
  uint16_t agressive_ws_trim    : 1;  // supposed to trim the working set of the running process (the amount of RAM the process uses) aggressivly by paging it out. This should be set if it is a demon-like application that waits most of the time and only wakes up once a day, or the like.
  uint16_t large_address_aware  : 1;  // The application can handle addresses larger than 2 GB.
  uint16_t bytes_reversed_low   : 1;  // The bytes of the word are reversed. This flag is obsolete.
  uint16_t machine_32bit        : 1;  // The computer supports 32-bit words.
  uint16_t debug_stripped       : 1;  // Debugging information was removed and stored separately in another file. This is unused for executable files. According to other information ([6]), this bit is called "fixed" and is set if the image can only run if it is loaded at the preferred load address (i.e. it is not relocatable).
  uint16_t media_run_from_swap  : 1;  // file is on removable media, copy it to and run it from the swap file.
  uint16_t net_run_from_swap    : 1;  // file is on the network, copy it to and run it from the swap file.
  uint16_t system               : 1;  // file is a system file such as a driver. This is unused for executable files; it is also not used in all the NT drivers I inspected.
  uint16_t dll                  : 1;  // file is a DLL. While it is an executable file, it cannot be run directly.
  uint16_t single_thread_only   : 1;  // file is not designed to run on multiprocessor systems (that is, it will crash there because it relies in some way on exactly one processor).
  uint16_t bytes_reversed_high  : 1;  // The bytes of the word are reversed. This flag is obsolete.
};

struct s_section_flags  // (4 bytes)  easy access to IMAGE_SCN enumeration values
{
  uint32_t                        : 5;
  uint32_t cnt_code               : 1; // executable code.
  uint32_t cnt_initialized_data   : 1; // initialized data - gets a defined value before execution starts
  uint32_t cnt_uninitialized_data : 1; // uninitialized data - will be initialized to all-0-bytes before execution starts. This is normally the BSS.
  uint32_t                        : 1;
  uint32_t lnk_info               : 1; // the section doesn't contain image data but comments, description or other documentation. This information is part of an object file and may be information for the linker, such as which libraries are needed.
  uint32_t                        : 1;
  uint32_t lnk_remove             : 1; // the data is part of an object file's section that is supposed to be left out when the executable file is linked. Often combined with LNK_INFO.
  uint32_t lnk_comdat             : 1; // "common block data" (COMDAT data), which are packaged functions of some sort. only valid for object files.
  uint32_t                        : 2;
  uint32_t gprel                  : 1; // contains data referenced through the global pointer (GP).
  uint32_t                        : 1;
  uint32_t mem_purgeable          : 1; // datails purgeable - but I don't think that this is the same as "discardable", which has a bit of its own, see below. The same bit is apparently used to indicate 16-bit-information as there is also a define IMAGE_SCN_MEM_16BIT for it. This bit's meaning is unsure.
  uint32_t mem_locked             : 1; // the section should not be moved in memory? Perhaps it indicates there is no relocation information? This bit's meaning is unsure.
  uint32_t mem_preload            : 1; // the section should be paged in before execution starts? This bit's meaning is unsure.

  uint32_t byte_alignment         : 4; // 1 << byte_alignment = num bytes to align

  uint32_t lnk_nreloc_ovfl        : 1; // contains extended relocations. The count of relocations for the section exceeds the 16 bits that is reserved for it in the section header.
  uint32_t mem_discardable        : 1; // data is not needed after the process has started. This is the case, for example, with the relocation information. I've seen it also for startup routines of drivers and services that are only executed once, and for import directories.
  uint32_t mem_not_cached         : 1; // data should not be cached. Don't ask my why not.
  uint32_t mem_not_paged          : 1; // data should not be paged out. This is interesting for drivers.
  uint32_t mem_shared             : 1; // data is shared among all running instances of the image. If it is e.g. the initialized data of a DLL, all running instances of the DLL will at any time have the same variable contents. Note that only the first instance's section is initialized. Sections containing code are always shared copy-on-write (i.e. the sharing doesn't work if relocations are necessary).
  uint32_t mem_execute            : 1; // can be executed as code.
  uint32_t mem_read               : 1; // can be read.
  uint32_t mem_write              : 1; // can be written to.
};


/***************** STANDARD STRUCTS *****************/


struct s_dos_header // (64 bytes) IMAGE_DOS_HEADER  // DOS .EXE header
{
  uint16_t Signature;               // "magic number" of the file. see enumeration MAGIC_NUMBER
  uint16_t BytesInLastBlock;        // The number of bytes in the last block/page of the program that are actually used. If this value is zero, that means the entire last block is used (i.e. the effective value is 512).
  uint16_t BlocksInFile;            // Number of blocks in the file that are part of the EXE file. If non-zero, only that much of the last block is used.
  uint16_t NumberOfRelocations;     // Number of relocation entries stored after the header. May be zero.
  uint16_t NumberOfParagraphs;      // Number of paragraphs in the header. The program's data begins just after the header, and this field can be used to calculate the appropriate file offset. The header includes the relocation entries. Note that some OSs and/or programs may fail if the header is not a multiple of 512 bytes.
  uint16_t MinExtraParagraphs;      // Number of paragraphs of additional memory that the program will need. This is the equivalent of the BSS size in a Unix program. The program can't be loaded if there isn't at least this much memory available to it.
  uint16_t MaxExtraParagraphs;      // Maximum number of paragraphs of additional memory. Normally, the OS reserves all the remaining conventional memory for your program, but you can limit it with this field.
  uint16_t SS_register;             // Relative value of the stack segment. This value is added to the segment the program was loaded at, and the result is used to initialize the SS register.
  uint16_t SP_register;             // Initial value of the SP register.
  uint16_t CheckSum;                // Word checksum. If set properly, the 16-bit sum of all words in the file should be zero. Usually, this isn't filled in.
  uint16_t IP_Register;             // Initial value of the IP register.
  uint16_t CS_Register;             // Initial value of the CS register, relative to the segment the program was loaded at.
  uint16_t RelocationTableAddress;  // EFO of relocation table
  uint16_t OverlayNumber;           // Overlay number. Normally zero, meaning that it's the main program.
  uint16_t Reserved[4];             // Reserved
  uint16_t DeveloperId;             // OEM identifier (for DeveloperInfo)
  uint16_t DeveloperInfo;           // OEM information; DeveloperId specific
  uint16_t Reserved2[10];           // Reserved
  efo_t    NtHeaderAddress;         // EFO of the NT header (s_nt_header)
};

struct s_relocation_data // (4 bytes)
{
  uint16_t Offset;
  uint16_t Segment;
};

// see http://msdn.microsoft.com/en-us/library/ms680313%28v=VS.85%29.aspx
// and http://support.microsoft.com/default.aspx?scid=kb;en-us;q121460
struct s_coff_header // (20 bytes) IMAGE_FILE_HEADER
{
  uint16_t ArchitectureId;              // Number identifying type of target machine.  see enumeration IMAGE_FILE_MACHINE
  uint16_t NumberOfSections;            // Section Table, which immediately follows the headers.
  uint32_t TimeDateStamp;               // unix timedate stamp of when the the file was created (number of seconds since midnight January 1, 1970)
  uint32_t PointerToSymbolTable;        // The offset of the symbol table, in bytes, or zero if no COFF symbol table exists.
  uint32_t NumberOfSymbols;             // Number of entries in the symbol table. This data can be used in locating the string table, which immediately follows the symbol table.
  uint16_t SizeOfOptionalHeader;        // Size of the optional header, which is included for executable files but not object files. An object file should have a value of 0 here.
  struct s_file_flags Characteristics;  // Bit flags indicating attributes of the file.  see struct s_characteristics
};

struct s_optional_header // (220 bytes for 32bit, 240 bytes for 64bit) IMAGE_OPTIONAL_HEADER
{
  //
  // Standard fields.
  //
  uint16_t Signature;                 // "magic number" of the header. 0x010B for 32 bit
                                      //                               0x020B for 64 bit
  struct                              // (2 bytes, not 4 bytes like s_version) for storing version number
  {
    uint8_t Major;                    // major version (e.g. x in "x.3")
    uint8_t Minor;                    // minor version (e.g. x in "3.x")
  } LinkerVersion;                    // the version of the linker that linked this executable.
  uint32_t SizeOfCode;                // Size of executable code.
  uint32_t SizeOfInitializedData;     // Size of initialized data.
  uint32_t SizeOfUninitializedData;   // Size of uninitialized data.
  rva_t AddressOfEntryPoint;       // the location of the entry point for the application and the location of the end of the IAT.
  rva_t BaseOfCode;                // RVA of code (".text" section)
  rva_t BaseOfData;                // RVA of uninitialized data (".bss" section)

  //
  // NT additional fields.
  //
  vaddr_t  BaseAddress;               // Preferred base address in the address space of a process to which the executable image should be mapped
  uint32_t SectionAlignment;          // the minimum amount of space a section can occupy when loaded
  uint32_t FileAlignment;             // Minimum length of chunks of information within the image file prior to loading.
  struct s_version OSVersion;         // version of the Windows NT operating system.
  struct s_version ImageVersion;      // version of the application.
  struct s_version SubsystemVersion;  // version of the Windows NT Win32 subsystem
  uint32_t Reserved;

  uint32_t SizeOfImage;               // the amount of address space to reserve in the address space for the loaded executable image.
  uint32_t SizeOfHeaders;             // how much space in the file is used for representing all the file headers, including the MS-DOS header, PE file header, PE optional header, and PE section headers.
  uint32_t CheckSum;                  // checksum value used to validate the executable file at load time.
  uint16_t Subsystem;                 // identifies the target subsystem for this executable.  see enumeration IMAGE_SUBSYSTEM
  uint16_t Characteristics;           // Flags used to indicate if a DLL image includes entry points for process and thread initialization and termination.  see enumeration IMAGE_CHARACTERISTICS
#define MODE_32BIT
#ifdef MODE_32BIT
  uint32_t SizeOfStackReserve;        // Size of stack to reserve. Only the Stack Commit Size is committed; the rest is made available one page at a time, until reserve size is reached.
  uint32_t SizeOfStackCommit;         // Size of stack to commit.
  uint32_t SizeOfHeapReserve;         // Size of local heap space to reserve. Only the Heap Commit Size is committed; the rest is made available one page at a time, until reserve size is reached.
  uint32_t SizeOfHeapCommit;          // Size of local heap space to commit.
#else
  uint64_t SizeOfStackReserve;        // Size of stack to reserve. Only the Stack Commit Size is committed; the rest is made available one page at a time, until reserve size is reached.
  uint64_t SizeOfStackCommit;         // Size of stack to commit.
  uint64_t SizeOfHeapReserve;         // Size of local heap space to reserve. Only the Heap Commit Size is committed; the rest is made available one page at a time, until reserve size is reached.
  uint64_t SizeOfHeapCommit;          // Size of local heap space to commit.
#endif
  uint32_t LoaderFlags;               // OBSOLETE. whether to break on load, debug on load or load normally
  uint32_t NumberOfDirectories;       // the length of the DataDirectory array. max value: 16
  struct
  {
    rva_t VirtualAddress;             // RVAs to sections
    uint32_t Size;                    // the size of the section
  } DataDirectory[16];                // info about sections.  see s_section_header
};

// see http://msdn.microsoft.com/en-us/library/ms680341%28VS.85%29.aspx
struct s_section_header // (40 bytes) IMAGE_SECTION_HEADER
{
  uint8_t Name[8];                        // arbitrary name of the section; non-terminated string.

  union                                   // OBSOLETE
  {
    uint32_t PhysicalAddress;             // if object: the address the contents is relocated to
    uint32_t VirtualSize;                 // if executable: The total size of the section when loaded into memory, in bytes.
  };

  rva_t VirtualAddress;                   // if executable: RVA of the first byte of the section
                                          // if object: RVA of the first byte before relocation is applied.

  uint32_t SizeOfRawData;                 // The size of the initialized data on disk, in bytes.
  efo_t PointerToRawData;                 // EFO to the first page (raw data) within the COFF file
  efo_t PointerToRelocations;             // EFO to the beginning of the relocation entries for the section. If there are no relocations, this value is zero.
  efo_t PointerToLineNumbers;             // EFO to the beginning of the line-number entries for the section. If there are no COFF line numbers, this value is zero.
  uint16_t NumberOfRelocations;           // The number of relocation entries for the section. This value is zero for executable images.
  uint16_t NumberOfLinenumbers;           // The number of line-number entries for the section.
  struct s_section_flags Characteristics; // bit flags (see IMAGE_SCN)
};

struct s_import_by_name
{
  uint16_t Ordinal;
  char     Name; // name starts here
};

union ThunkData
{
  rva_t ForwarderString;
  rva_t Function;
  uint32_t Ordinal;
  rva_t AddressOfData;
};

struct s_export_directory // (40 bytes) IMAGE_EXPORT_DIRECTORY
{
  struct s_characteristic_flags Characteristics;  // flags (see IMAGE_CHARACTERISTICS)
  time32_t Timestamp;
  struct s_version Version;
  rva_t DllName;                 // exes dont do exports do they?!
  uint32_t OrdinalNumberingBase; // add this to ordinal numbers to get the real ordinal number
  uint32_t NumberOfFunctions;
  uint32_t NumberOfNames;
  rva_t AddressesOfFunctions;    // RVA to list of RVAs
  rva_t AddressesOfNames;        // RVA to list of RVAs
  rva_t AddressesOfNameOrdinals; // RVA to list of RVAs
};

struct s_import_directory // (20 bytes) IMAGE_IMPORT_MODULE_DIRECTORY
{
  rva_t FunctionNames;
  uint32_t : 32;
  uint32_t : 32;
  rva_t ModuleName;
  rva_t FunctionAddresses;
};

struct s_import_descriptor // (20 bytes) IMAGE_IMPORT_DESCRIPTOR
{
  union
  {
    struct s_characteristic_flags Characteristics;  // flags (see IMAGE_CHARACTERISTICS)
    uint32_t OriginalFirstThunk;                    // RVA to original unbound IAT (*IMAGE_THUNK_DATA)
  };                                                // 0 for terminating null import descriptor
  union
  {
    uint32_t bound;         // 0 if not bound, -1 if bound
    uint32_t Timestamp;     // and real date\time stamp in IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT (new BIND)
  };                        // O.W. date/time stamp of DLL bound to (Old BIND)

  rva_t ForwarderChain;  // -1 if no forwarders
  rva_t DllName;         // RVA to name of the DLL it's importing from
  rva_t FirstThunk;      // RVA to IAT (if bound this IAT has actual addresses)
};

struct s_nt_header // (244 bytes) IMAGE_NT_HEADERS
{
  uint32_t Signature;                      // "magic number" of the header. should be 0x00004550 = PE\0\0
  struct s_coff_header FileHeader;
  struct s_optional_header OptionalHeader;
};


#endif
