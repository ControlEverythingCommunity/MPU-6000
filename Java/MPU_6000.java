// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// MPU-6000
// This code is designed to work with the MPU-6000_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Accelorometer?sku=MPU-6000_I2CS#tabs-0-product_tabset-2

import com.pi4j.io.i2c.I2CBus;
import com.pi4j.io.i2c.I2CDevice;
import com.pi4j.io.i2c.I2CFactory;
import java.io.IOException;

public class MPU_6000
{
	public static void main(String args[]) throws Exception
	{
		// Create I2C bus
		I2CBus Bus = I2CFactory.getInstance(I2CBus.BUS_1);
		// Get I2C device, MPU_6000 I2C address is 0x68(104)
		I2CDevice device = Bus.getDevice(0x68);

		// Select gyroscope configuration register
		device.write(0x1B, (byte)0x18);
		// Select accelerometer configuration register
		device.write(0x1C, (byte)0x18);
		// Select power management register1
		device.write(0x6B, (byte)0x01);
		Thread.sleep(500);

		// Read 6 bytes of data
		// xAccl msb, xAccl lsb, yAccl msb, yAccl lsb, zAccl msb, zAccl lsb
		byte[] data = new byte[6];
		device.read(0x3B, data, 0, 6);

		// Convert the data
		int xAccl = ((data[0] & 0xFF) * 256 + (data[1] & 0xFF));
		if(xAccl > 32767)
		{
			xAccl -= 65536;
		}
		int yAccl = ((data[2] & 0xFF) * 256 + (data[3] & 0xFF));
		if(yAccl > 32767)
		{
			yAccl -= 65536;
		}
		int zAccl = ((data[4] & 0xFF) * 256 + (data[5] & 0xFF));
		if(zAccl > 32767)
		{
			zAccl -= 65536;
		}

		// Read 6 bytes of data
		// xGyro msb, xGyro lsb, yGyro msb, yGyro lsb, zGyro msb, zGyro lsb
		device.read(0x43, data, 0, 6);

		// Convert the data
		int xGyro = ((data[0] & 0xFF) * 256 + (data[1] & 0xFF));
		if(xGyro > 32767)
		{
			xGyro -= 65536;
		}
		int yGyro = ((data[2] & 0xFF) * 256 + (data[3] & 0xFF));
		if(yGyro > 32767)
		{
			yGyro -= 65536;
		}
		int zGyro = ((data[4] & 0xFF) * 256 + (data[5] & 0xFF));
		if(zGyro > 32767)
		{
			zGyro -= 65536;
		}

		// Output data to screen
		System.out.printf("Acceleration in X-axis : %d %n", xAccl);
		System.out.printf("Acceleration in Y-axis : %d %n", yAccl);
		System.out.printf("Acceleration in Z-axis : %d %n", zAccl);
		System.out.printf("X-Axis of Rotation : %d %n", zGyro);
		System.out.printf("Y-Axis of Rotation : %d %n", yGyro);
		System.out.printf("Z-Axis of Rotation : %d %n", zGyro);
	}
}
