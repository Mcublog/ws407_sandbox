# ws407_sandbox

Some code examples and experiments with STM32 and CMake.

![building](/doc/Peek_2022-10-25_21-34.gif)

## Installing toolchain

Download tar.bz2 with toolchain from [Arm site](https://developer.arm.com/downloads/-/gnu-rm)
Unzip it to /usr/share

```bash
# unzip bz2
sudo tar xjf gcc-arm-none-eabi-your-version.bz2 -C /usr/share/
```

```bash
# setting symlink
sudo ln -s /usr/share/gcc-arm-none-eabi-your-version/bin/* /usr/bin/
```

```bash
# installing ncurses for GDB
sudo apt install libncurses5
```

## Using debugger

Project using [J-Link](https://www.segger.com/products/debug-probes/j-link/) for fw downloading and debuging.
You need [install JLink driver and libraries](https://www.segger.com/downloads/jlink/) and add it to system environment PATH.
