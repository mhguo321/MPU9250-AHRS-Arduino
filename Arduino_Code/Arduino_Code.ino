#include "MPU9250_new.h"
#include "MathProcess/DataProcess.h"   

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire,0x68);
int status;
// 静态数据：
static float accX,accY,accZ;
static float gyrX,gyrY,gyrZ;
static float magX,magY,magZ;
static float tempreture;
static uchar decodePacket[20] = {0x09,
      0x01,0x12,0x03,0x84,0xF3,0x44,
      0x01,0x8A,0x13,0x04,0x03,0x34,
      0x31,0x02,0x03,0x56,0x23,0x17,
      0x00};
static int de_Length = 20;
static int en_Length = 23;
static uchar encodePacket[23];
const float _r2d = 180.0f/3.14159265359f;
static int sensorValue = 0;
static float voltage = 0.0;


void setup() {
  // serial to display data
  Serial.begin(115200);
  while(!Serial) {}

  // start communication with IMU 
  status = IMU.begin();
  
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }
  /******************************************************/
  /********** Configuration of MPU9250 ************/
  // setting the accelerometer full scale range to +/-8G 
  IMU.setAccelRange(MPU9250::ACCEL_RANGE_4G);
  // setting the gyroscope full scale range to +/-2000 deg/s
  IMU.setGyroRange(MPU9250::GYRO_RANGE_2000DPS);
  // setting DLPF bandwidth to 184 Hz
  IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_184HZ);
  // setting SRD to 1 for a 500 Hz update rate
  IMU.setSrd(1);
  /******************************************************/
  /******************************************************/
    //初始化处理：
  InsertChecksum(decodePacket,20);
  EncodePacket(decodePacket,de_Length, encodePacket,en_Length);// 03:编码
}
/******************************************************/
/******************************************************/
/******************************************************/
/******************************************************/
void loop() 
{  
    IMU.readSensor();// read the sensor
    accX = 1/9.0*IMU.getAccelX_mss();//单位变成g
    accY = 1/9.0*IMU.getAccelY_mss();
    accZ = -1/9.0*IMU.getAccelZ_mss();
    gyrX = _r2d*IMU.getGyroY_rads();//单位变成deg/s
    gyrY = _r2d*IMU.getGyroX_rads();//Gx和Gy方向要颠倒过来用
    gyrZ = -1.0f*_r2d*IMU.getGyroZ_rads();
    magX = IMU.getMagX_uT();
    magY = IMU.getMagY_uT();
    magZ = IMU.getMagZ_uT();
    //tempreture = IMU.getTemperature_C();


    DataIntoPacket(decodePacket,
                   gyrX,gyrY,gyrZ,
                   accX,accY,accZ,
                   magX,magY,magZ);//01:打包

    InsertChecksum(decodePacket,20);//02:校验
    EncodePacket(decodePacket,de_Length, encodePacket,en_Length);// 03:编码
    Serial.write(encodePacket,23);
    
    delay(3); //Trying to make it work at 200Hz
} 
