#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int sendPostRequest(char* dataToSend, const char* webServer, const char* ProductID) {
  // Start of the curl command that defines that a post will be made
  char* post = "curl -X POST ";
  // Comes after the URL and defines what will be send. Part of it will be de data which contains of a product id which will be defined after this string
  char* contentType = " -H 'Content-Type: application/json' --data '{\"ProductID\": \"";
  // Json to say that the data will come next
  char* data = "\", \"Data\": \"";
  // End of the data
  char* end = "\"}'";
  // Calculates the string length of everything combined
  int commandSize = strlen(post) + strlen(contentType) + strlen(data) + strlen(end) + strlen(dataToSend) + strlen(webServer) + strlen(ProductID);
  // Create a string with the size of the strings combined
  char* command = malloc(commandSize + 1);
  // Set the last byte to zero, to set the end of the string
  command[commandSize] = 0;
  
  // Add every part of the command to the just created string
  strcat(command, post);
  strcat(command, webServer);
  strcat(command, contentType);
  strcat(command, ProductID);
  strcat(command, data);
  strcat(command, dataToSend);
  strcat(command, end);

  // Run the command and return the state
  return system(command);
}