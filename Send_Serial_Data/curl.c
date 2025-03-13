#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int sendPostRequest(char* dataToSend, const char* webServer, const char* ProductID) {
  char* post = "curl -X POST ";
  char* contentType = " -H 'Content-Type: application/json' --data '{\"ProductID\": \"";
  char* data = "\", \"Data\": \"";
  char* end = "\"}'";
  int commandSize = strlen(post) + strlen(contentType) + strlen(data) + strlen(end) + strlen(dataToSend) + strlen(webServer) + strlen(ProductID);
  char* command = malloc(commandSize + 1);
  command[commandSize] = 0;
  
  strcat(command, post);
  strcat(command, webServer);
  strcat(command, contentType);
  strcat(command, ProductID);
  strcat(command, data);
  strcat(command, dataToSend);
  strcat(command, end);
  int error = 0;

  error = system(command);
  // TODO Check for error
  return error;
}