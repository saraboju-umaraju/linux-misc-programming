#include <linux/module.h> // Needed by all modules
#include <linux/kernel.h> // Needed for KERN_INFO
#include <linux/slab.h>   // needed for kmalloc
#include <linux/string.h> // needed for strlen
#include <linux/file.h>

#include <linux/fs.h>
#include <linux/binfmts.h>

#include "winnt_pe_structs.h"
#include "directory_structs.h"


static int load_pe_binary(struct linux_binprm * bprm, struct pt_regs * regs);
static int load_pe_library(struct file *file);
static int pe_core_dump (struct coredump_params *cprm);

static struct linux_binfmt pe_format = {
  .module 	  = THIS_MODULE,
  .load_binary    = load_pe_binary,
  .load_shlib	  = load_pe_library,
  .core_dump	  = pe_core_dump,
  .min_coredump   = PE_EXEC_PAGESIZE
};


const char* hex = "0123456789ABCDEF"; // 1 byte wasted for null terminator but it's much cleaner

// int kernel_read(struct file *source, uint32_t begin, char *destination, uint32_t length);

#define BAILOUT_ERR(condition, err_val, goto_name, ...) if(condition) { printk("\nbinfmt_pe loader failure\nfile: %s\nline: %i\ncondition: %s\nerror: %s", __FILE__, __LINE__, #condition, #err_val); printk(" " __VA_ARGS__); retval = -err_val; goto goto_name; }
#define BAILOUT(condition, ...) BAILOUT_ERR(condition, ENOEXEC, out, __VA_ARGS__)
#define BAILOUT_TO(condition, goto_name, ...) BAILOUT_ERR(condition, ENOEXEC, goto_name, __VA_ARGS__)
#define BAILOUT_MEM(mem, ...) BAILOUT_ERR( mem == NULL, ENOMEM, free_mem_out, __VA_ARGS__)
#define BAILOUT_READ(retval, ... ) BAILOUT_ERR( retval == 0, ENOEXEC, free_mem_out, __VA_ARGS__)

#define CONDITIONAL_ALIGNMENT(boundry, value) if(value % boundry) { value += boundry - (value % boundry); }

#define RVA_ARRAY(addr, index) (mem + ((rva_t*)(mem + addr))[index])

static int load_pe_binary(struct linux_binprm* bprm, struct pt_regs* regs)
{
  void* mem = NULL;
  int retval = 0;
  uint32_t i, j;
  size_t mem_size;
  struct s_directory_data directories;

  struct s_dos_header* dos_header;
  struct s_nt_header* nt_header = NULL;
  struct s_section_header* section_headers = NULL;
  struct s_import_descriptor* import_end;
  struct s_import_descriptor* import_pos;
  rva_t* import_location;
  struct s_export_directory* exported;
  struct s_import_dll* dll;
  struct s_import_function* imported_func;
  struct s_export_function* exported_func;


  // basic DOS type executable check
  dos_header = (struct s_dos_header *) bprm->buf;
  BAILOUT(dos_header->Signature != DOS_SIGNATURE);

  printk("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  printk("\nnt header address: %x", dos_header->NtHeaderAddress);

  // allocate mem for nt_header
  mem = kmalloc(sizeof(struct s_nt_header), GFP_KERNEL);
  BAILOUT_MEM(mem);
  nt_header = (struct s_nt_header*)mem;

  // read the full NT header
  retval = kernel_read(bprm->file, dos_header->NtHeaderAddress, mem, sizeof(struct s_nt_header));
  BAILOUT_READ(retval);

  // basic PE type header check
  BAILOUT(nt_header->Signature != PE_SIGNATURE);

  // check if it's an executable
  BAILOUT(!nt_header->FileHeader.Characteristics.executable_image);

  printk("\narchitecture: %s", nt_header->FileHeader.ArchitectureId == X86_ARCH ? "32-bit" : nt_header->FileHeader.ArchitectureId == AMD64_ARCH ? "64-bit" : "HUH?!");

  // if this fails, the executable file is corrupt
  BAILOUT(nt_header->FileHeader.SizeOfOptionalHeader != sizeof(struct s_optional_header), "\noptional header size: %x\nexpected header size: %lx", nt_header->FileHeader.SizeOfOptionalHeader, sizeof(struct s_optional_header));

  // check for matching architechture
  BAILOUT(nt_header->FileHeader.ArchitectureId != X86_ARCH &&
  nt_header->FileHeader.ArchitectureId != AMD64_ARCH);

  mem_size = nt_header->OptionalHeader.SizeOfImage;
  printk("\nsize of loaded exe: 0x%08lx", mem_size);

  kfree(mem);
  mem = kmalloc(mem_size, GFP_KERNEL);
  BAILOUT_MEM(mem);

  retval = kernel_read(bprm->file, 0, mem, dos_header->NtHeaderAddress + sizeof(struct s_nt_header) + (sizeof(struct s_section_header) * nt_header->FileHeader.NumberOfSections));
  BAILOUT_READ(retval);

  nt_header = (struct s_nt_header*) (mem + dos_header->NtHeaderAddress);
  section_headers = (struct s_section_header*)((char*)nt_header + sizeof(struct s_nt_header));

  printk("\nminimum data chunk size: 0x%04x", nt_header->OptionalHeader.FileAlignment);
  printk("\nnumber of sections: %i", nt_header->FileHeader.NumberOfSections);
  printk("\noptional header size: 0x%08x", nt_header->FileHeader.SizeOfOptionalHeader);
  printk("\nend of nt header: 0x%08lx", dos_header->NtHeaderAddress + sizeof(struct s_nt_header));
  printk("\nnumber of dirs: %i", nt_header->OptionalHeader.NumberOfDirectories);

  for(i = 0; i < nt_header->OptionalHeader.NumberOfDirectories; ++i) { printk("\ndata dir: %02u - location: 0x%08x - size: 0x%08x", i, nt_header->OptionalHeader.DataDirectory[i].VirtualAddress, nt_header->OptionalHeader.DataDirectory[i].Size); }
  for(i = 0; i < nt_header->FileHeader.NumberOfSections; ++i) { printk("\nsection %02u - offset: %08x - file addr: %08x - vaddr: %08x - size/address: 0x%08x - name: %.8s", i, (uint32_t)((void*)(&section_headers[i]) - mem), section_headers[i].PointerToRawData, section_headers[i].VirtualAddress, section_headers[i].PhysicalAddress, section_headers[i].Name); }

  for(i = 0; i < nt_header->FileHeader.NumberOfSections; ++i)
    {
      if(section_headers[i].SizeOfRawData)
        {
          printk("\nread file at: 0x%08x - length: 0x%08x - write to vaddr: 0x%08x", section_headers[i].PointerToRawData, section_headers[i].SizeOfRawData, section_headers[i].VirtualAddress);

          retval = kernel_read(bprm->file, section_headers[i].PointerToRawData, mem + section_headers[i].VirtualAddress, section_headers[i].SizeOfRawData);
          BAILOUT_READ(retval);
        }
    }

  //===================== READ IMPORTS =====================

  CONDITIONAL_ALIGNMENT(nt_header->OptionalHeader.FileAlignment,
                        nt_header->OptionalHeader.DataDirectory[eImport].Size);

  //  printk("\n\nsizeof mod import: %i\n\n", nt_header->OptionalHeader.DataDirectory[eImport].Size % sizeof(struct s_import_descriptor));
  //  BAILOUT(nt_header->OptionalHeader.DataDirectory[eImport].Size % sizeof(struct s_import_descriptor))

  import_pos = mem + nt_header->OptionalHeader.DataDirectory[eImport].VirtualAddress;
  import_end = (void*)import_pos + nt_header->OptionalHeader.DataDirectory[eImport].Size;
  directories.imports.num_dlls = 0;
  while(import_pos->FirstThunk && import_pos != import_end)
    {
      ++import_pos;
      ++directories.imports.num_dlls;
    }

  directories.imports.dlls = dll = kmalloc(sizeof(struct s_import_dll) * i, GFP_KERNEL);

  import_pos = mem + nt_header->OptionalHeader.DataDirectory[eImport].VirtualAddress;
  while(import_pos->FirstThunk && import_pos != import_end)
    {
      dll->dll_name = mem + import_pos->DllName;
      dll->timestamp = import_pos->Timestamp;

      for(dll->num_imports = 0, import_location = mem + import_pos->FirstThunk; *import_location; ++import_location, ++dll->num_imports);
      dll->imported_functions = kmalloc(sizeof(struct s_import_function) * dll->num_imports, GFP_KERNEL);

      imported_func = dll->imported_functions;
      import_location = mem + import_pos->FirstThunk;
      for(i = 0; i < dll->num_imports; ++i)
        {
          dll->imported_functions[i].name    = &((struct s_import_by_name*)(mem + *import_location))->Name;
          dll->imported_functions[i].ordinal =  ((struct s_import_by_name*)(mem + *import_location))->Ordinal;
          ++imported_func;
          ++import_location;
        }

      ++import_pos;
      ++dll;
    }

  //===================== READ EXPORTS =====================

  CONDITIONAL_ALIGNMENT(nt_header->OptionalHeader.FileAlignment,
                        nt_header->OptionalHeader.DataDirectory[eExport].Size);

  exported = mem + nt_header->OptionalHeader.DataDirectory[eExport].VirtualAddress;

  directories.exports.dll_name = mem + exported->DllName;
  directories.exports.num_exported_functions = exported->NumberOfFunctions < exported->NumberOfNames ? exported->NumberOfFunctions : exported->NumberOfNames;
  directories.exports.exported_functions = exported_func = kmalloc(sizeof(struct s_export_function) * directories.exports.num_exported_functions, GFP_KERNEL);

  directories.exports.timestamp = exported->Timestamp;

  for(i = 0; i < directories.exports.num_exported_functions; ++i)
    {
      j = *((uint16_t*)(mem + exported->AddressesOfNameOrdinals) + i);
      exported_func->address = *((uint32_t*)(mem + exported->AddressesOfFunctions) + j);
      exported_func->ordinal = j + exported->OrdinalNumberingBase;
      exported_func->name = RVA_ARRAY(exported->AddressesOfNames, i);
      ++exported_func;
    }



  //===================== DUMP INFO =====================

  //////
  printk("\n\n\nvirtual address of import table: 0x%08x - size: 0x%08x",
         nt_header->OptionalHeader.DataDirectory[eImport].VirtualAddress,
         nt_header->OptionalHeader.DataDirectory[eImport].Size);

  dll = directories.imports.dlls;
  for(i = 0; i < directories.imports.num_dlls; ++i)
    {
      printk("\n\nfilename: %s", dll->dll_name);
      for(j = 0; j < dll->num_imports; ++j)
        {
          printk("\n  ordinal: %04u - name: %s",
                  dll->imported_functions[j].ordinal,
                  dll->imported_functions[j].name);
        }

      ++dll;
    }
  printk("\n\nEND IMPORTS\n");

  //////
  printk("\n\n\nvirtual address of export dir: 0x%08x - size: 0x%08x",
          nt_header->OptionalHeader.DataDirectory[eExport].VirtualAddress,
          nt_header->OptionalHeader.DataDirectory[eExport].Size);

  exported_func = directories.exports.exported_functions;
  printk("\n\nfilename: %s", directories.exports.dll_name);

  for(i = 0; i < directories.exports.num_exported_functions; ++i)
    {
      printk("\n  address: %08x - ordinal: %04u - name: %s",
              directories.exports.exported_functions[i].address,
              directories.exports.exported_functions[i].ordinal,
              directories.exports.exported_functions[i].name);
      ++exported_func;
    }
  printk("\n\nEND EXPORTS\n");

  //////


  //===================================================

  goto load_out;

  setup_new_exec(bprm);
  current;


  set_binfmt(&pe_format);

  goto loader_err;
loader_err:

load_out:

free_mem_out:
  if(mem) { kfree(mem); }
out:
  printk("\n\nPE execution complete\n");
  return retval;
}

static int load_pe_library(struct file* file)
{
  int retval = 0;
  printk("\nchecking library for DLL signature");

  return retval;
}

static int pe_core_dump(struct coredump_params *cprm)
{
  int error = 0;
  return error;
}

static int __init init_pe_binfmt(void)
{
  printk(KERN_INFO "Loading Portable Executable loader module.\n");

  (register_binfmt(&pe_format));
 
	return 0;
}

static void __exit exit_pe_binfmt(void)
{
  printk(KERN_INFO "Unloading Portable Executable loader module.\n");
  unregister_binfmt(&pe_format);
}

core_initcall(init_pe_binfmt);
module_exit(exit_pe_binfmt);

MODULE_LICENSE("Public Domain");
MODULE_AUTHOR("Daniel Wingerd");
MODULE_DESCRIPTION("A loader for Portable Executable formatted files.");
