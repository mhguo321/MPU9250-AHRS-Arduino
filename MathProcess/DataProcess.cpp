#include "DataProcess.h"



short FloatToFixed(float floatValue, Qvals q)
{
	int temp = (int)( (floatValue) * (int)(1<<q) );
	if (temp > 32767)
		temp = 32767;
	else if (temp < -32768)
		temp = -32768;
	return (short)temp;
}

//传感器数据编码
void InsertChecksum(uchar * decodePacket,int Length) // 01：计算末尾校验位
{
	decodePacket[Length - 1] = 0;
	int i;
	for(i = 0;i<Length - 1;i++)
	{
		decodePacket[Length - 1] += decodePacket[i];
	}
}

void RightShiftByteArray(uchar * byteArray,int Length) // 02: 右移
{
	byteArray[Length - 1] >>= 1;
	int i;
	for(i = Length - 2;i >= 0;i--)
	{
		if ( (byteArray[i] & 0x01) == 0x01 )
			byteArray[i+1] |= 0x80;
		byteArray[i] >>=1;
	}
}

void EncodePacket(uchar * decodePacket,int de_Length, uchar * encodePacket,int en_Length) // 03:编码
{
	uchar shiftRegister[23] = {}; 
	int i;
	for(i = 0;i<de_Length;i++)
	{
		shiftRegister[i] = decodePacket[i];
	}
	for(i = 0; i< en_Length; i++)
	{
		RightShiftByteArray(shiftRegister,en_Length);
		encodePacket[i] = shiftRegister[i];
		shiftRegister[i] = 0;
	}
	encodePacket[22] |= 0x80;
}


void DataIntoPacket(uchar * decode,float GyrX,float GyrY,float GyrZ,float AccX,float AccY,float AccZ,float MagX,float MagY,float MagZ)
{
	short gx,gy,gz,ax,ay,az,cx,cy,cz;
	gx = FloatToFixed(GyrX, CalibratedGyroscope);
	gy = FloatToFixed(GyrY, CalibratedGyroscope);
	gz = FloatToFixed(GyrZ, CalibratedGyroscope);
	
	ax = FloatToFixed(AccX, CalibratedAccelerometer);
	ay = FloatToFixed(AccY, CalibratedAccelerometer);
	az = FloatToFixed(AccZ, CalibratedAccelerometer);
	
	cx = FloatToFixed(MagX, CalibratedMagnetometer);
	cy = FloatToFixed(MagY, CalibratedMagnetometer);
	cz = FloatToFixed(MagZ, CalibratedMagnetometer);
	//Gyr
	decode[1] = (uchar)((gx & 0xFF00) >>8 );//MSB
	decode[2] = (uchar)(gx & 0x00FF);//LSB
	
	decode[3] = (uchar)((gy & 0xFF00) >>8 );
	decode[4] = (uchar)(gy & 0x00FF);
	
	decode[5] = (uchar)((gz & 0xFF00) >>8 );
	decode[6] = (uchar)(gz & 0x00FF);
	//Acc
	decode[7] = (uchar)((ax & 0xFF00) >>8 );//MSB
	decode[8] = (uchar)(ax & 0x00FF);//LSB
	
	decode[9] = (uchar)((ay & 0xFF00) >>8 );
	decode[10] = (uchar)(ay & 0x00FF);
	
	decode[11] = (uchar)((az & 0xFF00) >>8 );
	decode[12] = (uchar)(az & 0x00FF);
	//Mag
	decode[13] = (uchar)((cx & 0xFF00) >>8 );//MSB
	decode[14] = (uchar)(cx & 0x00FF);//LSB
	
	decode[15] = (uchar)((cy & 0xFF00) >>8 );
	decode[16] = (uchar)(cy & 0x00FF);
	
	decode[17] = (uchar)((cz & 0xFF00) >>8 );
	decode[18] = (uchar)(cz & 0x00FF);
}