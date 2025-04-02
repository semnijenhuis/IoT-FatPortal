// C library headers
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Linux headers
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#include "readSerial.h"
#include "saveData.h"

#define MESSAGE_SIZE 180

const char* VERSION = "v0.0.2";

char* getOneMessage(char* total_string);

void printArguments(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  printArguments(argc, argv);
  // Try to open the serial port
  if (setupSerialPort() < 0) {
      return 1;
  }

  // Create a buffer where 2 messages will fit in
  char totalBuf[MESSAGE_SIZE * 2];

  // Try to read the data and set in in the totalBuf
  if (readTwoSerialMessages(totalBuf, MESSAGE_SIZE) < 0) {
      return 1;
  }

  // Close the serial port
  closeSerialPort();

  // Get one MPPT message out of the total buffer, because the standard beginning of the read is almost never the start of the message
  char* oneMessage = getOneMessage(totalBuf);

  printf("%s\n", oneMessage);

  // Set the MPPT message into the device driver
  int done = setInCharDevice(oneMessage, strlen(oneMessage));
  
  // Check if message succesfully saved
  if (done == -1) {
    printf("An error occured");
    return 1;
  } 

  return 0;
}

// This function will set the start of the message out of the string en will return the length of the message
char* getOneMessage(char* total_string) {
  // Get the place of the start of a MPPT message
  char* to_string = strstr(total_string, "PID");
  // Get the place of the start of the next MPPT message
  char* end_string = strstr(to_string, "\nPID");
  
  // Get the length between the messages
  size_t len = end_string - to_string;

  // Set the end of the string after the length of the string
  to_string[len+1] = 0;

  // Return the char pointer to the start of the message
  return to_string;
}

void printArguments(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
          printf("%ss\n", VERSION);
      } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
          printf("Usage: readSerial [OPTION]\n");
          printf("Read the serial connection for a message and save it in a character device driver\n\n");
          printf("    -v, --version\t Get the version of the program\n");
          printf("    -h, --help\t\t Get info for program arguments\n");
      } else {
          printf("readSerial: invalid option %s\n", argv[i]);
          printf("Try 'readSerial --help' for more information.\n");
      }
  }
}