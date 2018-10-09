#include <linux/init.h>
#include <linux/list.h>
#include <linux/reboot.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/sched/signal.h>

struct task {
		pid_t pid;
		char comm[32];
		struct list_head mylist; 
};
/*
   int notifier_fn(struct notifier_block *nb,unsigned long action, void *data)
   {
   printk(" __UMA__ %s %s %d",__FILE__,__func__,__LINE__); 
   msleep(10000);
   return 0;
   }
   struct notifier_block nb = {
   .notifier_call = notifier_fn,
   };
 */

struct list_head *task_list_HEAD = NULL;


int init_module()
{
		struct task_struct *tsk;
		struct task *inst = NULL;
		int flag = 0 ;


		for_each_process(tsk) {

				inst = kmalloc(sizeof(struct task),GFP_KERNEL);
				if (inst == NULL) {
						return -ENOMEM;
				}

				inst->pid = current->pid;
				strncpy(inst->comm,tsk->comm,31);	

				if (flag == 0) {
						//	INIT_LIST_HEAD(&inst->mylist);
						inst->mylist.next = &inst->mylist.next;
						inst->mylist.prev = &inst->mylist.prev;
						task_list_HEAD = &inst->mylist;
						flag = 1;
				} else{
						list_add(&inst->mylist, task_list_HEAD);
				}
		}
		//register_reboot_notifier(&nb);
		return 0;
}
void cleanup_module()
{
		struct task *t;
		list_for_each_entry(t, task_list_HEAD, mylist) {
				printk("%s",t->comm);
				kfree(t);
		}

		//unregister_reboot_notifier(&nb);
		return ;
}
