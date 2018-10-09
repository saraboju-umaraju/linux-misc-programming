#ifndef DIRECTORIES_H
#define DIRECTORIES_H

#include "winnt_pe_structs.h"

// DIRECTORY DATA STRUCTURES


//IMPORT

struct s_forward_function // data to lookup forwarded functions
{
  char* dll_name;   // DLL filename containing function
  uint16_t ordinal; // function identifier
  char* name;       // function name
};

struct s_import_function // imports from another DLL
{
  uint16_t ordinal; // function identifier
  char* name;       // function name
};

struct s_import_dll
{
  char* dll_name;                                 // original DLL filename
  struct s_characteristic_flags characteristics;  // flags (see IMAGE_CHARACTERISTICS)
  uint32_t timestamp;                             // DLL Compilation date

  uint32_t num_forwards;
  struct s_forward_function* forwarded_functions;

  uint32_t num_imports;
  struct s_import_function* imported_functions;
};

struct s_import_data
{
  uint32_t num_dlls;
  struct s_import_dll* dlls;
};

// EXPORT
struct s_export_function // exports functions for other EXEs and DLLs
{
  uint16_t ordinal; // function identifier
  char* name;       // function name
  rva_t address; // 32bit "pointer" to function
};


struct s_export_data // (40 bytes) IMAGE_EXPORT_DIRECTORY
{
  char* dll_name;                                 // original DLL filename
  uint32_t timestamp;                             // DLL Compilation date
  struct s_version version;                       // version numbers
  uint32_t num_exported_functions;
  struct s_export_function* exported_functions;   // function info
};


// DIRECTORIES

struct s_directory_data
{
  struct s_import_data imports;
  struct s_export_data exports;
};


#endif
