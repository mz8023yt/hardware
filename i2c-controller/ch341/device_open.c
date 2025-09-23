#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include "./lib/x64/dynamic/ch341_lib.h"

int main() {
    int fd;

    const char *dev = "/dev/ch34x_pis2";
    fd = CH34xOpenDevice(dev);
    if (fd < 0) {
        printf("err: open ch341 failed\n");
        return -1;
    }
    printf("success: fd=%d\n", fd);

    CH34xCloseDevice(fd);
    return 0;
}
