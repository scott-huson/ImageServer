# Image Server
### An Open Source Project for the NASA/JPL LiveView (Open Source Edition)

This repository contains the image server for the re-designed, open source release of LiveView, an imaging spectrometer data visualization program. This repository is meant as an optional component to the Open Source LiveView version, enabling the use of data streaming from network connected sources. By using the "Remote Camera" option during LiveView startup, it is possible to connect to a server at a desired IP and stream image data from a server running at that address. 

This image server is compatible with MacOS and Linux. Additionally, this image server is intended to be built for embedded Yocto Linux systems, where it can stream data to a LiveView client on a different system. 

## LiveView (Open Source Version)
https://github.com/nasa-jpl/LiveViewOpenSource

## Installation Guide
See the [installation guide](https://github.com/scott-huson/ImageServer/wiki/Installation-Guide) for more information on how to get the Image Server set up. 

This software was developed by [Scott Huson](https://scott.huson.com) at JPL. Should issues persist, contact [him](mailto:scott@huson.com).

## How it works

The Image Server takes frames from a directory and transmits those frames to a client. It utilizes at TCP/IP socket connection to send and receive messages. The console output of the server will display when a frame request is received and the subsequent frame is transmitted. The server has been tested to support up to 60 frames per second of transmission (but the LiveView application tends to be more of the bottleneck on bandwidth usage.
