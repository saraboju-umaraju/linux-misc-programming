/*
 * Demonstrate a NetDev_Chain Notifier Call Chain.
 *
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/netdevice.h>

/*
 * Boilerplate stuff.
 */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("B R Sahu");

static int nb_event(struct notifier_block *this, unsigned long event, void *ptr)
{
  printk("NetDev Event\n");
  return NOTIFY_DONE;
}

static struct notifier_block nb = {
        .notifier_call  = nb_event,
};

/*
 * module initialization.
 */
static int __init NetDevChain_init(void)
{
	return register_netdevice_notifier(&nb);
}

static void __exit NetDevChain_exit(void)
{
	unregister_netdevice_notifier(&nb);
}

module_init(NetDevChain_init);
module_exit(NetDevChain_exit);

