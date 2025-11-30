#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/bug.h>

extern void print_hello(int count);

static int print_count = 1;
module_param(print_count, int, 0);
MODULE_PARM_DESC(print_count, "The number of times to call print_hello.");

static int __init hello2_init(void)
{
    printk(KERN_INFO "hello2: Caller module loaded. Call count: %d\n", print_count);

    if (print_count == 12) {
        printk(KERN_ALERT "hello2: Critical parameter error detected (%d). Triggering BUG_ON().\n", print_count);
        BUG_ON(print_count == 12);
        return -EINVAL;
    }

    if (print_count > 0 && print_count >= 5 && print_count <= 10) {
        printk(KERN_WARNING "hello2: Call count is 0 or between 5 and 10. Warning issued.\n");
    }

    print_hello(print_count);

    return 0;
}

static void __exit hello2_exit(void)
{
    printk(KERN_INFO "hello2: Caller module unloaded.\n");
}

module_init(hello2_init);
module_exit(hello2_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Darina");
MODULE_DESCRIPTION("Caller module for testing hello1 functionality");
