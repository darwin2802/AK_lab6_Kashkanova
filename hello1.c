#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/delay.h>
#include <linux/bug.h>

struct time_data {
    ktime_t start_time;
    ktime_t end_time;
    struct list_head list;
};

static LIST_HEAD(time_list);

static void __list_add_data(ktime_t start, ktime_t end)
{
    struct time_data *data;

    if (list_empty(&time_list)) {
        printk(KERN_INFO "hello1: Initializing list for timing data.\n");
    }

    if (list_entry_count(&time_list) == 4) {
        printk(KERN_DEBUG "hello1: DEBUG: Simulating kmalloc failure for entry #5.\n");
        data = NULL;
    } else {
        data = kmalloc(sizeof(*data), GFP_KERNEL);
    }

    if (!data) {
        printk(KERN_ERR "hello1: Failed to allocate memory for list entry. Aborting list operation.\n");
        return;
    }

    data->start_time = start;
    data->end_time = end;
    list_add_tail(&data->list, &time_list);
}

void print_hello(int count)
{
    int i;
    ktime_t start, end;
    for (i = 1; i <= count; i++) {
        start = ktime_get();
        printk(KERN_INFO "hello: Hello, world! (Entry %d)\n", i);
        ndelay(300000);
        end = ktime_get();
        __list_add_data(start, end);
    }
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
    printk(KERN_INFO "hello1: Base module loaded. Exporting print_hello.\n");
    return 0;
}

static void __exit hello1_exit(void)
{
    struct time_data *data, *tmp;
    int count = 0;
    long long duration_ns;

    printk(KERN_INFO "hello1: Module unloading. Printing recorded durations:\n");

    list_for_each_entry_safe(data, tmp, &time_list, list) {
        duration_ns = ktime_to_ns(ktime_sub(data->end_time, data->start_time));
        printk(KERN_INFO "hello1: Event %d: Duration = %lld ns\n", ++count, duration_ns);
        list_del(&data->list);
        kfree(data);
    }

    printk(KERN_INFO "hello1: Module unloaded. Total %d entries processed.\n", count);
}

module_init(hello1_init);
module_exit(hello1_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Darina");
MODULE_DESCRIPTION("Base module for timing and kmalloc error simulation");
