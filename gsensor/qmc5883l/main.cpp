#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <cmath>
#include <iostream>
#include <cstdint>

class QMC5883L {
public:
    QMC5883L(const char* i2cBus = "/dev/i2c-1", uint8_t addr = 0x0D)
        : i2cAddr(addr)
    {
        fd = open(i2cBus, O_RDWR);
        if (fd < 0) {
            perror("Open I2C bus");
            exit(1);
        }
        if (ioctl(fd, I2C_SLAVE, i2cAddr) < 0) {
            perror("I2C ioctl");
            exit(1);
        }

        // 初始化 QMC5883L：Continuous measurement, 8Hz, 2G
        // 参考寄存器：
        // CONTROL_REG_1 0x0A: [OSR=0x01, RNG=0x01, ODR=0x01, MODE=0x01]
        writeRegister(0x0B, 0x01); // SET RESET
        writeRegister(0x09, 0x1D); // CONTROL_REG_1: OSR=512, 8Hz, continuous
    }

    ~QMC5883L() {
        if (fd >= 0) close(fd);
    }

    struct MagData { int16_t x, y, z; };

    MagData readRaw() {
        uint8_t buf[6];
        if (readRegisters(0x00, buf, 6) != 6) {
            perror("Read registers");
            return {0,0,0};
        }
        // QMC5883L 输出是 little endian
        int16_t x = buf[0] | (buf[1] << 8);
        int16_t y = buf[2] | (buf[3] << 8);
        int16_t z = buf[4] | (buf[5] << 8);
        return {x, y, z};
    }

    double computeHeading() {
        MagData mag = readRaw();
        double heading = std::atan2(static_cast<double>(mag.y), static_cast<double>(mag.x));
        if (heading < 0) heading += 2*M_PI;
        return heading * 180.0 / M_PI; // 转换为度
    }

private:
    int fd;
    uint8_t i2cAddr;

    void writeRegister(uint8_t reg, uint8_t val) {
        uint8_t buf[2] = {reg, val};
        if (write(fd, buf, 2) != 2) {
            perror("Write register");
        }
    }

    int readRegisters(uint8_t reg, uint8_t* buf, int len) {
        if (write(fd, &reg, 1) != 1) {
            perror("Write reg for read");
            return -1;
        }
        return read(fd, buf, len);
    }
};

int main() {
    QMC5883L compass("/dev/i2c-1", 0x0D);

    while (true) {
        double heading = compass.computeHeading();
        std::cout << "Heading: " << heading << " deg" << std::endl;
        usleep(200000); // 200ms
    }

    return 0;
}

