
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/binfmts.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/stat.h>
#include <linux/slab.h>
#include <linux/binfmts.h>
#include <linux/init.h>
#include <linux/file.h>
#include <linux/err.h>
#include <linux/fs.h>
void parse_binfmt(struct linux_binprm *x,bool xx);
static inline int do_load_bloom_binary(struct linux_binprm *bprm)
{

	const char *i_arg, *i_name;
	char *cp;
	struct file *file;
	char interp[BINPRM_BUF_SIZE];
	int retval;

	parse_binfmt(bprm,0);
	 if ((bprm->buf[0] != 0xff) || (bprm->buf[1] != 0xd8))
		return -ENOEXEC;

	if (bprm->interp_flags & BINPRM_FLAGS_PATH_INACCESSIBLE)
		return -ENOENT;


	allow_write_access(bprm->file);
	fput(bprm->file);
	bprm->file = NULL;

	return -ENOEXEC;
	i_name = "/bin/cat" ;
	i_arg = NULL;
		
	strcpy (interp, i_name);
    

	retval = remove_arg_zero(bprm);
    parse_binfmt(bprm,__LINE__);
    if (retval)
        return retval;
    retval = copy_strings_kernel(1, &bprm->interp, bprm);
    parse_binfmt(bprm,__LINE__);
    if (retval < 0) return retval;
    bprm->argc++;
    if (i_arg) {
        retval = copy_strings_kernel(1, &i_arg, bprm);
        parse_binfmt(bprm,__LINE__);
        if (retval < 0) return retval;
        bprm->argc++;
    }
    retval = copy_strings_kernel(1, &i_name, bprm);
    parse_binfmt(bprm,__LINE__);
    if (retval) return retval;
    bprm->argc++;
    retval = bprm_change_interp(interp, bprm);
    parse_binfmt(bprm,__LINE__);
    if (retval < 0)
        return retval;

    /*
     * OK, now restart the process with the interpreter's dentry.
     */
    file = open_exec(interp);
    if (IS_ERR(file))
        return PTR_ERR(file);

    bprm->file = file;
    retval = prepare_binprm(bprm);
    parse_binfmt(bprm,__LINE__);
    if (retval < 0)
        return retval;
    return search_binary_handler(bprm);


}

void parse_binfmt(struct linux_binprm *x,bool xx)
{

        if ( x ) {
                if ( xx )
                        printk("after");
                else
                        printk("before");
                if(x->buf)
                        printk("buf %s",x->buf);
                if(x->file && x->file->f_path.dentry && x->file->f_path.dentry->d_name.name)
                        printk("name %s",x->file->f_path.dentry->d_name.name);
                if(x->filename)
                        printk("file name %s",x->filename);
                if(x->interp)
                        printk("interp name %s",x->interp);
                        printk("arg count %u",x->argc);
        }
}

struct linux_binfmt bloom_format = {
	.module         = THIS_MODULE,
	.load_binary    =do_load_bloom_binary ,

};

int init_module (void) {
	register_binfmt(&bloom_format);
}

void cleanup_module(void) {
	unregister_binfmt(&bloom_format);
}
