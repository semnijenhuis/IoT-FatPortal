#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Create a array of character arrays to save the parameters as character arrays (strings), make sure the number of parameters is defined between the array brackets
// The order is not important, but it is important that every parameter exists
static const char* parameters[19] = {"PID", "FW", "V", "SER#", "I", "VPV", "PPV", "CS", "MPPT", "OR", "ERR", "LOAD", "IL", "H19", "H20", "H21", "H22", "H23", "HSDS"};

// Get the size of the serial as a json
int getJsonSizeFromSerial(char* serialToConvert) {   
  // Add a standard of to for the curly brackets of the json
  int jsonSize = 2;

  // For every parameter
  for (int i = 0; i < sizeof(parameters)/sizeof(parameters[0]); i++) {
    // Add a tab to the parameter to look for the specific parameter, and not part of the parameter
    char* parameterWithTab = malloc(strlen(parameters[i]) + 2);
    memcpy(parameterWithTab, parameters[i], strlen(parameters[i]));
    parameterWithTab[strlen(parameters[i])] = '\t';
    parameterWithTab[strlen(parameters[i]) + 1] = 0;
    // Get the start of the parameter value
    char* to_string = strstr(serialToConvert, parameterWithTab);
    // Get the end of the parameter value
    char* end_string = strstr(to_string, "\n");
    // The length of the value is the end of string - the start of the string
    size_t len = end_string - to_string;
    // Remove the tab and endline from the length of the parameter and value
    len -= 2;
    // Add the length of the value, parameter, and quotationmarks
    jsonSize += 10;
    jsonSize += len;
    // If their is a next parameter add one for the comma
    if (i + 1 < sizeof(parameters)/sizeof(parameters[0])) {
      jsonSize += 1;
    }
  }

  return jsonSize;
}

// Add serial data to a json string which is also given
int addSerialToJson(char* jsonValue, char* serialToConvert) {
  // For every parameter
  for (int i = 0; i < sizeof(parameters)/sizeof(parameters[0]); i++) {
    // Add a tab to the parameter to look for the specific parameter, and not part of the parameter
    char* parameterWithTab = malloc(strlen(parameters[i]) + 2);
    memcpy(parameterWithTab, parameters[i], strlen(parameters[i]));
    parameterWithTab[strlen(parameters[i])] = '\t';
    parameterWithTab[strlen(parameters[i]) + 1] = 0;
    // Get the start of the parameter
    char* to_string = strstr(serialToConvert, parameterWithTab);
    // Get the end of the value
    char* end_string = strstr(to_string, "\n");

    // Get the length parmeter tab and value
    size_t len = end_string - to_string;

    // Get the length of the value
    len -= (strlen(parameters[i]) + 2);

    // Create a string with the length of the value with a place for the end of the string
    char* value = malloc(len + 1);

    // Set the value of the parameter, and the end of the string
    memcpy(value, to_string + (strlen(parameters[i]) + 1), len);
    value[len] = 0;

    // Add everything into the json variable
    strcat(jsonValue, "\\\"");
    strcat(jsonValue, parameters[i]);
    strcat(jsonValue, "\\\": \\\"");
    strcat(jsonValue, value);
    strcat(jsonValue, "\\\"");

    // If there is a next variable, add a comma
    if (i + 1 < sizeof(parameters)/sizeof(parameters[0])) {
      strcat(jsonValue, ",");
    }
  }

  // Return the length
  return strlen(jsonValue);
}