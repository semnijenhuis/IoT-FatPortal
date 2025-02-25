// C library headers
#include <stdio.h>
#include <string.h>
#include <modbus.h>

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
  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
  // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

  tty.c_cc[VTIME] = 10;    // Wait for up to 2s (20 deciseconds), returning as soon as any data is received.
  tty.c_cc[VMIN] = 200;

	  // Set in/out baud rate to be 19200
  cfsetispeed(&tty, B19200);
  cfsetospeed(&tty, B19200);

  // Save tty settings, also checking for error
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
	  printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
	  return 1;
  }

  // Allocate memory for read buffer, set size according to your needs
  char read_buf [256];

    // Normally you wouldn't do this memset() call, but since we will just receive
    // ASCII data for this example, we'll set everything to 0 so we can
    // call printf() easily.
    memset(&read_buf, '\0', sizeof(read_buf));

    // Read bytes. The behaviour of read() (e.g. does it block?,
    // how long does it block for?) depends on the configuration
    // settings above, specifically VMIN and VTIME
    int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

    // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
    if (num_bytes < 0) {
	    printf("Error reading: %s", strerror(errno));
	    return 1;
    }

    // Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and
    // print it to the screen like this!)
    printf("Read %i bytes. Received message: %s\n", num_bytes, read_buf);

  close(serial_port);
  
  modbus_t *ctx;
  uint16_t tab_reg[32];
  
  memset(&tab_reg, '\0', sizeof(tab_reg));
  
  ctx = modbus_new_tcp("192.168.1.1", 502);
  if (ctx == NULL) {
    printf("Unable to allocate libmodbus context\n");
    return 1;
  } 
  
  if (modbus_set_slave(ctx, 1) == -1) {
    fprintf(stderr, "Invalid slave ID\n");
    modbus_free(ctx);
    return -1;
  }
  
  if (modbus_connect(ctx) == -1) {
    printf("Connection failed: %s\n", modbus_strerror(errno));
    modbus_free(ctx);
    return -1;
  }
  
  num_bytes = modbus_read_registers(ctx, 143, 2, tab_reg);
  
  if (num_bytes == -1) {
    fprintf(stderr, "%s\n", modbus_strerror(errno));
  }
  
  printf("Read %i bytes. Received message: %d, %d\n", num_bytes, tab_reg[0], tab_reg[1]);
  
  modbus_close(ctx);
  modbus_free(ctx);

  return 0;
}
