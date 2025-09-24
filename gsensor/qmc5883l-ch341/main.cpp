#include "ch341i2cmaster.hpp"
#include "qmc5883l.hpp"

#include <cmath> // atan2, M_PI
#include <iostream>
#include <unistd.h> // usleep

int main() {
  // CH341 设备路径，按实际修改
  const std::string devicePath = "/dev/ch34x_pis2";

  // 创建 CH341 I2C 主控对象
  CH341I2CMaster i2c(devicePath);
  if (!i2c.open()) {
    std::cerr << "Failed to open CH341 I2C device!" << std::endl;
    return -1;
  }

  // 创建 QMC5883L 对象
  QMC5883L sensor(i2c);

  // 读取 ID 寄存器
  uint8_t id = 0;
  if (sensor.readID(id)) {
    std::cout << "QMC5883L ID: 0x" << std::hex << static_cast<int>(id)
              << std::endl;
  } else {
    std::cerr << "Failed to read QMC5883L ID!" << std::endl;
  }

  // 初始化 QMC5883L
  if (!sensor.init()) {
    std::cerr << "Failed to initialize QMC5883L!" << std::endl;
    i2c.close();
    return -1;
  }

  while (true) {
    int16_t x, y, z;
    if (sensor.readRaw(x, y, z)) {
      double heading = atan2((double)y, (double)x) * 180.0 / M_PI;
      if (heading < 0) {
        heading += 360.0;
      }

      std::cout << "X=" << x << " Y=" << y << " Z=" << z
                << " Heading=" << heading << "°" << std::endl;
    } else {
      std::cerr << "Failed to read raw data" << std::endl;
    }

    usleep(200000); // 200ms 刷新一次
  }

  // 关闭 I2C
  i2c.close();
  return 0;
}
