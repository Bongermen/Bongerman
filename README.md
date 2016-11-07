Bongerman
=========
Bongerman is a bare-metal clone of the classic arcade game *Bomberman* designed to run on a Raspberry Pi. Developed in three weeks as part of a first year group project at Imperial College London.

Features
--------
* Custom kernel
* Graphics library
* Fully custom-made assets
* Two player multiplayer
* Two enemy players controlled by artificial intelligence
* Atari 2600 joystick input
* Sound using DMA
* Randomly generated maps

In Action
------------
We never really thought to film any gameplay once we finished; we were too busy playing. Thankfully Tom Bellingham was able to capture some footage before we dissassembled the hardware:

[![youtube video thumbnail](/README/images/video-thumbnail.png)](https://youtu.be/pQvIsv2j6D8)

*Note: Red and White are the AIs*

Instructions
------------
- Create a `build` directory and run `make` using the provided *Makefile*. This will generate a `kernel.img` file that contains the compiled game.

        $ mkdir build
        $ make
- Follow the official Raspberry Pi [installation guide](https://www.raspberrypi.org/documentation/installation/installing-images/) to install Raspbian onto an SD card.
- Once installed, replace the `kernel.img` file with the one generated in step one.

Input
-----
Bongerman was developed using a Raspberry Pi model B. This is the associated GPIO pinout:

![Raspberry Pi Model B Pinout](/README/images/pi-pinout.png)

The game is designed to work using two Atari 2600 controllers with a classic 9-pin connector.

![Atari 2600 Pinout](/README/images/atari-pinout.jpg)

The controllers should be wired according to the following table:

| Controller pin | GPIO pin |
|:--------------|:---------:|
|CONTROLLER ONE TRIGGER|2|
|CONTROLLER ONE UP|3|
|CONTROLLER ONE DOWN|4|
|CONTROLLER ONE LEFT|7|
|CONTROLLER ONE RIGHT|8|
|CONTROLLER TWO TRIGGER|9|
|CONTROLLER TWO UP|10|
|CONTROLLER TWO DOWN|11|
|CONTROLLER TWO LEFT|14|
|CONTROLLER TWO RIGHT|15|

The 5V pin and Ground pin on both controllers should be connected to their respective pins on the Raspberry Pi.

Credits
-------
* Daniel Dean
     - Email: daniel.dean15@imperial.ac.uk
     - Github: [@danrobdean](https://github.com/danrobdean)
* Virain (Nik) Gupta
     - Email: virain.gupta15@imperial.ac.uk
     - Github: [@neprune](https://github.com/neprune)
* Rishabh Jain
     - Email: rishabh.jain15@imperial.ac.uk
     - Github: [@wakeuprj] (https://github.com/wakeuprj)
* Han Thi Nguyen
     - Email: han.nguyen15@imperial.ac.uk
     - Github: [@Sunakarus](https://github.com/Sunakarus)

Special thanks
--------------
* Enrico Manfredi, for wiring assistance
* [PiFox](https://github.com/ICTeam28/PiFox)
