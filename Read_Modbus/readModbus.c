//  for printinnnnng errors
#include <stdio.h>
#include <string.h>

// Linux headers for talking with a driver
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

// Includes to write to the modbus
#include <modbus.h>
#include "readModbus.h"

int getRegisterLocation(char* registerName);
int getRegisterSize(char* registerName);

int readModbus(char* registerName, short* value) {
  // Get the register location in the modbus
  int registerLocation = getRegisterLocation(registerName);

  // If register not found give error and return
  if (registerLocation == -1) {
    printf("Invalid string: %s has no register value declared\n", registerName);
    return -1;
  }

  // Get the size of the modbus register
  int registerSize = getRegisterSize(registerName);
  
  // Create a ctx to start communication with the modbus
  modbus_t *ctx;
  
  // Set every short value to 0
  memset(&value, '\0', sizeof(value));
  
  // Setup modbus location
  ctx = modbus_new_tcp("192.168.1.1", 502);
  if (ctx == NULL) {
    printf("Unable to allocate libmodbus context\n");
    return 1;
  } 
  
  // Set modbus slave with device id 1
  if (modbus_set_slave(ctx, 1) == -1) {
    fprintf(stderr, "Invalid slave ID\n");
    modbus_free(ctx);
    return -1;
  }
  
  // Connect to modbus
  if (modbus_connect(ctx) == -1) {
    printf("Connection failed: %s\n", modbus_strerror(errno));
    modbus_free(ctx);
    return -1;
  }
  
  // Read from the modbus
  int num_bytes = modbus_read_registers(ctx, registerLocation, registerSize, value);
  
  // Check if reading is complete
  if (num_bytes == -1) {
    fprintf(stderr, "%s\n", modbus_strerror(errno));
  }
  
  // Close and set the modbus free
  modbus_close(ctx);
  modbus_free(ctx);

  return 0;
}

// A function which returns the position of the modbus register of that type
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
  } else if (strcmp(registerName, "temp") == 0) {
    return TX_LAST_24_HOURS_REG;
  } else {
    return -1;
  }
}

// A function which returns the length of the modbus register based on the type of information
int getRegisterSize(char* registerName) {
  if (strcmp(registerName, "latitude") == 0 || strcmp(registerName, "longitude") == 0) {
    return FLOAT_LENGTH;
  } else if (strcmp(registerName, "hostname") == 0 || strcmp(registerName, "router serial") == 0) {
    return TEXT_LENGTH;
  } else if (strstr(registerName, "rx ") != NULL || strstr(registerName, "tx ") != NULL || strcmp(registerName, "temp") == 0) {
    return LONG_LENGTH;
  } else {
    return -1;
  }
}
