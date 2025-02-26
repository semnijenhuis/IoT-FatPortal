// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

int main() {
  printf("Hello, ARM!\n");

  int serial_port = open("/dev/ttyUSB1", O_RDWR);

  if (serial_port < 0) {
	  printf("Error %i from open: %s\n", errno, strerror(errno));
	  return 1;
  }

  struct termios tty;
  if(tcgetattr(serial_port, &tty) != 0) {
	  printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
	  return 1;
  }

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

  tty.c_cc[VTIME] = 20;    // Wait for up to 2s (20 deciseconds), returning as soon as any data is received.
  // Wait until 20 bytes received
  tty.c_cc[VMIN] = 200;

	// Set in/out baud rate to be 19200, because that is what the MPPT sends
  cfsetispeed(&tty, B19200);
  cfsetospeed(&tty, B19200);

  // Save tty settings, also checking for error
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
	  printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
	  return 1;
  }

  // Allocate memory for read buffer, set size according to your needs
  char read_buf [256];

  // Set every bit to zero to use printf more easily
  memset(&read_buf, '\0', sizeof(read_buf));

  // Read bytes entered in VTIME and VMIN
  int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

  // If num_bytes is -1 a error occured
  if (num_bytes == -1) {
    printf("Error reading: %s", strerror(errno));
    return 1;
  }

  // Print all data we received from the serial port
  printf("Read %i bytes. Received message: %s\n", num_bytes, read_buf);

  close(serial_port);

  return 0;
}
