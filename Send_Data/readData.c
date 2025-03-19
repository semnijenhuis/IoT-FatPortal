// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "readData.h"

// Read all data form the driver called device, and save it in the data variable
int getDataFromDriver(char* data, const char* device) {
    char serialData[200];
    // Set every byte to zero so no byte has different data
    memset(serialData, '\0', sizeof(serialData));

    // Open the device driver
    int charDev = open(device, O_RDWR);
    if (charDev < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return -1;
    }

    // Read the first 200 bytes of data from the device driver
    int num_bytes = read(charDev, serialData, sizeof(serialData));
    if (num_bytes == -1) {
        printf("Error reading: %s", strerror(errno));
        return -1;
    }

    memcpy(data, serialData, num_bytes);
    return 0;
}