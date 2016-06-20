// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// MPU-6000
// This code is designed to work with the MPU-6000_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Accelorometer?sku=MPU-6000_I2CS#tabs-0-product_tabset-2

#include <application.h>
#include <spark_wiring_i2c.h>

// MPU-6000 I2C address is 0x68(104)
#define Addr 0x68

int xGyro = 0, yGyro = 0, zGyro = 0, xAccl = 0, yAccl =  0, zAccl = 0;
void setup()
{
  // Set variable
  Particle.variable("i2cdevice", "MPU-6000");
  Particle.variable("xAccl", xAccl);
  Particle.variable("yAccl", yAccl);
  Particle.variable("zAccl", zAccl);
  Particle.variable("xGyro", xGyro);
  Particle.variable("yGyro", yGyro);
  Particle.variable("zGyro", zGyro);

  // Initialise I2C communication as Master
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select gyroscope configuration register
  Wire.write(0x1B);
  // Full scale range = 2000 dps
  Wire.write(0x18);
  // Stop I2C transmission
  Wire.endTransmission();

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select accelerometer configuration register
  Wire.write(0x1C);
  // Full scale range = +/-16g
  Wire.write(0x18);
  // Stop I2C transmission
  Wire.endTransmission();

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select power management register
  Wire.write(0x6B);
  // PLL with xGyro reference
  Wire.write(0x01);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(300);
}

void loop()
{
  unsigned int data[6];

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x3B);
  // Stop I2C transmission
  Wire.endTransmission();

  // Request 6 bytes of data
  Wire.requestFrom(Addr, 6);

  // Read 6 byte of data
  if (Wire.available() == 6)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read();
  }
  delay(800);

  // Convert the data
  xAccl = ((data[1] * 256) + data[0]);
  if (xAccl > 32767)
  {
    xAccl -= 65536;
  }
  yAccl = ((data[3] * 256) + data[2]);
  if (yAccl > 32767)
  {
    yAccl -= 65536;
  }
  zAccl = ((data[5] * 256) + data[4]);
  if (zAccl > 32767)
  {
    zAccl -= 65536;
  }
  delay(800);

  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x43);
  // Stop I2C transmission
  Wire.endTransmission();

  // Request 6 bytes of data
  Wire.requestFrom(Addr, 6);

  // Read 6 byte of data
  if (Wire.available() == 6)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read();
  }

  // Convert the data
  xGyro = ((data[1] * 256) + data[0]);
  if (xGyro > 32767)
  {
    xGyro -= 65536;
  }
  yGyro = ((data[3] * 256) + data[2]);
  if (yGyro > 32767)
  {
    yGyro -= 65536;
  }
  zGyro = ((data[5] * 256) + data[4]);
  if (zGyro > 32767)
  {
    zGyro -= 65536;
  }

  // Output data to dashboard
  Particle.publish("Acceleration in X-Axis : ", String(xAccl));
  delay(1000);
  Particle.publish("Acceleration in Y-Axis : ", String(yAccl));
  delay(1000);
  Particle.publish("Acceleration in Z-Axis : ", String(zAccl));
  delay(1000);
  Particle.publish("X-Axis of Rotation : ", String(xGyro));
  delay(1000);
  Particle.publish("Y-Axis of Rotation : ", String(yGyro));
  delay(1000);
  Particle.publish("Z-Axis of Rotation : ", String(zGyro));
  delay(1000);
}

