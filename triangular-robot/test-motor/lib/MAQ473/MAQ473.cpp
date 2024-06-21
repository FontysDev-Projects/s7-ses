#include <Arduino.h>
#include <MAQ473.h>
#include <cmath>

// Wrapper constructors
MAQ473::MAQ473() : MAQ473(CLOCK_SPEED, MAQ473_CS, MAQ473_MOSI, MAQ473_MISO, MAQ473_SCK){

                   };

MAQ473::MAQ473(uint8_t pin_CS) : MAQ473(CLOCK_SPEED, pin_CS, MAQ473_MOSI, MAQ473_MISO, MAQ473_SCK){

                                 };

MAQ473::MAQ473(struct MAQ473Settings settings) : MAQ473(settings.clock, settings.pin_CS, settings.pin_MOSI, settings.pin_MISO, settings.pin_SCK){

                                                 };

MAQ473::MAQ473(uint8_t pin_CS, uint8_t pin_MOSI, uint8_t pin_MISO, uint8_t pin_SCK) : MAQ473(CLOCK_SPEED, pin_CS, pin_MOSI, pin_MISO, pin_SCK){

                                                                                      };

// Main constructor
MAQ473::MAQ473(uint32_t clock, uint8_t pin_CS, uint8_t pin_MOSI, uint8_t pin_MISO, uint8_t pin_SCK)
{
    _settings.clock = clock;
    _settings.pin_CS = pin_CS;
    _settings.pin_MOSI = pin_MOSI;
    _settings.pin_MISO = pin_MISO;
    _settings.pin_SCK = pin_SCK;
}

// Initialize
int MAQ473::begin()
{
    _spi = new SPIClass(_settings.pin_MOSI, _settings.pin_MISO, _settings.pin_SCK);
    if (_spi == nullptr)
    {
        return 1;
    }

    _spi->begin();
    return 0;
}

// TODO zero angle    DONE
// MAGH MAGL          DONE
// Magnetic field in range  DONE
// Set Filter Window  DONE

/*
    Reads the current angle of the magnet.

    @returns 16bit value containing the angle
*/
uint16_t MAQ473::readAngleRaw()
{
    _spi->beginTransaction(SPISettings(CLOCK_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(_settings.pin_CS, LOW);
    uint16_t receivedVal16 = _spi->transfer16(0x0000);
    digitalWrite(_settings.pin_CS, HIGH);
    _spi->endTransaction();

    return receivedVal16;
}

/*
    Transforms the 16bit angle value into a floating point value.

    @returns A float that is the degrees of the angle from 0.0 to 360.0
*/
float MAQ473::readAngle()
{
    return (float)readAngleRaw() / 0xFFFF * 360.0;
}

/*
    Writes to register.

    @param reg, 1st byte (e.g. 100xxxxxx) with first 3-bits set to 100 (being the write command)
           and the other 5-bits being the register address (e.g. 10011011)

    @param value, the 8-bit value that will be written to the register

    @returns newly written register value (acknowledgment)
*/
uint16_t MAQ473::writeRegister(uint16_t value)
{
    _spi->beginTransaction(SPISettings(CLOCK_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(_settings.pin_CS, LOW);
    uint16_t angle = _spi->transfer16(value);
    digitalWrite(_settings.pin_CS, HIGH);
    _spi->endTransaction();
    delay(20);
    _spi->beginTransaction(SPISettings(CLOCK_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(_settings.pin_CS, LOW);
    uint16_t registerVal16 = _spi->transfer16(0x0000);
    digitalWrite(_settings.pin_CS, HIGH);
    _spi->endTransaction();
    return registerVal16;
}

/*
    Reads register value.

    @param value, 2 bytes, 1st byte's 3-bit write command (100) and 5-bit register address
                           2nd byte 8-bit value

    @returns newly written register value (acknowledgment)
*/
uint16_t MAQ473::readRegister(uint16_t value)
{
    _spi->beginTransaction(SPISettings(CLOCK_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(_settings.pin_CS, LOW);
    uint16_t angle = _spi->transfer16(value);
    digitalWrite(_settings.pin_CS, HIGH);
    _spi->endTransaction();
    _spi->beginTransaction(SPISettings(CLOCK_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(_settings.pin_CS, LOW);
    uint16_t registerVal16 = _spi->transfer16(0x0000);
    digitalWrite(_settings.pin_CS, HIGH);
    _spi->endTransaction();
    return registerVal16;
}

/*
    Sets the zero position angle.

    @param angle, float in degrees (e.g. 20.0 degrees)

    @returns the complementary angle of the zero setting
*/
float MAQ473::setZeroAngle(float angle)
{

    uint16_t zeroPosition = floor(pow(2, 16) - ((angle / 360) * pow(2, 16)));
    Serial.println(zeroPosition);
    uint8_t lsb = zeroPosition & 0xFF;
    uint8_t msb = zeroPosition >> 8 & 0xFF;
    uint16_t sendReg0 = (0x80 << 8) | lsb;
    uint16_t sendReg1 = (0x81 << 8) | msb;

    uint16_t registerVal0 = writeRegister(sendReg0);
    uint16_t registerVal1 = writeRegister(sendReg1);

    float zeroAngle = (registerVal0 >> 8 | registerVal1) / 0xFFFF * 360.0;
    return zeroAngle;
}

/*
    Sets the Magnetic Field Thresholds.

    @param value

    @returns the value of the magnetic field thresholds

    @example setMGT(0x8630); --> Sets the MGLT and MGHT values to 41 and 78.
*/
uint16_t MAQ473::setMGT(uint16_t value)
{
    uint16_t receivedValue16 = writeRegister(value);
    return receivedValue16 >> 10;
}

/*
    Sets the Magnetic Field Thresholds.

    @param value

    @returns

    @example setMGT(0x8630); --> Sets the MGLT and MGHT values to 41 and 78.
*/
uint16_t MAQ473::getMGT()
{
    uint16_t receivedValue16 = readRegister(0x4600);
    return receivedValue16 >> 10;
}

uint16_t MAQ473::getMG()
{
    uint16_t receivedValue16 = readRegister(0x5b00);
    return receivedValue16 >> 14;
}

/*
    Gets the Magnetic Field Range.

    @param value

    @returns 4bit value logic state, 0b0011 means that the hall sensor is in the field's range
                                     0b1111 means that the magnet is too close to the sensor
                                     0b0000 means that the magnet is too far from the sensor
                                     every other value is a hall sensor functional error

    @example getMGT(0x5b00); --> Reads register 27 and gets the MIR logic state.
*/
uint16_t MAQ473::getMIR()
{
    uint16_t receivedValue16 = readRegister(0x5b00);
    uint16_t stateMIR = (receivedValue16) >> 10 & 0b001111;
    return stateMIR;
}

/*
    Sets the Filter WIndow.

    @param value, first 8bits of the value are the write command(100) and the address(01110)
                  second 8bits are the value of the register that you want to set
*/
uint16_t MAQ473::setFW(uint16_t value)
{
    uint16_t receivedValue16 = writeRegister(value);
    return receivedValue16 >> 8;
}