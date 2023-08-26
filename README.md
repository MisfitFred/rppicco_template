# Raspberry PI pico template project

This project shall be a template project to develop software for raspberry pi pico.

## setup your environment

A dev container is used to build the binaries. To create the container and start a bash inside call

```bash
    ./startDocker.sh
```

To trigger the build, within the dev container

```bash
    cmake -S . -B ./build -G Ninja
    cd build 
```

after the first cmake run

```bash
    ninja 
```

can be used to build the binaries

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

#### Flash target with OpenOCD

```bash
    sudo openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program blink.elf verify reset exit"
```

#### Start  OpenOCD

```bash
    openocd -c "gdb_port 50000" -c "tcl_port 50001" -c "telnet_port 50002" -s /home/matthias/work/pico/pico-examples -f /home/matthias/.vscode-server/extensions/marus25.cortex-debug-1.12.0/support/openocd-helpers.tcl -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000"
```

#### WIP

To be clarified if this is needed
```bash
    sudo apt update
    sudo apt install binutils-multiarch
    cd /usr/bin
    ln -s /usr/bin/objdump objdump-multiarch
    ln -s /usr/bin/nm nm-multiarch 
```

[id1]: https://www.raspberrypi.com/documentation/microcontrollers/debug-probe.html#installing-gdb

#### Grant USB access without root privileges
    
    @todo check code below

    ```bash
        sudo cp 99-rpi-pico.rules /etc/udev/rules.d/
        sudo udevadm control --reload-rules
        sudo udevadm trigger
    ```