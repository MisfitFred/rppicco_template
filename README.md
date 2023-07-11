# Raspberry PI picco template project

This project shall be a template project to develop software for raspberry pi picco on an ubuntu 22.04 LTS system.

## setup your environment

### Raspberry Pi Debug Probe

Due to some trouble I had to get the Raspberry Pi Debug Probe to run on Ubuntu 22.04, I decided to write this short manual which describe what I did. Unfortunately this manual was written afterward, I might be the case that I missed some steps. May you find a way to let me know the trouble you had that I get the chance to fix this manual.

#### Installing OpenOCD

The OpenOCD version provided in the Ubuntu 22.04 distribution will not work, so you have to download the source code and build it manually. First of all you've to download some libraries which are essential for the piprobe support.

```bash
    sudo apt install libhidapi-dev libusb-1.0-0 libusb-1.0-0-dev
```

Then you can follow the manual given as [reference][id1]

```bash
    sudo apt install automake autoconf build-essential texinfo libtool libftdi-dev libusb-1.0-0-dev
    git clone https://github.com/raspberrypi/openocd.git --branch rp2040 --depth=1 --no-single-branch
    cd openocd
    ./bootstrap
    ./configure
    make -j4
    sudo make install
```

[id1]: https://www.raspberrypi.com/documentation/microcontrollers/debug-probe.html#installing-gdb