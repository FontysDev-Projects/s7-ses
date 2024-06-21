#pragma once

#include <SparkFunMPU9250-DMP.h>
#include <vector>

namespace DeviceReadings
{
    enum Status
    {
        OK = 0,
        ERROR = -1
    };
    /*
     * Result struct to store a 3-axis value
     * Can be used for acceleration, magnetic or gyroscopic values
     */
    struct Result
    {
        float x;
        float y;
        float z;
    };
}

class IMU
{
public:
    /*
     * @param enableAccelerometer    Enable the accelerometer sensor to measure acceleration
     * @param enableGyroscope        Enable the gyroscope to measure orientation
     * @param enableMagnet           Enable the magnet to measure the strength of magnetic fields
     */
    IMU(bool enableAccelerometer, bool enableGyroscope, bool enableMagnet);
    ~IMU();

    /*
     * Initialize MPU-9250
     */
    DeviceReadings::Status Init();

    /*
     * Collect and save the sensor's data into different members
     */
    void ReadSensors();

    /*
     * Get last acceleration value
     */
    DeviceReadings::Result GetAcceleration();
    /*
     * Get last gyroscopic value
     */
    DeviceReadings::Result GetGyroscope();
    /*
     * Get last magnetic value
     */
    DeviceReadings::Result GetMagnetation();

private:
    MPU9250_DMP mImu;

    DeviceReadings::Result mAcceleration;
    DeviceReadings::Result mGyro;
    DeviceReadings::Result mMagnetation;

    bool mAccelerometerEnabled;
    bool mGyroscopeEnabled;
    bool mMagnetEnabled;

    void ReadAccelerometer();
    void ReadMagnometer();
    void ReadGyroscope();
};
