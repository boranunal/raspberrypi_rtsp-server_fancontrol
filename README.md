# GStreamer RTSP Server & PWM Fan Controller on Raspberry Pi

This project includes an rtsp server for video streaming, a PWM fan controller that adjusts fan speed based on CPU temperature, and a Qt application to display the video stream, CPU temperature and fan speed.

## Features

- **RTSP Server**: Streams video from the Raspberry Pi using GStreamer.
- **PWM Fan Controller**: Monitors CPU temperature and adjusts fan speed accordingly. Implements a TCP server to send CPU temperature and fan speed. 
- **Qt Application**: Displays the RTSP video stream, CPU temperature and fan speed.

## Directory Structure

```
.
├── rtsp/         # GStreamer RTSP server source
├── fancontrol/     # Fan control and socket server
├── piviewer/         # Qt application source
├── README.md
└── ...
```

## Getting Started

### Prerequisites

- Raspberry Pi (tested on Pi 4)
- GStreamer
- Qt
- pigpio library for GPIO control

### Build & Run

#### 1. RTSP Server

```sh
cd rtsp
make
./rtspstream
```

#### 2. PWM Fan Controller

```sh
cd fancontrol
make
sudo ./fan-controller
```

#### 3. Pi Viewer (Qt App)

Please see (https://doc.qt.io/) for Qt installation instructions, and compiling Qt applications with command line interface,

```sh
cd piview
qmake
make
./piview
```

## Usage

- Start the RTSP server and fan controller on the Raspberry Pi.
- Launch the Qt app (change the source code with your specific RTSP URL) on your desktop.

## Issues

- There is no user interface for IP address and port configuration in Qt app. You need to modify the source code to set the RTSP URL and socket server address.

## Acknowledgements

- [GStreamer](https://gstreamer.freedesktop.org/)
- [Qt](https://www.qt.io/)
- [pigpio](http://abyz.me.uk/rpi/pigpio/)