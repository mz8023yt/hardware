#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

class I2CMaster {
public:
  virtual ~I2CMaster() = default;

  virtual bool open() = 0;
  virtual void close() = 0;

  virtual bool readReg8(uint8_t deviceAddr, uint8_t regAddr, uint8_t &data) = 0;
  virtual bool writeReg8(uint8_t deviceAddr, uint8_t regAddr, uint8_t data) = 0;
  virtual bool readRegs8(uint8_t deviceAddr, uint8_t startReg,
                         std::vector<uint8_t> &data, size_t length) = 0;
  virtual bool writeRegs8(uint8_t deviceAddr, uint8_t startReg,
                          const std::vector<uint8_t> &data) = 0;

  virtual bool readReg16(uint8_t deviceAddr, uint16_t regAddr,
                         uint8_t &data) = 0;
  virtual bool writeReg16(uint8_t deviceAddr, uint16_t regAddr,
                          uint8_t data) = 0;
  virtual bool readRegs16(uint8_t deviceAddr, uint16_t startReg,
                          std::vector<uint8_t> &data, size_t length) = 0;
  virtual bool writeRegs16(uint8_t deviceAddr, uint16_t startReg,
                           const std::vector<uint8_t> &data) = 0;
};
