#ifndef __MAQ473_H
#define __MAQ473_H

#include <Arduino.h>
#include <SPI.h>
#include <stdint.h>

// STM32 pinouts
#define MAQ473_SCK PA5
#define MAQ473_MISO PA6
#define MAQ473_MOSI PA7
#define MAQ473_CS PB4

// #define CLOCK_SPEED 20000000
#define CLOCK_SPEED 1000000

struct MAQ473Settings
{
    uint8_t pin_CS;
    uint8_t pin_MOSI;
    uint8_t pin_MISO;
    uint8_t pin_SCK;
    uint32_t clock;
};

class MAQ473
{
public:
    MAQ473();
    MAQ473(uint8_t pin_CS);
    MAQ473(struct MAQ473Settings);
    MAQ473(uint8_t pin_CS, uint8_t pin_MOSI, uint8_t pin_MISO, uint8_t pin_SCK);
    MAQ473(uint32_t clock, uint8_t pin_CS, uint8_t pin_MOSI, uint8_t pin_MISO, uint8_t pin_SCK);

    int begin();

    float readAngle();
    uint16_t readAngleRaw();
    uint16_t readRegister(uint16_t bytes);
    uint16_t writeRegister(uint16_t bytes);
    float setZeroAngle(float angle);
    uint16_t setMGT(uint16_t value);
    uint16_t getMGT();
    uint16_t getMIR();
    uint16_t getMG();
    uint16_t setFW(uint16_t value);

private:
    struct MAQ473Settings _settings;
    SPIClass *_spi;
};

#endif