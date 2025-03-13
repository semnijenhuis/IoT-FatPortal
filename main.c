// C library headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "curl.h"

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

  printf("Number of Bytes:%i\nText: %s\n", num_bytes, read_serial);
  printf("Char value = %s\n", read_serial);

  const char* test[19] = {"PID", "FW", "SER#", "V", "I", "VPV", "PPV", "CS", "MPPT", "OR", "ERR", "LOAD", "IL", "H19", "H20", "H21", "H22", "H23", "HSDS"};

  char json_string[500] = "{";

  for (int i = 0; i < sizeof(test)/sizeof(test[0]); i++) {
    char* to_string = strstr(read_serial, test[i]);
    char* end_string = strstr(to_string, "\n");

    size_t len = end_string - to_string;

    len -= (strlen(test[i]) + 2);

    char* value = malloc(len + 1);

    memcpy(value, to_string + (strlen(test[i]) + 1), len);
    value[len] = 0;

    strcat(json_string, "\\\"");
    strcat(json_string, test[i]);
    strcat(json_string, "\\\": \\\"");
    strcat(json_string, value);
    strcat(json_string, "\\\"");

    if (i + 1 < sizeof(test)/sizeof(test[0])) {
      strcat(json_string, ",");
    }
  }

  strcat(json_string, "}");

  sendPostRequest(json_string, WEBSERVER, PRODUCTID);

  return 0;
}
