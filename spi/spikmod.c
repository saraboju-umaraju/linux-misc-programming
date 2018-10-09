
#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/sched.h>
#include<linux/cdev.h>
#include<asm/uaccess.h>
#include<linux/device.h>
#include<linux/slab.h>
#include<linux/kobject.h>
#include <linux/sysfs.h>
//#include <linux/stat.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/compat.h>
#include <linux/of.h>
#include <linux/of_device.h>

#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>


MODULE_LICENSE("Dual BSD/GPL");

/** Constants **/
#define BUFFER_SIZE 1024

//Function Declarations for syscall definitions
int myOpen(struct inode *inode, struct file *filep);
int myRelease(struct inode *in, struct file *fp);

// InitiaLIZATION ROUTINES
static int mySPI_init(void);
static void mySPI_exit(void);

//struct attribute attr;    //pointer to the srtucture
struct kobject *k_obj;
//static int foo;
struct spi_device *spi_t;

static ssize_t spi_show(struct kobject *kobj, struct kobj_attribute *attr,
                char *buf);

static ssize_t spi_store(struct kobject *kobj, struct kobj_attribute *attr, 
                const char *buf, size_t count);    

int mywrite(void);

char my_buffer[BUFFER_SIZE];	// Temporary buffer to store written data in to the file


int myOpen(struct inode *inode, struct file *filep)
{

	printk( /*"4"*KERN_WARNING* KERN_DEFAULT */ KERN_INFO
	       "Open succesful\n");
	return 0;
}

static struct kobj_attribute spi_attribute =__ATTR(spidev, S_IWUSR | S_IRUGO, spi_show,
                spi_store);
                                                                                
/* show and store functions written by me */
static ssize_t spi_show(struct kobject *kobj, struct kobj_attribute *attr,      
                char *buf)                                                              
{   
	printk ("INSIDE SHOW\nmy_buffer = %s\n", my_buffer);

	return 1;
//	return sprintf(buf, "%d\n", foo);                                           
}
                                                                                
static ssize_t spi_store(struct kobject *kobj, struct kobj_attribute *attr,     
                const char *buf, size_t count)                                                
{
	mywrite();
	return 1;
}

int mywrite(void)
{
	struct spi_transfer t[2];
    struct spi_message  m;
	int status;
	int count;
    u8 cmd[3]={0x01,0x80,0xff};
	u8 res[3];

    spi_message_init(&m);
    memset(t, 0, sizeof t);

    t[0].tx_buf = cmd;

	count = 3;
    t[0].len = count;

    t[0].rx_buf = res;
    spi_message_add_tail(&t[0], &m);

    status = spi_sync(spi_t, &m);

	if(status == 0)
		printk("%x %x %x",res[0], res[1], res[2]);

	return count;                                                               
}      

int myRelease(struct inode *in, struct file *fp)
{
	printk(KERN_INFO "File released\n");
	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id mydev_ids[] = {
	{ .compatible = "mySPIdev" },
	{},
};

MODULE_DEVICE_TABLE(of, mydev_ids);
#endif


static int spidev_probe(struct spi_device *spi)
{
	printk("Probe function\n");
	spi_t = spi;
	
//	mywrite();
	return 0;

}

static int spidev_remove(struct spi_device *spi)
{
	return 0;

}

static struct spi_driver my_driver = {
	.driver = {
		.name =		"mySPIdev",
		.of_match_table = of_match_ptr(mydev_ids),
	},
	.probe =	spidev_probe,
	.remove =	spidev_remove,

};

static int __init mySPI_init(void)
{
//	int ret = -ENODEV;
	int status;
    int crt_file;

	printk(KERN_INFO "Initializing Character Device\n");


	k_obj = kobject_create_and_add("spiDev",NULL);
    crt_file = sysfs_create_file(k_obj, &spi_attribute.attr);


	status = spi_register_driver(&my_driver);
	if (status < 0) {
		printk("Unsuccessful\n");		
	}
	return status;


}

/*
 * myExit:cleanup function of the kernel module
 */
static void mySPI_exit(void)
{
	printk(KERN_INFO "Exiting the Character Driver\n");
	spi_unregister_driver(&my_driver);
    kobject_del(k_obj);
    return;
}

module_init(mySPI_init);
module_exit(mySPI_exit);
