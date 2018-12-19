#ifndef _DATAPROCESS_H
#define _DATAPROCESS_H

#define uchar unsigned char

typedef enum _PacketHeaders
{
	Error,
	Command,
	ReadRegister,
	WriteRegister,
	ReadDateTime,
	WriteDateTime,
	RawBatteryAndThermometerData,
	CalBatteryAndThermometerData,
	RawInertialAndMagneticData,
	CalInertialAndMagneticData,
	QuaternionData,
	DigitalIOdata,
	RawAnalogueInputData,
	CalAnalogueInputData,
	PWMoutputData,
	RawADXL345busData,
	CalADXL345busData,
}PacketHeaders;

typedef enum _Qvals
{
    CalibratedBattery = 12,
    CalibratedThermometer = 8,
    CalibratedGyroscope = 4,
    CalibratedAccelerometer = 11,
    CalibratedMagnetometer = 11,
    Quaternion = 15,
    BatterySensitivity = 5,
    BatteryBias = 8,
    ThermometerSensitivity = 6,
    ThermometerBias = 0,
    GyroscopeSensitivity = 7,
    GyroscopeSampled200dps = 0,
    GyroscopeBiasAt25degC = 3,
    GyroscopeBiasTempSensitivity = 11,
    GyroscopeSampledBias = 3,
    AccelerometerSensitivity = 4,
    AccelerometerBias = 8,
    AccelerometerSampled1g = 4,
    MagnetometerSensitivity = 4,
    MagnetometerBias = 8,
    MagnetometerHardIronBias = 11,
    AlgorithmKp = 11,
    AlgorithmKi = 15,
    AlgorithmInitKp = 11,
    AlgorithmInitPeriod = 11,
    CalibratedAnalogueInput = 12,
    AnalogueInputSensitivity = 4,
    AnalogueInputBias = 8,
    CalibratedADXL345 = 10,
    ADXL345busSensitivity = 6,
    ADXL345busBias = 8,
}Qvals;

short FloatToFixed(float floatValue, Qvals q);
void InsertChecksum(uchar * decodePacket,int Length);
void RightShiftByteArray(uchar * byteArray,int Length);
void EncodePacket(uchar * decodePacket,int de_Length, uchar * encodePacket,int en_Length);
void DataIntoPacket(uchar * decode,float GyrX,float GyrY,float GyrZ,float AccX,float AccY,float AccZ,float MagX,float MagY,float MagZ);

#endif