#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/binfmts.h>
static int load_elf_binary(struct linux_binprm *bprm)
{
	printk ("kernel asked my module\n");
	int retval = -ENOEXEC;
	int i=0;
	struct file *interp_file = NULL;

	const char *const iname_addr= "eog";
	for(;i<BINPRM_BUF_SIZE;i++){
		if (bprm->buf[i] == 0) 
			break;
		printk("%x ",bprm->buf[i]);
	}

	char *v[] = {"eog","./test.png"};

	printk("\n%d %d %s %s\n",bprm->argc,bprm->envc,bprm->filename,bprm->interp);
	retval = copy_strings_kernel(1, &iname_addr, bprm);
	if (retval < 0)
		return retval;
	bprm->argc++;
	retval = copy_strings_kernel(1, &bprm->interp, bprm);
	if (retval < 0)
		return retval;
	bprm->argc++;

	retval = bprm_change_interp("eog", bprm);
	if (retval < 0)
		return retval;

	if (((bprm->buf[0]==0xff) && (bprm->buf[1]==0xd8)))
		retval=0;
	else
		return retval;
	printk("\n----------%d %d %s %s\n",bprm->argc,bprm->envc,bprm->filename,bprm->interp);
	retval = prepare_binprm(bprm);
	interp_file = open_exec("eog");
	bprm->file = interp_file;


	if (retval < 0)
		return retval;

	retval = search_binary_handler(bprm);
	if (retval < 0)
		return retval;

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
