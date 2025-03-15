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

// Endpoint of the fatportal
static const char* WEBSERVER = "https://test.fatportal.nl/api/Ping";
// ProductID to save the information to
static const char* PRODUCTID = "acbf264b076c43c9aeb9a1d6d45a32ae";

int main() {
  // Create a variable to save the serial information in
  char serialData[200];
  // Set every byte to zero so no byte has different data
  memset(&serialData, '\0', sizeof(serialData));

  // Open the serial device driver
  int serialCharDev = open("/dev/serialDriver", O_RDWR);
  if (serialCharDev < 0) {
	  printf("Error %i from open: %s\n", errno, strerror(errno));
	  return -1;
  }

  // Read the first 200 bytes of data from the device driver 
  int num_bytes = read(serialCharDev, serialData, sizeof(serialData));
  if (num_bytes == -1) {
    printf("Error reading: %s", strerror(errno));
    return -1;
  }

  // Close the device driver
  close(serialCharDev);

  // Create a variable to save the router data in
  char routerData[200];
  // Set every byte to zero so no byte has different data
  memset(&routerData, '\0', sizeof(routerData));

  // Open the router device driver
  int routerCharDev = open("/dev/routerDriver", O_RDWR);
  if (routerCharDev < 0) {
	  printf("Error %i from open: %s\n", errno, strerror(errno));
	  return -1;
  }

  // Read the first 200 bytes of data from the device driver
  num_bytes = read(routerCharDev, routerData, sizeof(routerData));
  if (num_bytes == -1) {
    printf("Error reading: %s", strerror(errno));
    return -1;
  }

  // Close the device driver
  close(routerCharDev);

  // Get the json size if the serial data was converted to json
  int jsonSize = getJsonSizeFromSerial(serialData);

  // Create a variable to save al the json values in, plus a place for the last 0 and a comma
  char* json_string = malloc(jsonSize + strlen(routerData) + 2);
  // Set every byte to zero so no byte has different data
  memset(json_string, '\0', sizeof(json_string));

  // Set the start of the json
  strcat(json_string, "{");
  
  // Add the serial part as json to the json string
  int finalSize = addSerialToJson(json_string, serialData);

  // Add comma, beacause more JSON is comming
  strcat(json_string, ",");

  // Add the router data which is saved as JSON
  strcat(json_string, routerData);

  // Set the end of the json
  strcat(json_string, "}");

  // Add the router data length to the json size, and comma which seperate these 2 
  jsonSize += (1 + strlen(routerData));

  // Check if json is same size as calculated
  if (jsonSize != strlen(json_string)) {
    printf("Sizes are not equal");
    return -1;
  }

  // Send the json string to the fatportal, with the specified productid
  sendPostRequest(json_string, WEBSERVER, PRODUCTID);

  return 0;
}
