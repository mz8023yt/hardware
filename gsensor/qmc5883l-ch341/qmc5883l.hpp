#pragma once
#include "ch341i2cmaster.hpp"
#include <cstdint>

class QMC5883L {
public:
  explicit QMC5883L(CH341I2CMaster &i2c, uint8_t deviceAddr = 0x0D);

  // 读取 ID 寄存器（0x0D）
  bool readID(uint8_t &id);

  // 读取状态寄存器（0x06）
  bool readStatus(uint8_t &status);

  // 初始化，配置为连续测量模式
  bool init();

  // 读取三轴原始数据
  bool readRaw(int16_t &x, int16_t &y, int16_t &z);

private:
  CH341I2CMaster &i2c_;
  uint8_t deviceAddr_;
};
