#include <IMU.h>

IMU::IMU(bool enableAccelerometer, bool enableGyroscope, bool enableMagnet)
    : mAccelerometerEnabled(enableAccelerometer), mGyroscopeEnabled(enableGyroscope), mMagnetEnabled(enableMagnet)
{
}

IMU::~IMU()
{
}

DeviceReadings::Status IMU::Init()
{
    int tryCount = 5;
    do
    {
        if (mImu.begin() == INV_SUCCESS)
        {
            return DeviceReadings::Status::OK;
        }
        tryCount--;
        delay(1000);

    } while (tryCount != 0);

    return DeviceReadings::Status::ERROR;
}

void IMU::ReadSensors()
{
    if (mAccelerometerEnabled)
        ReadAccelerometer();
    if (mMagnetEnabled)
        ReadMagnometer();
    if (mGyroscopeEnabled)
        ReadGyroscope();
}

DeviceReadings::Result IMU::GetAcceleration()
{
    return mAcceleration;
}

DeviceReadings::Result IMU::GetGyroscope()
{
    return mGyro;
}

DeviceReadings::Result IMU::GetMagnetation()
{
    return mMagnetation;
}

void IMU::ReadAccelerometer()
{
    float accelX = mImu.calcAccel(mImu.ax);
    float accelY = mImu.calcAccel(mImu.ay);
    float accelZ = mImu.calcAccel(mImu.az);

    mAcceleration = {accelX, accelY, accelZ};
}

void IMU::ReadMagnometer()
{
    float magX = mImu.calcMag(mImu.mx);
    float magY = mImu.calcMag(mImu.my);
    float magZ = mImu.calcMag(mImu.mz);

    mMagnetation = {magX, magY, magZ};
}

void IMU::ReadGyroscope()
{
    float gyroX = mImu.calcGyro(mImu.gx);
    float gyroY = mImu.calcGyro(mImu.gy);
    float gyroZ = mImu.calcGyro(mImu.gz);

    mGyro = {gyroX, gyroY, gyroZ};
}
