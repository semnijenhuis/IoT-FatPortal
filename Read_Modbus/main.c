#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "readModbus.h"
#include "saveData.h"

const char* VERSION = "v0.0.1";

void printArguments(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    // Print arguments such as help and versions
    printArguments(argc, argv); 
    // Create a variable of size 200 (Which is enough to save the current string in) and set every byte as 0
    char saveVariable[250];
    memset(&saveVariable, '\0', sizeof(saveVariable));
    // Create a union which can convert the shorts given by the modbus into 
    union ConvertModbus converter;
    // Read the latitude from the modbus
    readModbus("latitude", converter.s);
    // Create a buffer where to save the value temporarily in
    char buffer[35];
    // Create a string to add to the json of the latitude, and add it to the save variable
    sprintf(buffer, "\\\"latitude\\\":\\\"%.6f", converter.f);
    strcat(saveVariable, buffer);
    // Read the longitude from the modbus
    readModbus("longitude", converter.s);
    // Overwrite a string to add to the json of the longitude, and add it to the save variable
    sprintf(buffer, "\\\",\\\"longitude\\\":\\\"%.6f", converter.f);
    strcat(saveVariable, buffer);
    // Read the hostname from the modbus
    readModbus("hostname", converter.s);
    // Add the json part and hostname to the save variable
    strcat(saveVariable, "\\\",\\\"hostname\\\":\\\"");
    strcat(saveVariable, converter.c);
    // Read the hostname from the modbus
    readModbus("router serial", converter.s);
    // Add the json part and serial number to the save variable
    strcat(saveVariable, "\\\",\\\"serialNumber\\\":\\\"");
    strcat(saveVariable, converter.c);
    // Read the hostname from the modbus
    readModbus("tx last 24h", converter.s);
    // Overwrite a string to add to the json of the tx of the last 24h, and add it to the save variable
    sprintf(buffer, "\\\",\\\"tx\\\":\\\"%lu", converter.ul);
    strcat(saveVariable, buffer);
    // Read the hostname from the modbus
    readModbus("rx last 24h", converter.s);
    // Overwrite a string to add to the json of the rx of the last 24h, and add it to the save variable
    sprintf(buffer, "\\\",\\\"rx\\\":\\\"%lu", converter.ul);
    strcat(saveVariable, buffer);
    // Read the hostname from the modbus
    readModbus("temp", converter.s);
    // Overwrite a string to add to the json of the temperature, and add it to the save variable
    sprintf(buffer, "\\\",\\\"temp\\\":\\\"%ld\\\"", converter.sl);
    strcat(saveVariable, buffer);
    // Set the created json in the character device
    setInCharDevice(saveVariable, strlen(saveVariable));

    //Stop the program
    return 0;
}

void printArguments(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
            printf("%s\n", VERSION);
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printf("Usage: readModbus [OPTION]\n");
            printf("Read data from the routers Modbus, and saves the data in a character device driver\n\n");
            printf("    -v, --version\t Get the version of the program\n");
            printf("    -h, --help\t\t Get info for program arguments\n");
        } else {
            printf("readModbus: invalid option %s\n", argv[i]);
            printf("Try 'readModbus --help' for more information.\n");
        }
    }
}
