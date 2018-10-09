#ifndef ENUMS_H
#define ENUMS_H


enum EDirectories
{
  eExport = 0,
  eImport,
  eResource,
  eException,
  eSecurity,            // 04 certificate file
  eBaseRelocationTable, // 05 base relocation table
  eDebug,
  eArchitechtureData,   // 07 architecture specific data
  eGlobalPointer,       // 08 RVA of Global Pointer register
  eTLS,                 // 09 thread local storage (TLS)
  eLoadConfiguration,
  eBoundImport,         // 11 bound import directory in headers
  eIAT,                 // 12 Import Address Table
  eDelayImport,         // 13 delay load import descriptors
  eComDescriptor        // 14 COM+ runtime header (CLR)
//eReserved             // 15 not currently used
};


enum SIGNATURE_ID
{
  DOS_SIGNATURE    = 0x5A4D,      // MZ
  OS2_SIGNATURE    = 0x454E,      // NE
  OS2_LE_SIGNATURE = 0x454C,      // LE
  PE_SIGNATURE     = 0x00004550,  // PE\0\0
  OP_HEADER32      = 0x010B,
  OP_HEADER64      = 0x020B
};

enum IMAGE_SCN           // section types
{
  CNT_CODE               = 0x00000020, // executable code.
  CNT_INITIALIZED_DATA   = 0x00000040, // initialized data - gets a defined value before execution starts
  CNT_UNINITIALIZED_DATA = 0x00000080, // uninitialized data - will be initialized to all-0-bytes before execution starts. This is normally the BSS.
  LNK_INFO               = 0x00000200, // the section doesn't contain image data but comments, description or other documentation. This information is part of an object file and may be information for the linker, such as which libraries are needed.
  LNK_REMOVE             = 0x00000800, // the data is part of an object file's section that is supposed to be left out when the executable file is linked. Often combined with LNK_INFO.
  LNK_COMDAT             = 0x00001000, // "common block data" (COMDAT data), which are packaged functions of some sort. only valid for object files.
  GPREL                  = 0x00008000, // contains data referenced through the global pointer (GP).
  MEM_PURGEABLE          = 0x00020000, // datails purgeable - but I don't think that this is the same as "discardable", which has a bit of its own, see below. The same bit is apparently used to indicate 16-bit-information as there is also a define IMAGE_SCN_MEM_16BIT for it. This bit's meaning is unsure.
  MEM_LOCKED             = 0x00040000, // the section should not be moved in memory? Perhaps it indicates there is no relocation information? This bit's meaning is unsure.
  MEM_PRELOAD            = 0x00080000, // the section should be paged in before execution starts? This bit's meaning is unsure.

  ALIGN_1BYTES           = 0x00100000, // Align data on a 1   -byte boundary. This is valid only for object files.
  ALIGN_2BYTES           = 0x00200000, // Align data on a 2   -byte boundary. This is valid only for object files.
  ALIGN_4BYTES           = 0x00300000, // Align data on a 4   -byte boundary. This is valid only for object files.
  ALIGN_8BYTES           = 0x00400000, // Align data on a 8   -byte boundary. This is valid only for object files.
  ALIGN_16BYTES          = 0x00500000, // Align data on a 16  -byte boundary. This is valid only for object files.
  ALIGN_32BYTES          = 0x00600000, // Align data on a 32  -byte boundary. This is valid only for object files.
  ALIGN_64BYTES          = 0x00700000, // Align data on a 64  -byte boundary. This is valid only for object files.
  ALIGN_128BYTES         = 0x00800000, // Align data on a 128 -byte boundary. This is valid only for object files.
  ALIGN_256BYTES         = 0x00900000, // Align data on a 256 -byte boundary. This is valid only for object files.
  ALIGN_512BYTES         = 0x00A00000, // Align data on a 512 -byte boundary. This is valid only for object files.
  ALIGN_1024BYTES        = 0x00B00000, // Align data on a 1024-byte boundary. This is valid only for object files.
  ALIGN_2048BYTES        = 0x00C00000, // Align data on a 2048-byte boundary. This is valid only for object files.
  ALIGN_4089BYTES        = 0x00D00000, // Align data on a 4089-byte boundary. This is valid only for object files.
  ALIGN_8192BYTES        = 0x00E00000, // Align data on a 8192-byte boundary. This is valid only for object files.

  ALIGNMENT_MASK         = 0x00F00000, // value & ALIGNMENT_MASK == alignment enumeration value

  LNK_NRELOC_OVFL        = 0x01000000, // contains extended relocations. The count of relocations for the section exceeds the 16 bits that is reserved for it in the section header.
  MEM_DISCARDABLE        = 0x02000000, // data is not needed after the process has started. This is the case, for example, with the relocation information. I've seen it also for startup routines of drivers and services that are only executed once, and for import directories.
  MEM_NOT_CACHED         = 0x04000000, // data should not be cached. Don't ask my why not.
  MEM_NOT_PAGED          = 0x08000000, // data should not be paged out. This is interesting for drivers.
  MEM_SHARED             = 0x10000000, // data is shared among all running instances of the image. If it is e.g. the initialized data of a DLL, all running instances of the DLL will at any time have the same variable contents. Note that only the first instance's section is initialized. Sections containing code are always shared copy-on-write (i.e. the sharing doesn't work if relocations are necessary).
  MEM_EXECUTE            = 0x20000000, // can be executed as code.
  MEM_READ               = 0x40000000, // can be read.
  MEM_WRITE              = 0x80000000  // can be written to.
};

enum IMAGE_FILE_MACHINE
{
  UNKNOWN_ARCH   = 0x0000, // Contents assumed to be applicable to any machine type.
  ALPHA_ARCH     = 0x0184, // 32-bit Alpha AXP.
  ARM_ARCH       = 0x01c0, // 32-bit ARM
  ALPHA64_ARCH   = 0x0284, // 64-bit Alpha AXP.
  X86_ARCH       = 0x014c, // 32-bit x86
  IA64_ARCH      = 0x0200, // 64-bit Intel IA64
  M68K_ARCH      = 0x0268, // 16/32-bit Motorola 68000 series.
  MIPS16_ARCH    = 0x0266, // 16-bit MIPS
  MIPSFPU_ARCH   = 0x0366, // 32-bit MIPS with FPU
  MIPSFPU16_ARCH = 0x0466, // 16-bit MIPS with FPU
  POWERPC_ARCH   = 0x01f0, // 32-bit Power PC, little endian.
  R3000_ARCH     = 0x0162, // 32-bit MIPS R3000
  R4000_ARCH     = 0x0166, // 64-bit MIPS R4000 little endian.
  R10000_ARCH    = 0x0168, // 64-bit MIPS R10000
  SH3_ARCH       = 0x01a2, // 32-bit Hitachi SH3
  SH4_ARCH       = 0x01a6, // 32-bit Hitachi SH4
  THUMB_ARCH     = 0x01c2, // 32-bit ARM Thumb
  AMD64_ARCH     = 0x8664  // 64-bit AMD or compatible
};

enum x86_arch
{
  X86_ABSOLUTE          = 0x0000,  // This relocation is ignored.
  X86_DIR16             = 0x0001,  // Not supported.
  X86_REL16             = 0x0002,  // Not supported.
  X86_DIR32             = 0x0006,  // The target's 32-bit virtual address.
  X86_DIR32NB           = 0x0007,  // The target's 32-bit relative virtual address.
  X86_SEG12             = 0x0009,  // Not supported.
  X86_SECTION           = 0x000A,  // The 16-bit-section index of the section containing the target. This is used to support debugging information.
  X86_SECREL            = 0x000B,  // The 32-bit offset of the target from the beginning of its section. This is used to support debugging information as well as static thread local storage.
  X86_REL32             = 0x0014   // The 32-bit relative displacement to the target. This supports the x86 relative branch and call instructions.                            MIPS Processors              The following relocation type indicators are defined for MIPS processors:              Constant              Value              Description
};

enum mips_arch
{
  MIPS_ABSOLUTE         = 0x0000,  // This relocation is ignored.
  MIPS_REFHALF          = 0x0001,  // The high 16 bits of the target's 32-bit virtual address.
  MIPS_REFWORD          = 0x0002,  // The target's 32-bit virtual address.
  MIPS_JMPADDR          = 0x0003,  // The low 26 bits of the target's virtual address. This supports the MIPS J and JAL instructions.
  MIPS_REFHI            = 0x0004,  // The high 16 bits of the target's 32-bit virtual address. Used for the first instruction in a two-instruction sequence that loads a full address. This relocation must be immediately followed by a PAIR relocations whose SymbolTableIndex contains a signed 16-bit displacement which is added to the upper 16 bits taken from the location being relocated.
  MIPS_REFLO            = 0x0005,  // The low 16 bits of the target's virtual address.
  MIPS_GPREL            = 0x0006,  // 16-bit signed displacement of the target relative to the Global Pointer (GP) register.
  MIPS_LITERAL          = 0x0007,  // Same as MIPS_GPREL.
  MIPS_SECTION          = 0x000A,  // The 16-bit section index of the section containing the target. This is used to support debugging information.
  MIPS_SECREL           = 0x000B,  // The 32-bit offset of the target from the beginning of its section. This is used to support debugging information as well as static thread local storage.
  MIPS_SECRELLO         = 0x000C,  // The low 16 bits of the 32-bit offset of the target from the beginning of its section.
  MIPS_SECRELHI         = 0x000D,  // The high 16 bits of the 32-bit offset of the target from the beginning of its section. A PAIR relocation must immediately follow this on. The SymbolTableIndex of the PAIR relocation contains a signed 16-bit displacement, which is added to the upper 16 bits taken from the location being relocated.
  MIPS_JMPADDR16        = 0x0010,  // The low 26 bits of the target's virtual address. This supports the MIPS16 JAL instruction.
  MIPS_REFWORDNB        = 0x0022,  // The target's 32-bit relative virtual address.
  MIPS_PAIR             = 0x0025   // This relocation is only valid when it immediately follows a REFHI or SECRELHI relocation. Its SymbolTableIndex contains a displacement and not an index into the symbol table.
};

enum alpha_arch
{
  ALPHA_ABSOLUTE        = 0x0000,  // This relocation is ignored.
  ALPHA_REFLONG         = 0x0001,  // The target's 32-bit virtual address. This fixup is illegal in a PE32+ image unless the image has been sandboxed by clearing the IMAGE_FILE_LARGE_ADDRESS_AWARE bit in the File Header.
  ALPHA_REFQUAD         = 0x0002,  // The target's 64-bit virtual address.
  ALPHA_GPREL32         = 0x0003,  // 32-bit signed displacement of the target relative to the Global Pointer (GP) register.
  ALPHA_LITERAL         = 0x0004,  // 16-bit signed displacement of the target relative to the Global Pointer (GP) register.
  ALPHA_LITUSE          = 0x0005,  // Reserved for future use.
  ALPHA_GPDISP          = 0x0006,  // Reserved for future use.
  ALPHA_BRADDR          = 0x0007,  // The 21-bit relative displacement to the target. This supports the Alpha relative branch instructions.
  ALPHA_HINT            = 0x0008,  // 14-bit hints to the processor for the target of an Alpha jump instruction.
  ALPHA_INLINE_REFLONG  = 0x0009,  // The target's 32-bit virtual address split into high and low 16-bit parts. Either an ABSOLUTE or MATCH relocation must immediately follow this relocation. The high 16 bits of the target address are stored in the location identified by the INLINE_REFLONG relocation. The low 16 bits are stored four bytes later if the following relocation is of type ABSOLUTE or at a signed displacement given in the SymbolTableIndex if the following relocation is of type MATCH.
  ALPHA_REFHI           = 0x000A,  // The high 16 bits of the target's 32-bit virtual address. Used for the first instruction in a two-instruction sequence that loads a full address. This relocation must be immediately followed by a PAIR relocations whose SymbolTableIndex contains a signed 16-bit displacement which is added to the upper 16 bits taken from the location being relocated.
  ALPHA_REFLO           = 0x000B,  // The low 16 bits of the target's virtual address.
  ALPHA_PAIR            = 0x000C,  // This relocation is only valid when it immediately follows a REFHI , REFQ3, REFQ2, or SECRELHI relocation. Its SymbolTableIndex contains a displacement and not an index into the symbol table.
  ALPHA_MATCH           = 0x000D,  // This relocation is only valid when it immediately follows INLINE_REFLONG relocation. Its SymbolTableIndex contains the displacement in bytes of the location for the matching low address and not an index into the symbol table.
  ALPHA_SECTION         = 0x000E,  // The 16-bit section index of the section containing the target.  This is used to support debugging information.
  ALPHA_SECREL          = 0x000F,  // The 32-bit offset of the target from the beginning of its section. This is used to support debugging information as well as static thread local storage.
  ALPHA_REFLONGNB       = 0x0010,  // The target's 32-bit relative virtual address.
  ALPHA_SECRELLO        = 0x0011,  // The low 16 bits of the 32-bit offset of the target from the beginning of its section.
  ALPHA_SECRELHI        = 0x0012,  // The high 16 bits of the 32-bit offset of the target from the beginning of its section. A PAIR relocation must immediately follow this on. The SymbolTableIndex of the PAIR relocation contains a signed 16-bit displacement which is added to the upper 16 bits taken from the location being relocated.
  ALPHA_REFQ3           = 0x0013,  // The low 16 bits of the high 32 bits of the target's 64-bit virtual address. This relocation must be immediately followed by a PAIR relocations whose SymbolTableIndex contains a signed 32-bit displacement which is added to the 16 bits taken from the location being relocated. The 16 bits in the relocated location are shifted left by 32 before this addition.
  ALPHA_REFQ2           = 0x0014,  // The high 16 bits of the low 32 bits of the target's 64-bit virtual address. This relocation must be immediately followed by a PAIR relocations whose SymbolTableIndex contains a signed 16-bit displacement which is added to the upper 16 bits taken from the location being relocated.
  ALPHA_REFQ1           = 0x0015,  // The low 16 bits of the target's 64-bit virtual address.
  ALPHA_GPRELLO         = 0x0016,  // The low 16 bits of the 32-bit signed displacement of the target relative to the Global Pointer (GP) register.
  ALPHA_GPRELHI         = 0x0017   // The high 16 bits of the 32-bit signed displacement of the target relative to the Global Pointer (GP) register.
};

enum ppc_arch
{
  PPC_ABSOLUTE          = 0x0000,  // This relocation is ignored.
  PPC_ADDR64            = 0x0001,  // The target's 64-bit virtual address.
  PPC_ADDR32            = 0x0002,  // The target's 32-bit virtual address.
  PPC_ADDR24            = 0x0003,  // The low 24 bits of the target's virtual address. This is only valid when the target symbol is absolute and can be sign extended to its original value.
  PPC_ADDR16            = 0x0004,  // The low 16 bits of the target's virtual address.
  PPC_ADDR14            = 0x0005,  // The low 14 bits of the target's virtual address. This is only valid when the target symbol is absolute and can be sign extended to its original value.
  PPC_REL24             = 0x0006,  // A 24-bit PC-relative offset to the symbol's location.
  PPC_REL14             = 0x0007,  // A 14-bit PC-relative offset to the symbol's location.
  PPC_ADDR32NB          = 0x000A,  // The target's 32-bit relative virtual address.
  PPC_SECREL            = 0x000B,  // The 32-bit offset of the target from the beginning of its section. This is used to support debugging information as well as static thread local storage.
  PPC_SECTION           = 0x000C,  // The 16-bit section index of the section containing the target. This is used to support debugging information.
  PPC_SECREL16          = 0x000F,  // The 16-bit offset of the target from the beginning of its section. This is used to support debugging information as well as static thread local storage.
  PPC_REFHI             = 0x0010,  // The high 16 bits of the target's 32-bit virtual address. Used for the first instruction in a two-instruction sequence that loads a full address. This relocation must be immediately followed by a PAIR relocations whose SymbolTableIndex contains a signed 16-bit displacement which is added to the upper 16 bits taken from the location being relocated.
  PPC_REFLO             = 0x0011,  // The low 16 bits of the target's virtual address.
  PPC_PAIR              = 0x0012,  // This relocation is only valid when it immediately follows a REFHI or SECRELHI relocation. Its SymbolTableIndex contains a displacement and not an index into the symbol table.
  PPC_SECRELLO          = 0x0013,  // The low 16 bits of the 32-bit offset of the target from the beginning of its section.
  PPC_SECRELHI          = 0x0014,  // The high 16 bits of the 32-bit offset of the target from the beginning of its section. A PAIR relocation must immediately follow this on. The SymbolTableIndex of the PAIR relocation contains a signed 16-bit displacement which is added to the upper 16 bits taken from the location being relocated.
  PPC_GPREL             = 0x0015   // 16-bit signed displacement of the target relative to the Global Pointer (GP) register.
};

enum sh3_arch
{
  SH3_ABSOLUTE          = 0x0000,  // This relocation is ignored.
  SH3_DIRECT16          = 0x0001,  // Reference to the 16-bit location that contains the virtual address of the target symbol.
  SH3_DIRECT32          = 0x0002,  // The target's 32-bit virtual address.
  SH3_DIRECT8           = 0x0003,  // Reference to the 8-bit location that contains the virtual address of the target symbol.
  SH3_DIRECT8_WORD      = 0x0004,  // Reference to the 8-bit instruction that contains the effective 16-bit virtual address of the target symbol.
  SH3_DIRECT8_LONG      = 0x0005,  // Reference to the 8-bit instruction that contains the effective 32-bit virtual address of the target symbol.
  SH3_DIRECT4           = 0x0006,  // Reference to the 8-bit location whose low 4 bits contain the virtual address of the target symbol.
  SH3_DIRECT4_WORD      = 0x0007,  // Reference to the 8-bit instruction whose low 4 bits contain the effective 16-bit virtual address of the target symbol.
  SH3_DIRECT4_LONG      = 0x0008,  // Reference to the 8-bit instruction whose low 4 bits contain the effective 32-bit virtual address of the target symbol.
  SH3_PCREL8_WORD       = 0x0009,  // Reference to the 8-bit instruction which contains the effective 16-bit relative offset of the target symbol.
  SH3_PCREL8_LONG       = 0x000A,  // Reference to the 8-bit instruction which contains the effective 32-bit relative offset of the target symbol.
  SH3_PCREL12_WORD      = 0x000B,  // Reference to the 16-bit instruction whose low 12 bits contain the effective 16-bit relative offset of the target symbol.
  SH3_STARTOF_SECTION   = 0x000C,  // Reference to a 32-bit location that is the virtual address of the symbol's section.
  SH3_SIZEOF_SECTION    = 0x000D,  // Reference to the 32-bit location that is the size of the symbol's section.
  SH3_SECTION           = 0x000E,  // The 16-bit section index of the section containing the target. This is used to support debugging information.
  SH3_SECREL            = 0x000F,  // The 32-bit offset of the target from the beginning of its section. This is used to support debugging information as well as static thread local storage.
  SH3_DIRECT32_NB       = 0x0010   // The target's 32-bit relative virtual address.
};

enum arm_arch
{
  ARM_ABSOLUTE          = 0x0000,  // This relocation is ignored.
  ARM_ADDR32            = 0x0001,  // The target's 32-bit virtual address.
  ARM_ADDR32NB          = 0x0002,  // The target's 32-bit relative virtual address.
  ARM_BRANCH24          = 0x0003,  // The 24-bit relative displacement to the target.
  ARM_BRANCH11          = 0x0004,  // Reference to a subroutine call, consisting of two 16-bit instructions with 11-bit offsets.
  ARM_SECTION           = 0x000E,  // The 16-bit section index of the section containing the target. This is used to support debugging information.
  ARM_SECREL            = 0x000F   // The 32-bit offset of the target from the beginning of its section. This is used to support debugging information as well as static thread local storage.
};


enum IMAGE_FILE_FLAGS
{
  RELOCS_STRIPPED         = 0x0001, // Relocation information was stripped from the file. The file must be loaded at its preferred base address. If the base address is not available, the loader reports an error.
  EXECUTABLE_IMAGE        = 0x0002, // The file is executable (there are no unresolved external references).
  LINE_NUMS_STRIPPED      = 0x0004, // COFF line numbers were stripped from the file.
  LOCAL_SYMS_STRIPPED     = 0x0008, // COFF symbol table entries were stripped from file.
  AGGRESIVE_WS_TRIM       = 0x0010, // supposed to trim the working set of the running process (the amount of RAM the process uses) aggressivly by paging it out. This should be set if it is a demon-like application that waits most of the time and only wakes up once a day, or the like.
  LARGE_ADDRESS_AWARE     = 0x0020, // The application can handle addresses larger than 2 GB.

  BYTES_REVERSED_LO       = 0x0080, // The bytes of the word are reversed. This flag is obsolete.
  MACHINE_32BIT           = 0x0100, // The computer supports 32-bit words.
  DEBUG_STRIPPED          = 0x0200, // Debugging information was removed and stored separately in another file. This is unused for executable files. According to other information ([6]), this bit is called "fixed" and is set if the image can only run if it is loaded at the preferred load address (i.e. it is not relocatable).
  REMOVABLE_RUN_FROM_SWAP = 0x0400, // file is on removable media, copy it to and run it from the swap file.
  NET_RUN_FROM_SWAP       = 0x0800, // file is on the network, copy it to and run it from the swap file.
  SYSTEM                  = 0x1000, // file is a system file such as a driver. This is unused for executable files; it is also not used in all the NT drivers I inspected.
  DLL                     = 0x2000, // file is a DLL. While it is an executable file, it cannot be run directly.
  SINGLE_THREAD_ONLY      = 0x4000, // file is not designed to run on multiprocessor systems (that is, it will crash there because it relies in some way on exactly one processor).
  BYTES_REVERSED_HI       = 0x8000  // The bytes of the word are reversed. This flag is obsolete.
};

enum IMAGE_SUBSYSTEM
{
  NATIVE      = 1, // The binary doesn't need a subsystem. This is used for drivers.
  WINDOWS_GUI = 2, // The image is a Win32 graphical binary. (It can still open a console with AllocConsole() but won't get one automatically at startup.)
  WINDOWS_CUI = 3, // The binary is a Win32 console binary. (It will get a console per default at startup, or inherit the parent's console.)
  OS2_CUI     = 5, // The binary is a OS/2 console binary. (OS/2 binaries will be in OS/2 format, so this value will seldom be used in a PE file.)
  POSIX_CUI   = 7  // The binary uses the POSIX console subsystem.
};

enum IMAGE_CHARACTERISTICS
{
  PROC_ATTACH           = 0x0001, // process attachment (DLL load).
  THREAD_DETACH         = 0x0002, // thread detachments (thread terminations).
  THREAD_ATTACH         = 0x0004, // thread attachments (thread creations).
  PROC_DETACH           = 0x0008, // process detachment (DLL unload).

  NO_BIND               = 0x0800, // Do not bind image
  WDM_DRIVER            = 0x2000, // Driver is a WDM Driver
  TERMINAL_SERVER_AWARE = 0x8000  // Image is Terminal Server aware
};

#endif
