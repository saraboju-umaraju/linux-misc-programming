#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>

char *attr [] = {
[0x01] =  "DW_AT_sibling               " ,
[0x02] =  "DW_AT_location              " ,
[0x03] =  "DW_AT_name                  " ,
[0x09] =  "DW_AT_ordering              " ,
[0x0b] =  "DW_AT_byte_size             " ,
[0x0c] =  "DW_AT_bit_offset            " ,
[0x0d] =  "DW_AT_bit_size              " ,
[0x10] =  "DW_AT_stmt_list             " ,
[0x11] =  "DW_AT_low_pc                " ,
[0x12] =  "DW_AT_high_pc               " ,
[0x13] =  "DW_AT_language              " ,
[0x15] =  "DW_AT_discr                 " ,
[0x16] =  "DW_AT_discr_value           " ,
[0x17] =  "DW_AT_visibility            " ,
[0x18] =  "DW_AT_import                " ,
[0x19] =  "DW_AT_string_length         " ,
[0x1a] =  "DW_AT_common_reference      " ,
[0x1b] =  "DW_AT_comp_dir              " ,
[0x1c] =  "DW_AT_const_value           " ,
[0x1d] =  "DW_AT_containing_type       " ,
[0x1e] =  "DW_AT_default_value         " ,
[0x20] =  "DW_AT_inline                " ,
[0x21] =  "DW_AT_is_optional           " ,
[0x22] =  "DW_AT_lower_bound           " ,
[0x25] =  "DW_AT_producer              " ,
[0x27] =  "DW_AT_prototyped            " ,
[0x2a] =  "DW_AT_return_addr           " ,
[0x2c] =  "DW_AT_start_scope           " ,
[0x2e] =  "DW_AT_stride_size           " ,
[0x2f] =  "DW_AT_upper_bound           " ,
[0x31] =  "DW_AT_abstract_origin       " ,
[0x32] =  "DW_AT_accessibility         " ,
[0x33] =  "DW_AT_address_class         " ,
[0x34] =  "DW_AT_artificial            " ,
[0x35] =  "DW_AT_base_types            " ,
[0x36] =  "DW_AT_calling_convention    " ,
[0x37] =  "DW_AT_count                 " ,
[0x38] =  "DW_AT_data_member_location  " ,
[0x39] =  "DW_AT_decl_column           " ,
[0x3a] =  "DW_AT_decl_file             " ,
[0x3b] =  "DW_AT_decl_line             " ,
[0x3c] =  "DW_AT_declaration           " ,
[0x3d] =  "DW_AT_discr_list            " ,
[0x3e] =  "DW_AT_encoding              " ,
[0x3f] =  "DW_AT_external              " ,
[0x40] =  "DW_AT_frame_base            " ,
[0x41] =  "DW_AT_friend                " ,
[0x42] =  "DW_AT_identifier_case       " ,
[0x43] =  "DW_AT_macro_info            " ,
[0x44] =  "DW_AT_namelist_item         " ,
[0x45] =  "DW_AT_priority              " ,
[0x46] =  "DW_AT_segment               " ,
[0x47] =  "DW_AT_specification         " ,
[0x48] =  "DW_AT_static_link           " ,
[0x49] =  "DW_AT_type                  " ,
[0x4a] =  "DW_AT_use_location          " ,
[0x4b] =  "DW_AT_variable_parameter    " ,
[0x4c] =  "DW_AT_virtuality            " ,
[0x4d] =  "DW_AT_vtable_elem_location  " 
};

char *tagarr[] = {
  [0x01] = "DW_TAG_array_type"             ,
  [0x02] = "DW_TAG_class_type"             ,
  [0x03] = "DW_TAG_entry_point"            ,
  [0x04] = "DW_TAG_enumeration_type"       ,
  [0x05] = "DW_TAG_formal_parameter"       ,
  [0x08] = "DW_TAG_imported_declaration"   ,
  [0x0a] = "DW_TAG_label                  ",
  [0x0b] = "DW_TAG_lexical_block"          ,
  [0x0d] = "DW_TAG_member"                 ,
  [0x0f] = "DW_TAG_pointer_type"           ,
  [0x10] = "DW_TAG_reference_type"         ,
  [0x11] = "DW_TAG_compile_unit"           ,
  [0x12] = "DW_TAG_string_type"            ,
  [0x13] = "DW_TAG_structure_type"         ,
  [0x15] = "DW_TAG_subroutine_type"        ,
  [0x16] = "DW_TAG_typedef"                ,
  [0x17] = "DW_TAG_union_type"             ,
  [0x18] = "DW_TAG_unspecified_parameters" ,
  [0x19] = "DW_TAG_variant                ",
  [0x1a] = "DW_TAG_common_block           ",
  [0x1b] = "DW_TAG_common_inclusion"       ,
  [0x1c] = "DW_TAG_inheritance"            ,
  [0x1d] = "DW_TAG_inlined_subroutine"     ,
  [0x1e] = "DW_TAG_module                 ",
  [0x1f] = "DW_TAG_ptr_to_member_type"     ,
  [0x20] = "DW_TAG_set_type"               ,
  [0x21] = "DW_TAG_subrange_type"          ,
  [0x22] = "DW_TAG_with_stmt"              ,
  [0x23] = "DW_TAG_access_declaration"     ,
  [0x24] = "DW_TAG_base_type"              ,
  [0x25] = "DW_TAG_catch_block"            ,
  [0x26] = "DW_TAG_const_type"             ,
  [0x27] = "DW_TAG_constant"               ,
  [0x28] = "DW_TAG_enumerator"             ,
  [0x29] = "DW_TAG_file_type"              ,
  [0x2a] = "DW_TAG_friend"                 ,
  [0x2b] = "DW_TAG_namelist"               ,
  [0x2c] = "DW_TAG_namelist_item"          ,
  [0x2d] = "DW_TAG_packed_type"            ,
  [0x2e] = "DW_TAG_subprogram"             ,
  [0x2f] = "DW_TAG_template_type_param"    ,
  [0x30] = "DW_TAG_template_value_param"   ,
  [0x31] = "DW_TAG_thrown_type"            ,
  [0x32] = "DW_TAG_try_block"              ,
  [0x33] = "DW_TAG_variant_part"           ,
  [0x34] = "DW_TAG_variable"               ,
  [0x35] = "DW_TAG_volatile_type"
};

#define dwarf_vma unsigned long
#define bfd_signed_vma unsigned long


/* Read in a LEB128 encoded value starting at address DATA. 
   If SIGN is true, return a signed LEB128 value. 
   If LENGTH_RETURN is not NULL, return in it the number of bytes read. 
   No bytes will be read at address END or beyond.  */ 
 
dwarf_vma 
read_leb128 (unsigned char *data, 
         unsigned int *length_return, 
         bfd_boolean sign, 
         const unsigned char * const end) 
{ 
  dwarf_vma result = 0; 
  unsigned int num_read = 0; 
  unsigned int shift = 0; 
  unsigned char byte = 0; 
 
  while (data < end) 
    { 
      byte = *data++; 
      num_read++; 
 
      result |= ((dwarf_vma) (byte & 0x7f)) << shift; 
 
      shift += 7; 
      if ((byte & 0x80) == 0) 
    break; 
 
      /* PR 17512: file: 0ca183b8. 
     FIXME: Should we signal this error somehow ?  */ 
      if (shift >= sizeof (result) * 8) 
    break; 
    } 
 
  if (length_return != NULL) 
    *length_return = num_read; 
 
  if (sign && (shift < 8 * sizeof (result)) && (byte & 0x40)) 
    result |= -((dwarf_vma) 1 << shift); 
 
  return result; 
}

static inline dwarf_vma 
read_uleb128 (unsigned char * data, 
          unsigned int *  length_return, 
          const unsigned char * const end) 
{ 
  return read_leb128 (data, length_return, FALSE, end); 
} 

void hexdump(char *buf, int size)
{
	int index = 0 ;
	for (; size ; size--)
		printf("%c",*buf++);
}

void print_name(char *base, int off)
{
	return ;
	printf(base+off);
	puts("");
}

static unsigned char * 
process_abbrev_section (unsigned char *start, unsigned char *end) 
{ 
 printf("%x %x\n", start,end);
  while (start < end) 
    { 
      unsigned int bytes_read; 
      unsigned long entry; 
      unsigned long tag; 
      unsigned long attribute; 
      int children; 
	  int implicit_const = 0 ;
 
      entry = read_uleb128 (start, & bytes_read, end); 
      start += bytes_read; 
 
      /* A single zero is supposed to end the section according 
     to the standard.  If there's more, then signal that to 
     the caller.  */ 
      if (start == end) 
    return NULL; 
      if (entry == 0) 
    return start; 
 
      tag = read_uleb128 (start, & bytes_read, end); 
      start += bytes_read; 
      if (start == end) 
    return NULL; 
 
      children = *start++; 
 
      //add_abbrev (entry, tag, children); 

	  //printf ("%lx %ld %d\n", tag, entry, children);
		printf ("%s\n", tagarr[tag]);
 
      do 
    { 
      unsigned long form; 
      /* Initialize it due to a false compiler warning.  */ 
 
      attribute = read_uleb128 (start, & bytes_read, end); 
      start += bytes_read; 
      if (start == end) 
        break; 
 
      form = read_uleb128 (start, & bytes_read, end); 
      start += bytes_read; 
      if (start == end) 
        break; 
#define DW_FORM_implicit_const 0x21
	     if (form == DW_FORM_implicit_const)
        {
          implicit_const = read_uleb128 (start, & bytes_read, end);
          start += bytes_read;
          if (start == end)
        break;
        }
 
	  //printf ("\t %lx %ld \n", attribute, form);
	  if (attribute && attribute < 0x4d)
		  printf ("%s\n", attr[attribute]);
      //add_abbrev_attr (attribute, form, implicit_const); 
    } 
      while (attribute != 0); 
    } 
 
  /* Report the missing single zero which ends the section.  */ 
 
  return NULL; 
}


void parse_sh_hdr(int fd, Elf64_Ehdr *hdr)
{
	unsigned long long off = lseek(fd, 0, SEEK_CUR) ;

	Elf64_Shdr sh_hdr ;

	memset(&sh_hdr, 0, sizeof (Elf64_Shdr));

	int index = 0;

	char *sh_buf = NULL ;

	char *abb = NULL ;

	lseek(fd, hdr->e_shoff, SEEK_SET); 	

	lseek(fd, hdr->e_shstrndx * sizeof (Elf64_Shdr), SEEK_CUR); 	

	read(fd, &sh_hdr, sizeof (Elf64_Shdr));

	printf ("strtble index = %d\n", sh_hdr.sh_offset);

	{
	
		sh_buf = malloc(sh_hdr.sh_size);
		
		lseek(fd, sh_hdr.sh_offset, SEEK_SET); 	
	
		read(fd, sh_buf, sh_hdr.sh_size);

		//hexdump(sh_buf, sh_hdr.sh_size);
	}

	lseek(fd, hdr->e_shoff, SEEK_SET); 	

	for (index = 0 ;index < hdr->e_shnum; index ++) {

		read(fd, &sh_hdr, sizeof (Elf64_Shdr));

		//printf ("section name %d\n", sh_hdr.sh_name);
		
		print_name(sh_buf, sh_hdr.sh_name);

		if (!strcmp(".debug_abbrev", (sh_buf+sh_hdr.sh_name))) {
			abb = malloc (sh_hdr.sh_size);
			printf ("%d\n", sh_hdr.sh_size);
			lseek(fd, sh_hdr.sh_offset, SEEK_SET);
			read(fd, abb, sh_hdr.sh_size);
			process_abbrev_section(abb, abb+sh_hdr.sh_size);
			return ;
		}

		if ( index == hdr->e_shstrndx) {
			printf ("strtble index = %d\n", sh_hdr.sh_offset);
		}

		memset(&sh_hdr, 0, sizeof (Elf64_Shdr));
	}	
}

int main (int argc, char *argv[])
{
	int fd = -1;
	Elf64_Ehdr hdr;
	
	memset(&hdr, 0, sizeof (Elf64_Ehdr));
	fd = open("/tmp/uma", O_RDONLY);
	if (-1 == fd) {
		perror("open");
		return fd ;
	}
	read(fd, &hdr, sizeof (Elf64_Ehdr));

	parse_sh_hdr(fd, &hdr);

//	while(1);
	return 0;
}

