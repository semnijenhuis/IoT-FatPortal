#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int sendPostRequest(char* dataToSend, const char* webServer, const char* ProductID) {
  char command[250] = "curl -X POST ";
  strcat(command, webServer);
  strcat(command,  " -H 'Content-Type: application/json' --data '{\"ProductID\": \"");
  strcat(command, ProductID);
  strcat(command, "\", \"Data\": \"");
  strcat(command, dataToSend);
  strcat(command, "\"}'");
  int error = 0;
  
  error = system(command);
  return error;
}