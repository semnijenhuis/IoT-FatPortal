// Includes needed for creating a kernel driver
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

// Set the buffer size where data can be saved
#define BUFFER_SIZE 1024

// Definition of the mayor number, this is usally available on a Teltonika TRB245 router
#define MYMAJOR 250

// Variables to save the major number of the device driver and the data
static int major;
static char device_buffer[BUFFER_SIZE];

// Say that these function are declared in somewhere
static ssize_t my_read(struct file *f, char __user *u, size_t l, loff_t *o);
static ssize_t my_write(struct file *f, const char __user *u, size_t l, loff_t *o);

// Specify where to find the read and write function
static struct file_operations fops = {
    .read = my_read,
    .write = my_write
};

// A read function to read data from the driver
static ssize_t my_read(struct file *f, char __user *u, size_t l, loff_t *o) {
    // Get how many bytes the user wants to read with the maximum the buffer size
    int bytes_read = l < BUFFER_SIZE ? l : BUFFER_SIZE;
    // Copy the data from the user space to the buffer
    if (copy_to_user(u, device_buffer, bytes_read)) {
        return -EFAULT;
    }
    // Return how many bytes are read by the user space
    return bytes_read;
}

// A write function to write data to the driver
static ssize_t my_write(struct file *f, const char __user *u, size_t l, loff_t *o) {
    // Get the amount of bytes the user wants to write, with the maximum the buffer size
    int bytes_to_write = l < BUFFER_SIZE ? l : BUFFER_SIZE;
    // Decleration of the int variable, because this yet can't be done in a for loop with the specified c compiler
    int i;
    // Set every byte to zero, so the last data will be deleted
    for (i = 0; i < BUFFER_SIZE; i++) {
        device_buffer[i] = '\0';
    }
    // Copy the data from the user space to the buffer
    if (copy_from_user(device_buffer, u, bytes_to_write)) {
        return -EFAULT;
    }
    // Return the number of bytes written
    return bytes_to_write;
}

// A function run when device driver is loaded
static int __init my_init(void) {
    // Set the major device number, functions and name
    major = register_chrdev(MYMAJOR, "routerDriver", &fops);
    if (major < 0) {
        printk(" routerDriver - Error registering chrdev\n");
        return major;
    }
    // Print message when succeeded
    printk("Started routerDriver on device number: %d\n", MYMAJOR);
    return 0;
}

// A function run when device driver is removed
static void __exit my_exit(void) {
    // Unregister character device
    unregister_chrdev(MYMAJOR, "routerDriver");
}

// Set the init end exit so the kernel knows how to start end end the functions
module_init(my_init);
module_exit(my_exit);

// Information about the creation and usage of the driver
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Oostrik");
MODULE_DESCRIPTION("A driver which can save up to 1024 bytes of data for the router data");