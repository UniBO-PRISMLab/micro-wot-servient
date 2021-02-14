# WoT-microcontroller-servient-gui

> WoT-microcontroller-servient is an implementation of W3C Web of Things Servient for embedded systems. This module allows users to: define W3C WoT Thing Descriptions, generate scripting files (sketches) in Embedded-C programming language executable by microcontrollers to expose Things, compile and flash these sketches.

## How does it work?

This module provides an Electron-made GUI for [WoT-microcontroller-servient project](https://github.com/UniBO-PRISMLab/WoT-microcontroller-servient) by Daniele Rossi. User can provide all the metadata included in W3C WoT Thing Descriptions via forms on GUI and store them in a JSON Thing Description file. 
The project can also build a scripting file including the Thing logic. This scripting file is compiled and flashed into a microcontroller.
The sketch has .ino extension (like Arduino IDE).
This project can also compile the built .ino project and upload to the prototipying board, calling the Servient Builder and showing it to a built-in terminal.

The **only supported** embedded system is Espressif ESP32 (an extra compatibility support is provided for NodeMCU ESP8266)

## Prerequisites
This package is only supported by Linux systems.

**Linux**
- Python v3.6.x
- Nodejs v8.10.0 (tested also with v9.11.2 and v10.21.0)
- npm (tested with version 3.5.x)
- pip3 Python package manager
- A proper C/C++ compiler toolchain, like GCC
- git
- Arduino IDE toolkit (the component used is arduino-cli)
- curl

## Usage

### Install software dependencies
Install needed software from repos:
```shell
sudo apt update && sudo apt upgrade -y
sudo apt install git python3 python3-pip npm nodejs git arduino curl -y
```
Please note that some Arduino packages are outdated if picked from official distros repositories.
We suggest to download and install the latest Arduino-IDE toolkit from the official Arduino website.

### Clone and install
Clone the repository:

```shell
git clone git@github.com:UniBO-PRISMLab/micro-wot-servient.git
```

Go into the repository:

```shell
cd WoT-microcontroller-servient-GUI
```
Install the package and all dependencies:

```shell
sudo pip3 install .
npm install
```

Depending of what you need, you have also to provide Arduino libraries (should be installed in default library directory, typically ```~/Arduino/libraries```)

It is **recomended** to install and use the package inside a virtual environment.

### Start application

The application can be started with the following command:

```shell
npm start
```

### Use with Docker 
With this repository a Dockerfile is provided. This dockerfile generates a new image working with wot.

```shell
cd micro-wot-servient #enter in the main project directory
docker build . -t wot 
```

For make it run (change/add devices you want to upload to. In this example /dev/ttyUSB0 is used):
```shell
docker run --net=host --env="DISPLAY" --volume="$HOME/.Xauthority:/root/.Xauthority:rw" --user=$(id -u $USER):$(id -g $USER) --device=/dev/dri:/dev/dri --device=/dev/ttyUSB0 wot
```

### Implemented/supported features

**Scripting API**

- RESTful API :heavy_check_mark:

**Protocol Support**

- HTTP :heavy_check_mark:
- WebSocket :heavy_check_mark:
- CoAP :heavy_check_mark:

**MediaType Support**

- JSON :heavy_check_mark: 
- Plain Text :heavy_check_mark: 

## License
WoT-microcontroller-servient-GUI is released under the MIT License.
