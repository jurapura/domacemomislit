//################################
//###     HMC5883L defines     ###
//################################

#define HMC5883L 0x1E
#define HMC5883L_W 0x3C // Writing
#define HMC5883L_R 0x3D // Reading
#define CONFIG_A 0x00 // Configuration register A
#define CONFIG_B 0x01 // Configuration register B
#define MODE_REG 0x02 // Mode configuration register
#define DATA_OUTPUT_X_H 0x03 // MSB X Data register
#define DATA_OUTPUT_X_L 0x04 // LSB X Data register
#define DATA_OUTPUT_Z_H 0x05 // MSB Z Data register
#define DATA_OUTPUT_Z_L 0x06 // LSB Z Data register
#define DATA_OUTPUT_Y_H 0x07 // MSB Y Data register
#define DATA_OUTPUT_y_L 0x08 // LSB Y Data register
#define STATUS 0x09 // Status register
#define ID_A 0x10 // Id register A
#define ID_B 0x11 // Id register B
#define ID_C 0x12 // Id register C
