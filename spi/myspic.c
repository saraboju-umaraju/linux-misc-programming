
#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/compat.h>
#include <linux/of.h>
#include <linux/of_device.h>

#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>

#include <linux/uaccess.h>

#define SPI_MAJOR 154
#define N_SPI_MINORS 32

static DECLARE_BITMAP(minors, N_SPI_MINORS);
static LIST_HEAD(device_list);

int count = 1;

static struct class *spidev_class;

ssize_t spidev_read (struct file *, char __user *, size_t, loff_t *);
int spidev_open(struct inode *, struct file *);
int spidev_release(struct inode *, struct file *);

struct spi_device *spi_t;
struct spidev_data {
	dev_t			devt;
	spinlock_t		spi_lock;
	struct spi_device	*spi;
	struct list_head	device_entry;

	/* TX/RX buffers are NULL unless this device is open (users > 0) */
	struct mutex		buf_lock;
	unsigned		users;
	u8			*tx_buffer;
	u8			*rx_buffer;
	u32			speed_hz;
};


static const struct file_operations spidev_fops = {
	.owner =	THIS_MODULE,
//	.write =	spidev_write,
	.read =		spidev_read,
	.open =		spidev_open,
	.release =	spidev_release,
};


int spidev_open(struct inode * in, struct file * fl)
{
	printk("Device Open\n");
	return 0;
}
int spidev_release(struct inode * in, struct file * fl)
{
	printk("Device Closed\n");
	return 0;
}

ssize_t spidev_read (struct file * fp, char __user * ch, size_t sz, loff_t * lf)
{
	struct spi_device * spi; 
	struct spi_transfer	t[2];
	struct spi_message	m;
	char *buf , res[3];
	int count,status;
	u8 cmd[3]={0x01,0xE0,0xFF};

	printk("********\n");

	spi = fp -> private_data;
	buf = res;
	count = 24;

	printk("%d %d\n",spi_t->master->bus_num,spi_t->chip_select);

	spi_message_init(&m);
	memset(t, 0, sizeof t);

	printk("********\n");
	t[0].tx_buf = cmd;
	t[0].len = count;
//	t[0].rx_buf = buf;
	spi_message_add_tail(&t[0], &m);

	if(spi_t == NULL)	
		printk("Null\n");
	else{

	printk("********\n");
	status = spi_sync(spi_t, &m);
	
	printk("********\n");
	printk("%c\t%c\t%c\n",res[0],res[1],res[2]);

	printk("********\n");
	}
	return count;
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
	struct spidev_data	*spidev;
	int	status;
	unsigned long minor;
	struct device *dev;

	printk("SPI Probe Function %d\n",count++);
	
	spi_t = spi;

/*	if (spi->dev.of_node && !of_match_device(mydev_ids, &spi->dev)) {
		dev_err(&spi->dev, "buggy DT: spidev listed directly in DT\n");
		WARN_ON(spi->dev.of_node &&
			!of_match_device(mydev_ids, &spi->dev));
	}
	else printk("Successfull\n");
*/
	spidev = kzalloc(sizeof(*spidev), GFP_KERNEL);
	if (!spidev)
		return -ENOMEM;

	spidev->spi = spi;

	INIT_LIST_HEAD(&spidev->device_entry);

	minor = find_first_zero_bit(minors, N_SPI_MINORS);

	spidev -> devt = MKDEV(SPI_MAJOR,minor);
	dev = device_create(spidev_class, &spi->dev, spidev->devt,
				    spidev, "mySPIdev%d.%d",
				    spi->master->bus_num, spi->chip_select);
	status = PTR_ERR_OR_ZERO(dev);
		
//	printk("%d %d\n",spi->master->bus_num,spi->chip_select);

	if(dev == NULL)
		printk("NULL\n");

	if (status == 0) {
		set_bit(minor, minors);
		list_add(&spidev->device_entry, &device_list);
	}

	spidev->speed_hz = spi->max_speed_hz;

	if (status == 0)
		spi_set_drvdata(spi, spidev);
	else
		kfree(spidev);

	return status;

}

static int spidev_remove(struct spi_device *spi)
{
	struct spidev_data	*spidev = spi_get_drvdata(spi);

	printk("SPI Remove Function\n");

	spidev->spi = NULL;

	list_del(&spidev->device_entry);
	device_destroy(spidev_class, spidev->devt);
	clear_bit(MINOR(spidev->devt), minors);
	if (spidev->users == 0)
		kfree(spidev);

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
	int status;

	printk("init %d \n",count++);
	status = register_chrdev(SPI_MAJOR, "mySPIdev", &spidev_fops);
	if (status < 0){
		printk("unsucessful\n");
		return status;
	}

	spidev_class = class_create(THIS_MODULE, "mySPIdev");
	if (IS_ERR(spidev_class)) {
		unregister_chrdev(SPI_MAJOR, my_driver.driver.name);
		return PTR_ERR(spidev_class);
	}

	status = spi_register_driver(&my_driver);
	if (status < 0) {
		printk("Unsuccessful\n");		
	}
	return status;
}

static void __exit mySPI_exit(void)
{
	spi_unregister_driver(&my_driver);
	class_destroy(spidev_class);
	unregister_chrdev(SPI_MAJOR, my_driver.driver.name);
}
//MODULE_ALIAS("spi:spidev");
MODULE_LICENSE("GPL");
module_init(mySPI_init);
module_exit(mySPI_exit);
