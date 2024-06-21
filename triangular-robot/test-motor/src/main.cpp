#include <Arduino.h>
// #include <IMU.h>
// #include <SparkFunMPU9250-DMP.h>

#include <MagAlpha.h>
#include <CytronMotorDriver.h>
#include <RadioController.h>

#define M_PI 3.14159265358979323846

#define UART_BAUDRATE 9600          // UART data rate in bits per second (baud)
#define SPI_SCLK_FREQUENCY 10000000 // SPI SCLK Clock frequency in Hz
#define SPI_CS_PIN PB4              // SPI CS pin
#define SPI_CS_PIN2 PB5             // SPI CS pin
#define SPI_CS_PIN3 PB3             // SPI CS pin

CytronMD motorA(MODE::PWM_DIR, PC8, PB1);
CytronMD motorB(MODE::PWM_DIR, PC6, PB15);
CytronMD motorC(MODE::PWM_DIR, PC9, PB14);

MagAlpha hallSensorA;
MagAlpha hallSensorB;
MagAlpha hallSensorC;

RadioController signal1(PB7);
RadioController signal2(PC13);
RadioController signal3(PB0);
void Change1() { RadioController::HandleIsr(&signal1); }
void Change2() { RadioController::HandleIsr(&signal2); }
void Change3() { RadioController::HandleIsr(&signal3); }

void setup()
{
  // Set the Serial Communication
  Serial.begin(UART_BAUDRATE);

  signal1.SetIsr(&Change1);
  signal2.SetIsr(&Change2);
  signal3.SetIsr(&Change3);
  pinMode(PF1, INPUT);

  // Set the SPI SCLK frequency, SPI Mode and CS pin
  // hallSensorA.begin(SPI_SCLK_FREQUENCY, MA_SPI_MODE_3, SPI_CS_PIN);
  // hallSensorB.begin(SPI_SCLK_FREQUENCY, MA_SPI_MODE_3, SPI_CS_PIN2);
  // hallSensorC.begin(SPI_SCLK_FREQUENCY, MA_SPI_MODE_3, SPI_CS_PIN3);
}

float Distance(int x1, int y1, int x2, int y2)
{
  // Calculating distance
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void loop()
{
  int x = map(signal1.GetValue(), signal1.GetRangeMin(), signal1.GetRangeMax(), -255, 255);
  int y = map(signal2.GetValue(), signal2.GetRangeMin(), signal2.GetRangeMax(), -255, 255);
  int r = map(signal3.GetValue(), signal3.GetRangeMin(), signal3.GetRangeMax(), -255, 255);

  float vel = Distance(0, 0, x, y);
  if (vel > 255)
  {
    vel = 255;
  }

  int f_a = vel * sin(90.0 * M_PI / 180.0 - atan2(y, x)) + r;
  int f_b = vel * sin(210.0 * M_PI / 180.0 - atan2(y, x)) + r;
  int f_c = vel * sin(330.0 * M_PI / 180.0 - atan2(y, x)) + r;

  f_a = constrain(f_a, -255, 255);
  f_b = constrain(f_b, -255, 255);
  f_c = constrain(f_c, -255, 255);

  if (!digitalRead(PF1))
  {
    motorA.setSpeed(abs(f_a));
    motorB.setSpeed(abs(f_b));
    motorC.setSpeed(abs(f_c));
  }
  else
  {
    motorA.setSpeed(0);
    motorB.setSpeed(0);
    motorC.setSpeed(0);
  }
}
