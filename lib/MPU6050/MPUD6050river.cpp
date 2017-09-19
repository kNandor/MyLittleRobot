#include<MPU6050Driver.h>


// int MPU6050Driver::init(){
//   // According to the datasheet, the 'sleep' bit
//   // should read a '1'.
//   // That bit has to be cleared, since the sensor
//   // is in sleep mode at power-up.
//   int error;
//   uint8_t c;
//   error = read (MPU6050_PWR_MGMT_1, &c, 1);
//   // Clear the 'sleep' bit to start the sensor.
//   write_reg (MPU6050_PWR_MGMT_1, 0);
//   return error;
// }

CMPU6050Driver::MPU6050_DATA CMPU6050Driver::readValue(){
  CMPU6050Driver::MPU6050_REG l_data;
  read (MPU6050_ACCEL_XOUT_H, (uint8_t *) &l_data, sizeof(l_data));
  uint8_t swap;
  #define SWAP(x,y) swap = x; x = y; y = swap
  SWAP (l_data.reg.x_accel_h, l_data.reg.x_accel_l);
  SWAP (l_data.reg.y_accel_h, l_data.reg.y_accel_l);
  SWAP (l_data.reg.z_accel_h, l_data.reg.z_accel_l);
  SWAP (l_data.reg.t_h, l_data.reg.t_l);
  SWAP (l_data.reg.x_gyro_h, l_data.reg.x_gyro_l);
  SWAP (l_data.reg.y_gyro_h, l_data.reg.y_gyro_l);
  SWAP (l_data.reg.z_gyro_h, l_data.reg.z_gyro_l);


  CMPU6050Driver::MPU6050_DATA l_data1;

  l_data1.temp = ( (double) l_data.value.temperature + 12412.0) / 340.0;
  l_data1.x_accel=l_data.value.x_accel/accel_scale;
  l_data1.y_accel=l_data.value.y_accel/accel_scale;
  l_data1.z_accel=l_data.value.z_accel/accel_scale;

  l_data1.x_gyro=l_data.value.x_gyro/gyro_scale;
  l_data1.y_gyro=l_data.value.y_gyro/gyro_scale;
  l_data1.z_gyro=l_data.value.z_gyro/gyro_scale;
  return l_data1;
}


int CMPU6050Driver::read(int start, uint8_t *buffer, int size)
{
 int i, n;

 Wire.beginTransmission(MPU6050_I2C_ADDRESS);
 n = Wire.write(start);
 if (n != 1)
   return (-10);

 n = Wire.endTransmission(false);    // hold the I2C-bus
 if (n != 0)
   return (n);

 // Third parameter is true: relase I2C-bus after data is read.
 Wire.requestFrom(MPU6050_I2C_ADDRESS, size, true);
 i = 0;
 while(Wire.available() && i<size)
 {
   buffer[i++]=Wire.read();
 }
 if ( i != size)
   return (-11);

 return (0);  // return : no error
}

int CMPU6050Driver::write(int start, const uint8_t *pData, int size)
{
 int n, error;

 Wire.beginTransmission(MPU6050_I2C_ADDRESS);
 n = Wire.write(start);        // write the start address
 if (n != 1)
   return (-20);

 n = Wire.write(pData, size);  // write data bytes
 if (n != size)
   return (-21);

 error = Wire.endTransmission(true); // release the I2C-bus
 if (error != 0)
   return (error);

 return (0);         // return : no error
}

int CMPU6050Driver::write_reg(int reg, uint8_t data)
{
 int error;

 error = write(reg, &data, 1);

 return (error);
}
