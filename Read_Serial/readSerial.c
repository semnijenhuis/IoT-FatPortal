// C library headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Linux headers
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

int serial_port = -1;

int setupSerialPort() {
    // Open rs232 device driver
    serial_port = open("/dev/ttyUSB1", O_RDWR);

    // If error print it and return
    if (serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return -1;
    }

    // Create tty and get the state of the serial port in it
    struct termios tty;
    if(tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return -1;
    }

    // Wait for up to 2s (20 deciseconds), or when data is received
    tty.c_cc[VTIME] = 20;    
    // Wait until 180 bytes received
    tty.c_cc[VMIN] = 180;

    // Set in/out baud rate to be 19200, because that is what the MPPT sends
    cfsetispeed(&tty, B19200);
    cfsetospeed(&tty, B19200);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return -1;
    }
    return 0;
}

int readTwoSerialMessages(char* totalBuf, int messageSize) {
    // Check if serial port initialised
    if (serial_port < 0) {
        printf("Serial port not initialized\n");
        return -1;
    }

    // Check if the message isn't any larger than the 256 bytes max we can read from the serial port
    if (messageSize >= 256) {
        printf("Message is to large\n");
        return -1;
    }

    // Get two messages, because you normally won't start reading at te start of the MPPT message
    // Also a normal message can only be 256 bytes long
    for (int i = 0; i < 2; i++) {
        // A buffer where the serial output will be saved in
        char read_buf[messageSize];
        // Set every byte to 0, so their will be no interference with the message
        memset(&read_buf, '\0', sizeof(read_buf));
        // Read the message from the serial portand get the number of bytes read
        // The read time and number of bytes are entered in VTIME and VMIN at the setup of the serial port
        int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));
        // Check if the read whas succesful
        if (num_bytes == -1) {
            printf("Error reading: %s", strerror(errno));
            return -1;
        }
        // Copy the message into the buffer
        memcpy(totalBuf + (messageSize * i), read_buf, num_bytes);
    }

    return 0;
}

void closeSerialPort() {
    // Closes the device driver
    close(serial_port);
    // Set the serial port back to -1
    serial_port = -1;
}



