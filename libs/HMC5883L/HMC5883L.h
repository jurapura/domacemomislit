/* Library for basic abstraction of features implemented in HMC5883L magnetometer via Arduino
I2C communication */

#include "Arduino.h"
#include "I2Clib.h"

/** ALL REAGISTERS CAN BE READ, BUT ONLY CRA, CRB AND MR CAN BE WRITTEN TO. */

#define HMC5883L_ADD 0x1E // 7 bit address of the HMC5883L

/** <CRA7>  must be 0 
    <CRA6>  Select number of samples averaged (1 to 8) per measurement output.
    <CRA5>  00 = 1(Default); 01 = 2; 10 = 4; 11 = 8
    <CRA4>  These bits set the rate at which data is written to data output registers.  
    <CRA3>  <b000> 0.75Hz, <b001> 1.5Hz, <b010> 3Hz, <b011> 7.5Hz, <b100> 15 (Default)
    <CRA2>  <b101> 30Hz, <b110> 75Hz, <b111> Reserved
    <CRA1>  These bits define the measurement flow of the device, specifically whether or not
    <CRA0>  to incorporate an applied bias into the measurement.
            <b00>   Normal measurement configuration (Default). In normal measurement
                    configuration the device follows normal measurement flow. The positive and
                    negative pins of the resistive load are left floating and high impedance.
            <b01>   Positive bias configuration for X, Y, and Z axes. In this configuration, 
                    a positive current is forced across the resistive load for all three axes.
            <b10>   Negative bias configuration for X, Y and Z axes. In this configuration, 
                    a negative current is forced across the resistive load for all three axes.
            <b11>   This configuration is reserved.
 */
#define HMC5883L_CONF_REG_A 00 // Default is <0x10>

/** <CRB7>  Gain Configuration Bits. These bits configure the gain for
    <CRB6>  the device. The gain configuration is common for all channels.
    <CRB5>  REGs   Range     Gain
            <b000> ± 0.88 Ga 1370
            <b001> ± 1.3  Ga 1090 (default)
            <b010> ± 1.9  Ga 820 
            <b011> ± 2.5  Ga 660 
            <b100> ± 4.0  Ga 440
            <b101> ± 4.7  Ga 390
            <b110> ± 5.6  Ga 330
            <b111> ± 8.1  Ga 230
    <CRB4> Must be 0
    <CRB3> Must be 0
    <CRB2> Must be 0
    <CRB1> Must be 0
    <CRB0> Must be 0
 */
#define HMC5883L_CONF_REG_B 01

/** <MR7> Set this pin to enable High Speed I2C, 3400kHz.
    <MR6> Must be 0
    <MR5> Must be 0
    <MR4> Must be 0
    <MR3> Must be 0
    <MR2> Must be 0
    <MR1> Mode Select Bits. These bits select the operation mode of this device.
    <MR0> <b00> Continuous-Measurement Mode.
          <b01> Single-Measurement Mode. When single-measurement
                mode is selected, device performs a single measurement, sets RDY
                high and returned to idle mode. Mode register returns to idle mode
                bit values. The measurement remains in the data output register and
                RDY remains high until the data output register is read or another
                measurement is performed. 
          <b10> Idle mode.
          <b11> Idle mode. 
 */
#define HMC5883L_MODE_REG 02 // Default mode is Single-Measurement Mode.

/** 16-bit 2's complement form. 
    Range is 0xF800 to 0x07FF
    In case of overflow or underflow, register will contain value -4096.
 */
#define HMC5883L_DATA_X_MSB 03
#define HMC5883L_DATA_X_LSB 04
#define HMC5883L_DATA_Z_MSB 05
#define HMC5883L_DATA_Z_LSB 06
#define HMC5883L_DATA_Y_MSB 07
#define HMC5883L_DATA_Y_LSB 08

/** <SR7> Must be 0.
    <SR6> Must be 0.
    <SR5> Must be 0.
    <SR4> Must be 0.
    <SR3> Must be 0.
    <SR2> Must be 0.
    <SR1> Data output register lock. 
        -Some, but not all OutReg whave been read.
        -MR has been read.
        This bit will not clear until:
        1.all six bytes have been read
        2.the mode register is changed
        3.the measurement configuration (CRA) is changed
        4.power cycle
    <SR0> Ready Bit. 
            Set when data is written to all six data registers.
            Cleared when device initiates a write to the data output registers 
            and after one or more of the data output registers are written to.
 */
#define HMC5883L_STATUS_REG 09

/** <IRA> == <b01001000>
    <IRB> == <b00110100>
    <IRC> == <b00110011>
    Used to identify the device.
 */
#define HMC5883L_ID_REG_A 10
#define HMC5883L_ID_REG_B 11
#define HMC5883L_ID_REG_C 12

    uint8_t data[5];
    uint8_t temp;
