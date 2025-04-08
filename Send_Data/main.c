// C library headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Linux headers
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "curl.h"
#include "json.h"
#include "readData.h"

const char* VERSION = "v0.0.2";

// Endpoint of the fatportal
static const char* WEBSERVER = "https://test.fatportal.nl/api/Ping";
// ProductID to save the information to
static const char* PRODUCTID = "acbf264b076c43c9aeb9a1d6d45a32ae";
// Router device driver
static const char* ROUTER_DRIVER = "/dev/routerDriver";
// Serial device driver
static const char* SERIAL_DRIVER = "/dev/serialDriver";

int createJson(char* jsonString, char* serialData, char* routerData);

void printArguments(int argc, char *argv[], char* productId, char* url);

int main(int argc, char *argv[]) {
  char productId[64] = {'\0'};
  char url[100] = {'\0'};

  printArguments(argc, argv, productId, url);

  if (strlen(productId) == 0) strcpy(productId, PRODUCTID);
  if (strlen(url) == 0) strcpy(url, WEBSERVER);
  
  // Create a variable to save the serial information in
  char serialData[200];

  // Get the data from the serial driver
  if (getDataFromDriver(serialData, SERIAL_DRIVER) < 0) {
    return -1;
  }
  
  // Create a variable to save the router data in
  char routerData[200];
  
  // Get the data from the router driver
  if (getDataFromDriver(routerData, ROUTER_DRIVER) < 0) {
    return -1;
  }

  // Get the json size if the serial data was converted to json
  int jsonSize = getJsonSizeFromSerial(serialData);

  // Create a variable to save all the json values in, plus a place for the last 0 and a comma
  char* jsonString = malloc(jsonSize + strlen(routerData) + 2);

  // Set every byte to zero so no byte has different data
  memset(jsonString, '\0', sizeof(jsonString));

  // Create a full JSON from the serial en router data, where the serial data is transformt to JSON
  createJson(jsonString, serialData, routerData);

  // Add the router data length to the json size, and comma which seperate these 2 
  jsonSize += (1 + strlen(routerData));

  // Check if string is same size as calculated
  if (jsonSize != strlen(jsonString)) {
    printf("Sizes are not equal");
    return -1;
  }

  // Send the json string to the fatportal, with the specified productid
  sendPostRequest(jsonString, url, productId);

  return 0;
}

// Create a json from the serial data and router data, and returns the length of the json
int createJson(char* jsonString, char* serialData, char* routerData) {
  // Set the start of the json
  strcat(jsonString, "{");

  // Add the serial part as json to the json string
  int finalSize = addSerialToJson(jsonString, serialData);

  // Add comma, beacause more JSON is comming
  strcat(jsonString, ",");

  // Add the router data which is saved as JSON
  strcat(jsonString, routerData);

  // Set the end of the json
  strcat(jsonString, "}");
}

void printArguments(int argc, char *argv[], char* productId, char* url) {
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
      printf("%ss\n", VERSION);
    } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
      printf("Usage: sendData [OPTION]\n");
      printf("Send data saved in the character device drivers, this is data from the MPPT and the router\n\n");
      printf("    -v, --version\t Get the version of the program\n");
      printf("    -h, --help\t\t Get info for program arguments\n");
      printf("    -p, --productid\t\t Enter a productid of the product in the FATportal\n");
      printf("    -u, --url\t\t The specify the url of the FATportal\n");
    } else if (strcmp(argv[i], "--productid") == 0 || strcmp(argv[i], "-p") == 0) {
      if (i + 1 < argc && argv[i+1][0] != '-' && strlen(argv[i]) <= 64) {
        i++;
        strcpy(productId, argv[i]);
      } else {
        printf("Missing productid");
      }
    } else if (strcmp(argv[i], "--url") == 0 || strcmp(argv[i], "-u") == 0) {
      if (i + 1 < argc && argv[i+1][0] != '-' && strlen(argv[i]) <= 100) {
        i++;
        strcpy(url, argv[i]);
      } else {
        printf("Missing url");
      }
    } else {
      printf("sendData: invalid option %s\n", argv[i]);
      printf("Try 'sendData --help' for more information.\n");
    }
  }
}
