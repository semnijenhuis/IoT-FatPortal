#define HOST_NAME_REG 7
#define ROUTER_SERIAL_NUMBER_REG 39
#define GPS_LAT_REG 143
#define GPS_LONG_REG 145
#define RX_LAST_24_HOURS_REG 197
#define TX_LAST_24_HOURS_REG 199

#define FLOAT_LENGTH 2
#define LONG_LENGTH 2
#define TEXT_LENGTH 16

int readModbus(char* registerName, short* value);
