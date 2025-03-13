// C library headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "curl.h"
#include "json.h"

// Linux headers
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

static const char* WEBSERVER = "https://test.fatportal.nl/api/Ping";
static const char* PRODUCTID = "acbf264b076c43c9aeb9a1d6d45a32ae";

int main() {
  char read_serial[200];
  memset(&read_serial, '\0', sizeof(read_serial));

  int char_dev = open("/dev/HelloWorld", O_RDWR);
  if (char_dev < 0) {
	  printf("Error %i from open: %s\n", errno, strerror(errno));
	  return -1;
  }

  int num_bytes = read(char_dev, read_serial, sizeof(read_serial));
  if (num_bytes == -1) {
    printf("Error reading: %s", strerror(errno));
    return -1;
  }

  close(char_dev);

  char read_modbus[200];

  memset(&read_modbus, '\0', sizeof(read_modbus));

  char_dev = open("/dev/routerDriver", O_RDWR);
  if (char_dev < 0) {
	  printf("Error %i from open: %s\n", errno, strerror(errno));
	  return -1;
  }

  num_bytes = read(char_dev, read_modbus, sizeof(read_modbus));
  if (num_bytes == -1) {
    printf("Error reading: %s", strerror(errno));
    return -1;
  }

  close(char_dev);

  int jsonSize = getJsonSizeFromSerial(read_serial);

  char* json_string = malloc(jsonSize + strlen(read_modbus) + 2);
  memset(json_string, '\0', sizeof(json_string));

  strcat(json_string, "{");
  
  int finalSize = convertJsonToSerial(json_string, read_serial);

  strcat(json_string, ",");

  strcat(json_string, read_modbus);

  strcat(json_string, "}");

  jsonSize += (1 + strlen(read_modbus));

  printf("jsonSize: %i, finalSize: %i\n", jsonSize, finalSize);

  if (jsonSize != strlen(json_string)) {
    printf("Sizes are not equal");
    return -1;
  }

  sendPostRequest(json_string, WEBSERVER, PRODUCTID);

  return 0;
}
