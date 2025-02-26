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
  
  int num_bytes = modbus_read_registers(ctx, 143, 2, tab_reg);
  
  if (num_bytes == -1) {
    fprintf(stderr, "%s\n", modbus_strerror(errno));
  }
  
  printf("Read %i bytes. Received message: %d, %d\n", num_bytes, tab_reg[0], tab_reg[1]);
  
  modbus_close(ctx);
  modbus_free(ctx);

  return 0;
}
