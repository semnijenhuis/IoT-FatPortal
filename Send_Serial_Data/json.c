#include <string.h>
#include <stdlib.h>

static const char* parameters[19] = {"PID", "FW", "SER#", "V", "I", "VPV", "PPV", "CS", "MPPT", "OR", "ERR", "LOAD", "IL", "H19", "H20", "H21", "H22", "H23", "HSDS"};

int getJsonSizeFromSerial(char* serialToConvert) {    
    int jsonSize = 2;

    for (int i = 0; i < sizeof(parameters)/sizeof(parameters[0]); i++) {
        char* to_string = strstr(serialToConvert, parameters[i]);
        char* end_string = strstr(to_string, "\n");

        size_t len = end_string - to_string;

        len -= (strlen(parameters[i]) + 2);

        jsonSize += 10;
        jsonSize += strlen(parameters[i]);
        jsonSize += len;

        if (i + 1 < sizeof(parameters)/sizeof(parameters[0])) {
            jsonSize += 1;
        }
    }

    return jsonSize;
}

int convertJsonToSerial(char* saveVariable, char* serialToConvert) {
  for (int i = 0; i < sizeof(parameters)/sizeof(parameters[0]); i++) {
    char* to_string = strstr(serialToConvert, parameters[i]);
    char* end_string = strstr(to_string, "\n");

    size_t len = end_string - to_string;

    len -= (strlen(parameters[i]) + 2);

    char* value = malloc(len + 1);

    memcpy(value, to_string + (strlen(parameters[i]) + 1), len);
    value[len] = 0;

    strcat(saveVariable, "\\\"");
    strcat(saveVariable, parameters[i]);
    strcat(saveVariable, "\\\": \\\"");
    strcat(saveVariable, value);
    strcat(saveVariable, "\\\"");

    if (i + 1 < sizeof(parameters)/sizeof(parameters[0])) {
      strcat(saveVariable, ",");
    }
  }

  return strlen(saveVariable);
}