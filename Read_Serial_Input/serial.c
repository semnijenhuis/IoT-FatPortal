// C library headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Linux headers
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

char* getOneMessage(char* total_string);
int setInCharDevice(char* total_string, int length);

int main() {
  while (1) {
    // Open rs232 device driver
    int serial_port = open("/dev/ttyUSB1", O_RDWR);

    // If error print it and return
    if (serial_port < 0) {
      printf("Error %i from open: %s\n", errno, strerror(errno));
      return 1;
    }

    // Create tty and get the state of the serial port in it
    struct termios tty;
    if(tcgetattr(serial_port, &tty) != 0) {
      printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
      return 1;
    }

    // Change multiple flags wich will be saved later
    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

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
      return 1;
    }

    // There are 2 buffers which will be read, because you can only read a buffer with 256 bytes as maximum in one go
    // Allocate memory for read buffer, set size according to your needs
    char read_buf1 [180];
    char read_buf2 [180];
    char total_buf [360];

    // Set every bit to zero to use printf more easily
    memset(&read_buf1, '\0', sizeof(read_buf1));
    memset(&read_buf2, '\0', sizeof(read_buf2));

    // Read time and number of bytes entered in VTIME and VMIN
    int num_bytes1 = read(serial_port, &read_buf1, sizeof(read_buf1));
    int num_bytes2 = read(serial_port, &read_buf2, sizeof(read_buf2));

    // If num_bytes is -1 a error occured
    if (num_bytes1 == -1) {
      printf("Error reading: %s", strerror(errno));
      return 1;
    }
    if (num_bytes2 == -1) {
      printf("Error reading: %s", strerror(errno));
      return 1;
    }

    // Copy both outputs in 1 array
    memcpy(total_buf, read_buf1, 180);
    memcpy(total_buf + 180, read_buf2, 180);

    // Closes the device driver
    close(serial_port);

    // Get one MPPT message out of the total buffer
    char* messageString = getOneMessage(total_buf);

    // Set the MPPT message into the device driver
    int done = setInCharDevice(messageString, strlen(messageString));
    
    // Check if message succesfully saved
    if (done == -1) {
      printf("An error occured");
      return 1;
    } 
  }

  return 0;
}

// This function will set the start of the message out of the string en will return the length of the message
char* getOneMessage(char* total_string) {
  // Get the place of the start of a MPPT message
  char* to_string = strstr(total_string, "PID");
  // Get the place of the start of the next MPPT message
  char* end_string = strstr(to_string, "\nPID");
  
  // Get the length between the messages
  size_t len = end_string - to_string;

  // Set the end of the string after the length of the string
  to_string[len+1] = 0;

  // Return the char pointer to the start of the message
  return to_string;
}

int setInCharDevice(char* total_string, int length) {
  // Open the device driver by place, and what you can do with it
  int char_dev = open("/dev/serialDriver", O_RDWR);
  // Check if driver found
  if (char_dev < 0) {
	  printf("Error %i from open: %s\n", errno, strerror(errno));
	  return -1;
  }

  // Write the message to the device driver
  int num_bytes = write(char_dev, total_string, length);
  // Check if write was succesfull
  if (num_bytes == -1) {
    printf("Error reading: %s", strerror(errno));
    return -1;
  }

  // Close the device driver
  close(char_dev);
}
