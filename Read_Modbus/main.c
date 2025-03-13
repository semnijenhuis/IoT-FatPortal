#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Linux headers
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#include "readModbus.h"

static const char* WEBSERVER = "https://test.fatportal.nl/api/Ping";
static const char* PRODUCTID = "bdfc827b275d4fd0a7ee3c9743379728";

union ConvertModbus {
    int i;
    float f;
    unsigned long l;
    short s[16];
    char c[33];
};

int setInCharDevice(char* total_string, int length);

int main() {
    char saveVariable[200];
    memset(&saveVariable, '\0', sizeof(saveVariable));
    union ConvertModbus converter;
    readModbus("latitude", converter.s);
    strcat(saveVariable, "\\\"latitude\\\":\\\"");
    char buffer[9];
    sprintf(buffer, "%.6f", converter.f);
    strcat(saveVariable, buffer);
    readModbus("longitude", converter.s);
    strcat(saveVariable, "\\\",\\\"longitude\\\":\\\"");
    sprintf(buffer, "%.6f", converter.f);
    strcat(saveVariable, buffer);
    readModbus("hostname", converter.s);
    strcat(saveVariable, "\\\",\\\"hostname\\\":\\\"");
    strcat(saveVariable, converter.c);
    readModbus("router serial", converter.s);
    strcat(saveVariable, "\\\",\\\"serialNumber\\\":\\\"");
    strcat(saveVariable, converter.c);
    readModbus("rx last 24h", converter.s);
    strcat(saveVariable, "\\\",\\\"rx\\\":\\\"");
    buffer[9];
    sprintf(buffer, "%lu", converter.l);
    strcat(saveVariable, buffer);
    readModbus("tx last 24h", converter.s);
    strcat(saveVariable, "\\\",\\\"tx\\\":\\\"");
    buffer[9];
    sprintf(buffer, "%lu", converter.l);
    strcat(saveVariable, buffer);
    strcat(saveVariable, "\\\"");
    setInCharDevice(saveVariable, strlen(saveVariable));
    return 0;
}

int setInCharDevice(char* total_string, int length) {
    int char_dev = open("/dev/routerDriver", O_RDWR);
    if (char_dev < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return -1;
    }

    int num_bytes = write(char_dev, total_string, length);
    if (num_bytes == -1) {
        printf("Error reading: %s", strerror(errno));
        return -1;
    }

    close(char_dev);
}
  