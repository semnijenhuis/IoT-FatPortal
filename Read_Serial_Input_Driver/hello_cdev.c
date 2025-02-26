#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

#define BUFFER_SIZE 1024

static int major;
static char device_buffer[BUFFER_SIZE];

static ssize_t my_read(struct file *f, char __user *u, size_t l, loff_t *o);
static ssize_t my_write(struct file *f, const char __user *u, size_t l, loff_t *o);

static struct file_operations fops = {
    .read = my_read,
    .write = my_write
};

static ssize_t my_read(struct file *f, char __user *u, size_t l, loff_t *o) {
    int bytes_read = l < BUFFER_SIZE ? l : BUFFER_SIZE;
    if (copy_to_user(u, device_buffer, bytes_read)) {
        return -EFAULT;
    }
    printk("hello_cdev - Read is called\n");
    return bytes_read;
}

static ssize_t my_write(struct file *f, const char __user *u, size_t l, loff_t *o) {
    int bytes_to_write = l < BUFFER_SIZE ? l : BUFFER_SIZE;
    if (copy_from_user(device_buffer, u, bytes_to_write)) {
        return -EFAULT;
    }
    return bytes_to_write;
}

static int __init my_init(void) {
    major = register_chrdev(0, "hello_cdev", &fops);
    if (major < 0) {
        printk(" hello_cdev - Error registering chrdev\n");
        return major;
    }
    printk("hello_cdev - Major Device Number: %d\n", major);
    return 0;
}

static void __exit my_exit(void) {
    unregister_chrdev(major, "hello_cdev");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Oostrik");
MODULE_DESCRIPTION("A example Hello World Linux Kernel Module for a teltonika router");