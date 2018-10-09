/*
 * interfacing with proc filesystem
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define DIRECTORY_NAME  "park"
#define FILE_NAME       "rw_counter"

static struct proc_dir_entry *park_dir, *counter_file;

int counter=1;
int temp=1;

static int proc_read_counter(struct file *filp, char *buf, size_t count, loff_t *offset)
{
        int len;

//brs        if (offset > 0) return 0;
        if (temp == 0) return 0;

        len = sprintf(buf, "Counter = %d\n", counter++);
	temp = 0;
        return len;

}

static int proc_write_counter(struct file *filp, const char *buf, size_t count, loff_t *offset)
{
        int len;
        char inbuf[10];

        if (count > 8) len = 8;
        else len = count;

        if(copy_from_user(inbuf, buf, len))
        {
                return -EFAULT;
        }
        inbuf[len] =  '\0' ;

	counter = simple_strtol(inbuf, NULL,10);
	temp = len;
        return len;
}

struct file_operations proc_fops = 
{
  .read = proc_read_counter,
  .write = proc_write_counter
};

int init_module(void)
{
        int sts = 0; /* create directory */

        park_dir = proc_mkdir(DIRECTORY_NAME, NULL);
        if(park_dir == NULL)
        {
                printk("ERROR Creating Directory\n");
                sts = -1;
                goto exit_dir;
        }

        counter_file = proc_create(FILE_NAME, 0644, park_dir, &proc_fops);
        if(counter_file == NULL)
        {        sts = -1;
                 goto exit_file;
        }

        return 0;

exit_file: 
	remove_proc_entry(DIRECTORY_NAME, NULL);

exit_dir: 
	return sts;

}

void cleanup_module(void)
{
        remove_proc_entry(FILE_NAME, park_dir);
        remove_proc_entry(DIRECTORY_NAME, NULL);
}

MODULE_LICENSE("GPL");

