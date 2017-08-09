# Diploma thesis: ZX Spectrum Emulator with an Integrated Debugger

This is source code repository for my diploma thesis implementing a multi-platform ZX Spectrum Emulator.

## Features
### Emulator
* Uses [z80ex library](https://github.com/mkoloberdin/z80ex) for emulation of Z80 CPU
* Multilingual user interface based on [wxWidgets 2.8](http://docs.wxwidgets.org/2.8)
* Sound (using [Portaudio](http://www.portaudio.com/docs.html))
* Keyboard and joystick support
* Loading different snapshot and tape images (using [libspectrum](http://fuse-emulator.sourceforge.net/libspectrum.php))

### Debugger
* Step debugging
* Breakpoints
* Z80 byte code decoder (using [z80ex library](https://github.com/mkoloberdin/z80ex))
* Memory and Z80 registers editor

## Screenshots
![Emulator screen](https://user-images.githubusercontent.com/885946/29118845-ff517868-7d03-11e7-8b7c-a0859e1cc7ea.png)
![Debugger - Registers](https://user-images.githubusercontent.com/885946/29119047-d756e69e-7d04-11e7-9d45-10385bc2be96.png)

![Debugger - Memory](https://user-images.githubusercontent.com/885946/29119066-ed0866b6-7d04-11e7-809f-c5233c1b6631.png)
![Debugger - Code](https://user-images.githubusercontent.com/885946/29118755-97fa955a-7d03-11e7-8872-70f5143a3440.png)

## Building
See [BUILD](BUILD.md).
