// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// MPU-6000
// This code is designed to work with the MPU-6000_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Accelorometer?sku=MPU-6000_I2CS#tabs-0-product_tabset-2

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, MPU-6000 I2C address is 0x68(104)
	ioctl(file, I2C_SLAVE, 0x68);

	// Select gyroscope configuration register(0x1B)
	char config[2] = {0};
	config[0] = 0x1B;
	config[1] = 0x18;
	write(file, config, 2);
	// Select accelerometer configuration register(1C)
	config[0] = 0x1C;
	config[1] = 0x18;
	write(file, config, 2);
	// Select power management register1(6B)
	config[0] = 0x6B;
	config[1] = 0x01;
	write(file, config, 2);
	sleep(1);

	// Read 6 bytes of data from register(0x3B)
	// xAccl msb, xAccl lsb, yAccl msb, yAccl lsb, zAccl msb, zAccl lsb
	char reg[1] = {0x3B};
	write(file, reg, 1);
	char data[6] = {0};
	if(read(file, data, 6) != 6)
	{
		printf("Error : Input/output Error \n");
		exit(1);
	}

	// Convert the data
	int xAccl = (data[0] * 256 + data[1]);
	if(xAccl > 32767)
	{
		xAccl -= 65536;
	}

	int yAccl = (data[2] * 256 + data[3]);
	if(yAccl > 32767)
	{
		yAccl -= 65536;
	}

	int zAccl = (data[4] * 256 + data[5]);
	if(zAccl > 32767)
	{
		zAccl -= 65536;
	}

	// Read 6 bytes of data from register(0x43)
	// xGyro msb, xGyro lsb, yGyro msb, yGyro lsb, zGyro msb, zGyro lsb
	reg[0] = 0x43;
	write(file, reg, 1);
	read(file, data, 6);

	// Convert the data
	int xGyro = (data[0] * 256 + data[1]);
	if (xGyro > 32767)
	{
	xGyro -= 65536;
	}

	int yGyro = (data[2] * 256 + data[3]);
	if (yGyro > 32767)
	{
	yGyro -= 65536;
	}

	int zGyro = (data[4] * 256 + data[5]);
	if (zGyro > 32767)
	{
		zGyro -= 65536;
	}

	// Output data to screen
	printf("Acceleration in X-axis : %d \n", xAccl);
	printf("Acceleration in Y-axis : %d \n", yAccl);
	printf("Acceleration in Z-axis : %d \n", zAccl);
	printf("X-Axis of Rotation : %d \n", xGyro);
	printf("Y-Axis of Rotation : %d \n", yGyro);
	printf("Z-Axis of Rotation : %d \n", zGyro);
}
