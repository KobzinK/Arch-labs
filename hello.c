// SPDX-License-Identifier: GPL-2-Clause
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_AUTHOR("Kobzin Mykola");
MODULE_DESCRIPTION("Hello world module");
MODULE_LICENSE("Dual BSD/GPL");

static uint how_many_times = 1;
module_param(how_many_times, uint, 0444);
MODULE_PARM_DESC(how_many_times, "Number of times the message is going to be printed");

struct message_log {
	struct list_head list;
	ktime_t log_time;
};

static LIST_HEAD(message_log_list);

static int __init hello_init(void)
{
	if (how_many_times == 0 || (how_many_times >= 5 && how_many_times <= 10)) {
		printk(KERN_WARNING "Warning: how_many_times is %d\n", how_many_times);
	}

	BUG_ON(how_many_times > 10);

	int i = 0;

	for (i = 0; i < how_many_times; i++) {
		struct message_log *msg = kmalloc(sizeof(*msg), GFP_KERNEL);

		if (i == how_many_times - 1) {
			msg = NULL;
		}
		if (msg != NULL) {
			msg->log_time = ktime_get();
			list_add_tail(&msg->list, &message_log_list);
			printk(KERN_INFO "Hello, world!\n");
		} else {
			struct message_log *entry, *temp;
			list_for_each_entry_safe(entry, temp, &message_log_list, list) {
				list_del(&entry->list);
				kfree(entry);

				BUG();
			}
		}
	}
	return 0;
}

static void __exit hello_exit(void)
{
	int i = 0;
	struct message_log *entry, *temp;

	list_for_each_entry_safe(entry, temp, &message_log_list, list) {
		long long ns_time = ktime_to_ns(entry->log_time);

		printk(KERN_INFO "[%u] kernel time = %lld\n", i++, ns_time);
		list_del(&entry->list);
		kfree(entry);
	}
	printk(KERN_INFO "Module was removed.\n");
}

module_init(hello_init);
module_exit(hello_exit);

