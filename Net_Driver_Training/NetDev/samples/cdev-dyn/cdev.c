/*
 * A sample device driver, uses dynamic major/minor and cdev registraion 
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define FIRST_MINOR	0
#define MINOR_CNT	1

static dev_t devno;
char kbuf[64];
struct _MyData {
    long MyInt;
    char MyString[16];
    } *MyDataPtr;

/************************************************************************************
****	When application calls int open(const char *pathname, int flags),	 ****
****	my_open() method will be invoked by the Kernel, which returns		 ****
****	the file descriptor for the device or -1 if any error occurs		 ****
************************************************************************************/

static int my_open(struct inode *inode, struct file *filp)
{
	int major;
        int minor;

        major = imajor(inode);
        minor = iminor(inode);
        printk("\n*****Some body is opening me at major %d  minor %d*****\n",major, minor);
        return 0;
}

/************************************************************************************
****	When application calls int close(int fd), where fd is the file		 ****
****	descriptor returned by corresponding open(),				 ****
****	my_open() method will be invoked by the Kernel, which returns		 ****
****	0 on success and -1 on error						 ****
************************************************************************************/

static int my_close(struct inode *inode, struct file *filp)
{
	int minor;
//        minor = MINOR(filp->f_dentry->d_inode->i_rdev);
        printk("*****Some body is closing me at minor %d*****\n",minor);
        return 0;
}

/************************************************************************************
****	When application calls ssize_t read(int fd, void *buf, size_t count),	 ****
****	where fd is the file descriptor returned by corresponding open(),	 ****
****	buf is user buffer, and count is the number of bytes requested,		 ****
****	my_read() method will be invoked by the Kernel, which returns	         ****
****	number of bytes actually transferred or -1 on error			 ****
************************************************************************************/

ssize_t my_read(struct file *filp, char *buf, size_t count, loff_t *offset)
{
	int minor;
//        minor = MINOR(filp->f_dentry->d_inode->i_rdev);
        printk("*****Some body is reading me at minor %d*****\n",minor);
        copy_to_user(buf, kbuf, count);
        return count;
}

/************************************************************************************
****	When application calls ssize_t write(int fd, const void *buf, size_t count),*
****	where fd is the file descriptor returned by corresponding open(),	 ****
****	buf is user buffer with data, and count is the number of bytes to be written,
****	my_write() method will be invoked by the Kernel, which returns	 ****
****	number of bytes actually transferred or -1 on error			 ****
************************************************************************************/

ssize_t my_write(struct file *filp, const char *buf, size_t count, loff_t *offset)
{
	int minor;
//        minor = MINOR(filp->f_dentry->d_inode->i_rdev);
        printk("*****Some body is writing me at minor %d*****\n",minor);
        copy_from_user(kbuf, buf, count);
        kbuf[count] = 0;
        printk("User Data: %s\n", kbuf);
        return count;
}

/************************************************************************************
****	When application calls int ioctl(int fd, int request char *argp),        ****
****	where fd is the file descriptor returned by corresponding open(),	 ****
****	request is command, and argp is a pointer to argument list,              ***
****	my_ioctl() method will be invoked by the Kernel, which returns	 ****
****	0 or a positive integer on success or -1 on error			 ****
************************************************************************************/

static long my_control(struct file *filp, unsigned int command, unsigned long argument)
{
	int minor;
//	minor = MINOR(filp->f_dentry->d_inode->i_rdev);
	printk("*****Some body is controlling me at minor %d*****\n",minor);
        printk("Ioctl code %d\n", command);
        MyDataPtr = (struct _MyData *)argument;
        printk("Int %ld Str %s\n", MyDataPtr->MyInt, MyDataPtr->MyString);
        return 0;
}

struct file_operations my_fops = {
        .read = my_read,
        .write = my_write,
        .open = my_open,
        .release = my_close,
        .unlocked_ioctl = my_control,
        .owner = THIS_MODULE
        };

struct cdev my_cdev;

int my_init(void)
{
	int err;

        printk("<1>Hello World\n");

	alloc_chrdev_region(&devno, FIRST_MINOR, MINOR_CNT, "mydevice");

        cdev_init(&my_cdev, &my_fops);

	err = cdev_add(&my_cdev, devno, MINOR_CNT);
	if (err < 0)
	{
		printk("Device Add Error\n");
		return -1;
	}

        return 0;
}

void my_cleanup(void)
{
        printk("<1>Goodbye\n");

	unregister_chrdev_region(devno, MINOR_CNT);
	cdev_del(&my_cdev);
}

module_init(my_init);
module_exit(my_cleanup);

MODULE_LICENSE("GPL");

