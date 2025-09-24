#include "qmc5883l.hpp"

QMC5883L::QMC5883L(CH341I2CMaster &i2c, uint8_t deviceAddr)
    : i2c_(i2c), deviceAddr_(deviceAddr) {}

// 读取 ID 寄存器
bool QMC5883L::readID(uint8_t &id) {
  return i2c_.readReg8(deviceAddr_, 0x0D, id);
}

// 读取状态寄存器
bool QMC5883L::readStatus(uint8_t &status) {
  return i2c_.readReg8(deviceAddr_, 0x06, status);
}

bool QMC5883L::init() {
  // 1. 软件复位
  if (!i2c_.writeReg8(deviceAddr_, 0x0B, 0x01)) { // REG_RESET = 0x0B
    return false;
  }
  usleep(200000); // 200ms

  // 2. 配置控制寄存器2 (0x0A)
  if (!i2c_.writeReg8(deviceAddr_, 0x0A, 0x00)) { // REG_CONTROL_2 = 0x0A
    return false;
  }

  // 3. 配置控制寄存器1 (0x09)
  // 0x1D = OSR=512, RNG=2G, ODR=8Hz, Continuous mode
  if (!i2c_.writeReg8(deviceAddr_, 0x09, 0x1D)) { // REG_CONTROL_1 = 0x09
    return false;
  }
  usleep(100000); // 100ms

  return true;
}

bool QMC5883L::readRaw(int16_t &x, int16_t &y, int16_t &z) {
  uint8_t status = 0;

  // 检查状态寄存器 (0x06)
  if (!i2c_.readReg8(deviceAddr_, 0x06, status)) {
    return false;
  }

  if (!(status & 0x01)) {
    // 数据未就绪
    return false;
  }

  // 连续读取 6 字节 (0x00 ~ 0x05)
  std::vector<uint8_t> buf;
  if (!i2c_.readRegs8(deviceAddr_, 0x00, buf, 6)) {
    return false;
  }

  // 合并成有符号 16-bit (低字节在前)
  x = static_cast<int16_t>(buf[0] | (buf[1] << 8));
  y = static_cast<int16_t>(buf[2] | (buf[3] << 8));
  z = static_cast<int16_t>(buf[4] | (buf[5] << 8));

  return true;
}
