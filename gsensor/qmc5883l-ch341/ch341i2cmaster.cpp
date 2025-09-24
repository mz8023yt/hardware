#include "ch341i2cmaster.hpp"
#include <cstring>

CH341I2CMaster::CH341I2CMaster(const std::string &devicePath)
    : fd_(-1), devicePath_(devicePath) {}

CH341I2CMaster::~CH341I2CMaster() { close(); }

bool CH341I2CMaster::open() {
  fd_ = CH34xOpenDevice(devicePath_.c_str());
  if (fd_ < 0)
    return false;

  // 设置 I2C 流模式，默认标准速率 100KHz
  return CH34xSetStream(fd_, 0x01);
}

void CH341I2CMaster::close() {
  if (fd_ >= 0) {
    CH34xCloseDevice(fd_);
    fd_ = -1;
  }
}

// ===================== 8-bit 寄存器 =====================
bool CH341I2CMaster::readReg8(uint8_t deviceAddr, uint8_t regAddr,
                              uint8_t &data) {
  uint8_t buf[2] = {static_cast<uint8_t>(deviceAddr << 1), regAddr};
  return CH34xStreamI2C(fd_, 2, buf, 1, &data);
}

bool CH341I2CMaster::writeReg8(uint8_t deviceAddr, uint8_t regAddr,
                               uint8_t data) {
  uint8_t buf[3] = {static_cast<uint8_t>(deviceAddr << 1), regAddr, data};
  return CH34xStreamI2C(fd_, 3, buf, 0, nullptr);
}

bool CH341I2CMaster::readRegs8(uint8_t deviceAddr, uint8_t startReg,
                               std::vector<uint8_t> &data, size_t length) {
  data.resize(length);
  uint8_t buf[2] = {static_cast<uint8_t>(deviceAddr << 1), startReg};
  return CH34xStreamI2C(fd_, 2, buf, static_cast<uint32_t>(length),
                        data.data());
}

bool CH341I2CMaster::writeRegs8(uint8_t deviceAddr, uint8_t startReg,
                                const std::vector<uint8_t> &data) {
  std::vector<uint8_t> buf(data.size() + 2);
  buf[0] = static_cast<uint8_t>(deviceAddr << 1);
  buf[1] = startReg;
  memcpy(buf.data() + 2, data.data(), data.size());
  return CH34xStreamI2C(fd_, static_cast<uint32_t>(buf.size()), buf.data(), 0,
                        nullptr);
}

// ===================== 16-bit 寄存器 =====================
bool CH341I2CMaster::readReg16(uint8_t deviceAddr, uint16_t regAddr,
                               uint8_t &data) {
  uint8_t buf[3] = {static_cast<uint8_t>(deviceAddr << 1),
                    static_cast<uint8_t>(regAddr >> 8),
                    static_cast<uint8_t>(regAddr & 0xFF)};
  return CH34xStreamI2C(fd_, 3, buf, 1, &data);
}

bool CH341I2CMaster::writeReg16(uint8_t deviceAddr, uint16_t regAddr,
                                uint8_t data) {
  uint8_t buf[4] = {static_cast<uint8_t>(deviceAddr << 1),
                    static_cast<uint8_t>(regAddr >> 8),
                    static_cast<uint8_t>(regAddr & 0xFF), data};
  return CH34xStreamI2C(fd_, 4, buf, 0, nullptr);
}

bool CH341I2CMaster::readRegs16(uint8_t deviceAddr, uint16_t startReg,
                                std::vector<uint8_t> &data, size_t length) {
  data.resize(length);
  uint8_t buf[3] = {static_cast<uint8_t>(deviceAddr << 1),
                    static_cast<uint8_t>(startReg >> 8),
                    static_cast<uint8_t>(startReg & 0xFF)};
  return CH34xStreamI2C(fd_, 3, buf, static_cast<uint32_t>(length),
                        data.data());
}

bool CH341I2CMaster::writeRegs16(uint8_t deviceAddr, uint16_t startReg,
                                 const std::vector<uint8_t> &data) {
  std::vector<uint8_t> buf(data.size() + 3);
  buf[0] = static_cast<uint8_t>(deviceAddr << 1);
  buf[1] = static_cast<uint8_t>(startReg >> 8);
  buf[2] = static_cast<uint8_t>(startReg & 0xFF);
  memcpy(buf.data() + 3, data.data(), data.size());
  return CH34xStreamI2C(fd_, static_cast<uint32_t>(buf.size()), buf.data(), 0,
                        nullptr);
}
