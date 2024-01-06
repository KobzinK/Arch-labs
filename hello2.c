// SPDX-License-Identifier: GPL-2-Clause
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include "hello1.h"

MODULE_AUTHOR("Kobzin Mykola");
MODULE_DESCRIPTION("Hello world module");
MODULE_LICENSE("Dual BSD/GPL");

static uint how_many_times = 1;
module_param(how_many_times, uint, 0444);
MODULE_PARM_DESC(how_many_times, "how many times the greeting is going to be printed");


static int __init hello_init(void)
{
	if (how_many_times == 0 || (how_many_times >= 5 && how_many_times <= 10)) {
		pr_warn("Warning: how_many_times is %u\n", how_many_times);
	} else if (how_many_times > 10) {
		pr_err("Error: how_many_times is more then 10\n");
		return -EINVAL;
	}
	print_hello(how_many_times);
	return 0;
}

static void __exit hello_exit(void)
{
	pr_info("Module hello2 was removed.\n");
}

module_init(hello_init);
module_exit(hello_exit);
