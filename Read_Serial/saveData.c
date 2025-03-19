// Headers to print to the console`
#include <stdio.h>
#include <string.h>

// Linux headers for drivers
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

int setInCharDevice(char* total_string, int length) {
    // Open the device driver to write the serial data to, and get the char dev number
    int char_dev = open("/dev/serialDriver", O_RDWR);
    // If char dev not available print a error
    if (char_dev < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return -1;
    }

    // Write the recieved data to the device driver 
    int num_bytes = write(char_dev, total_string, length);

    // Give error if write is failed
    if (num_bytes == -1) {
        printf("Error writing: %s", strerror(errno));
        return -1;
    }

    // close the device driver
    close(char_dev);
}
