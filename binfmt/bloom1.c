
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

static inline int do_load_bloom_binary(struct linux_binprm *bprm)
{
		
	return -ENOEXEC;

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
