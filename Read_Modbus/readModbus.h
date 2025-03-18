// Defenitions for the registers of the modbus
#define DEVICE_TEMP_REG 5
#define HOST_NAME_REG 7
#define ROUTER_SERIAL_NUMBER_REG 39
#define GPS_LAT_REG 143
#define GPS_LONG_REG 145
#define RX_LAST_24_HOURS_REG 197
#define TX_LAST_24_HOURS_REG 199

// Defenitions of the amount of shorts returned for each variable type
#define FLOAT_LENGTH 2
#define LONG_LENGTH 2
#define TEXT_LENGTH 16

// Union to convert the shorts given by the modbus to usefull values
union ConvertModbus {
    int i;
    float f;
    unsigned long ul;
    long sl;
    short s[16];
    char c[33];
};

// Function to read a specific part of the modbus
int readModbus(char* registerName, short* value);
