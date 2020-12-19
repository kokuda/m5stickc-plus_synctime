# M5StickC-Plus Clock Synchronizer

`Connects to local Wifi and synchronizes the clock on the M5StickC-Plus with the NTP server pool.ntp.org`

## Installation

This project was created with Visual Studio Code and the PlatformIO extension

Runs on the M5StickC-Plus (Should work on M5StickC with small modifications but untested)

## Configuration

Copy config-TEMPLATE.h into config.h and change the WIFI_SSID to your Wifi name and WIFI_PASSWORD to your Wifi password.

The default time zone is UTC-8 (PST) but it can be changed by setting TIME_ZONE_OFFSET_HRS to your timezone offset in hours.

By default it uses the NTP server pool.ntp.org, but this can be configured by changing NTP_SERVER.

## Usage

Build, install, and run on your M5StickC-Plus and it will display the synchronized time.