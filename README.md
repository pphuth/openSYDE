# openSYDE

openSYDE by STW (https://www.stw-mobile-machines.com/) is a software toolchain that supports implementation, commissioning, analysis and maintenance of control systems for mobile machines.

openSYDE stands out with its openness: freely useable interfaces to other applications like requirement engineering tools or the integration of 3rd party devices. Based on the open architecture openSYDE can be flexibly integrated into an existing tool- and process-environment.

With openSYDE efficient realization of applications that need to consider functional safety is guaranteed. 
In addition openSYDE provides the development- and service-teams with a user friendly graphical interface with latest design and shines with a continuous workflow from system definition to maintenance.

Main strong points:

Focus on System

- One development environment for the complete system (single point of source)
- Realization of functionality shared between multiple devices
- Fast realization – consistent workflow for all STW products

Open Source

-	Add your own features
-	Basic logic implemented in free to use ANSI C++ openSYDE Core Library
-	Integrate openSYDE functionality into your existing Service Tool or Linux HMI
- Licence model:
  - openSYDE GUI – GPL V3
    - is based on the openSYDE Core Library.
  -	openSYDE Core – Dual
    -	GPL V3
    -	STW Licence

Open Interface
-	open (human readable) file format
-	Connect openSYDE to your tools and processes solution
-	Import / Export standard file formats (DBC, EDS, DCF, ASC, BLF, …)

Safety ready
-	Realize applications that need to address functional safety requirements. All safety related openSYDE components are certified by TÜV.


## Building for Linux platforms

Target platform for the openSYDE tools is Windows. 

This branch adds basic support for Linux platforms. Currently the opensyde_tool and the opensyde_syde_coder_c 
can be built for Windows and Linux.

**Linux platform support is pretty experimental and untested...**

### Preparations (on Ubuntu 18.04)

* sudo apt-get install git make g++ qt5-default libqt5charts5-dev libqt5svg5-dev libzip-dev

### Building the openSYDE tool

* cd <repo>/opensyde_tool/pjt
* mkdir build
* cd  build
* qmake ../openSYDE.pro
* make -j8


### Building the openSYDE C coder

* cd <repo>/opensyde_syde_coder_c/pjt
* mkdir build
* cd  build
* qmake ../osy_syde_coder_c.pro.pro
* make -j8


### Drawbacks on Linux platforms

* Online help via key F1 is not supported
* Opening an IDE from the tool is not supported
* Configuring the CAN bitrate from the tool is not supported. This is a system-wide setting on Linux.
* Selecting a specific Ethernet interface via config file is not supported. All interfaces of the host are used.
* Detection if running on a Laptop is not supported.
* WinTaskbarProgress is not supported.
