# 驱动下载

下载 CH341PAR_LINUX_ZIP
https://www.wch.cn/downloads/CH341PAR_LINUX_ZIP.html

# 驱动安装

解压驱动包

```
unzip CH341PAR_LINUX.ZIP
```

编译驱动

```
cd CH341PAR_LINUX/driver/
make -j
```

安装驱动

```
sudo insmod ch34x_pis.ko
```

确认驱动是否安装成功

```
lsmod | grep ch34
ls /dev/ch34*
```

# 硬件连接

检查是否检测到 CH341 设备

```
lsusb | grep CH34
```

# 测试程序

```
make
sudo ./device_open
```
