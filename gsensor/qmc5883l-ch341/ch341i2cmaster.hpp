#pragma once

#include "i2cmaster.hpp"
#include <cstdbool>
#include <cstdint>
#include <string>
#include <unistd.h>
#include <vector>

#include "ch341_lib.h"

class CH341I2CMaster : public I2CMaster {
public:
  CH341I2CMaster(const std::string &devicePath);
  virtual ~CH341I2CMaster();

  bool open() override;
  void close() override;

  bool readReg8(uint8_t deviceAddr, uint8_t regAddr, uint8_t &data) override;
  bool writeReg8(uint8_t deviceAddr, uint8_t regAddr, uint8_t data) override;
  bool readRegs8(uint8_t deviceAddr, uint8_t startReg,
                 std::vector<uint8_t> &data, size_t length) override;
  bool writeRegs8(uint8_t deviceAddr, uint8_t startReg,
                  const std::vector<uint8_t> &data) override;

  bool readReg16(uint8_t deviceAddr, uint16_t regAddr, uint8_t &data) override;
  bool writeReg16(uint8_t deviceAddr, uint16_t regAddr, uint8_t data) override;
  bool readRegs16(uint8_t deviceAddr, uint16_t startReg,
                  std::vector<uint8_t> &data, size_t length) override;
  bool writeRegs16(uint8_t deviceAddr, uint16_t startReg,
                   const std::vector<uint8_t> &data) override;

private:
  int fd_;
  std::string devicePath_;
};
