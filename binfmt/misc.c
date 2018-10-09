#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/binfmts.h>
static char *viewer = "/bin/cat";
static int load_elf_binary(struct linux_binprm *bprm)
{
	printk ("kernel asked my module\n");
	int retval=-ENOEXEC;    
	unsigned char *b = bprm->buf;

	if (((b[0]==0xff) && (b[1]==0xd8)))
		retval=0;
	else
		return retval;

	printk("\n%d %d %s %s\n",bprm->argc,bprm->envc,bprm->filename,bprm->interp);
	const char *file_name = bprm->filename;
	const char *const iname_addr= "/usr/bin/eog";
	/* replace argv0 with the full pathname */
	remove_arg_zero(bprm);
	printk ( "%d\n", copy_strings_kernel(1, &iname_addr, bprm));
	bprm->argc++;

	/* and push the viewer's basename to the top of the command line */
	printk ("%d\n",copy_strings_kernel(1,&bprm->filename, bprm));
	bprm->argc++;
	bprm->p++;

	/* very unlikely it's more than 32 pages... */
	/* ok, now open the interpreter, and reenter the loop */
	retval=prepare_binprm(bprm);
	if(retval<0)
		return retval;

	return search_binary_handler(bprm);

	return -ENOEXEC;

}

static struct linux_binfmt elf_format = {
	.module         = THIS_MODULE,
	.load_binary    = load_elf_binary,
	//  .load_shlib     = load_elf_library,
	//  .core_dump      = elf_core_dump,
	//  .min_coredump   = ELF_EXEC_PAGESIZE,
};


static int __init init_elf_binfmt(void)
{
	register_binfmt(&elf_format);
	return 0;
}

static void __exit exit_elf_binfmt(void)
{
	/* Remove the COFF and ELF loaders. */
	unregister_binfmt(&elf_format);
}
module_init(init_elf_binfmt);
module_exit(exit_elf_binfmt);
