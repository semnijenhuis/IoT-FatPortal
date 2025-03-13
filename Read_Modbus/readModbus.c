// C library headers
#include <stdio.h>
#include <string.h>
#include <modbus.h>

// Linux headers
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#include "readModbus.h"

int getRegisterLocation(char* registerName);
int getRegisterSize(char* registerName);

int readModbus(char* registerName, short* value) {
  int registerLocation = getRegisterLocation(registerName);

  if (registerLocation == -1) {
    printf("Invalid string: %s has no register value declared\n", registerName);
    return -1;
  }

  int registerSize = getRegisterSize(registerName);
  
  modbus_t *ctx;
  uint16_t tab_reg[registerSize];
  
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
  
  int num_bytes = modbus_read_registers(ctx, registerLocation, registerSize, tab_reg);
  
  if (num_bytes == -1) {
    fprintf(stderr, "%s\n", modbus_strerror(errno));
  }

  for (int i = 0; i < registerSize; i++) {
    value[i] = tab_reg[i];
  }
  
  modbus_close(ctx);
  modbus_free(ctx);

  return 0;
}

int getRegisterLocation(char* registerName) {
  if (strcmp(registerName, "latitude") == 0) {
    return GPS_LAT_REG;
  } else if (strcmp(registerName, "longitude") == 0) {
    return GPS_LONG_REG;
  } else if (strcmp(registerName, "hostname") == 0) {
    return HOST_NAME_REG;
  } else if (strcmp(registerName, "router serial") == 0) {
    return ROUTER_SERIAL_NUMBER_REG;
  } else if (strcmp(registerName, "rx last 24h") == 0) {
    return RX_LAST_24_HOURS_REG;
  } else if (strcmp(registerName, "tx last 24h") == 0) {
    return TX_LAST_24_HOURS_REG;
  } else {
    return -1;
  }
}

int getRegisterSize(char* registerName) {
  if (strcmp(registerName, "latitude") == 0 || strcmp(registerName, "longitude") == 0) {
    return FLOAT_LENGTH;
  } else if (strcmp(registerName, "hostname") == 0 || strcmp(registerName, "router serial") == 0) {
    return TEXT_LENGTH;
  } else if (strstr(registerName, "rx ") != NULL || strstr(registerName, "tx ") != NULL) {
    return LONG_LENGTH;
  } else {
    return -1;
  }
}
