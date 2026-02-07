[![License](https://img.shields.io/github/license/SahibManjal/SplitFlap?color=darkgreen&labelColor=grey&style=flat-circle)](https://github.com/SahibManjal/SplitFlap/blob/readme/LICENSE)
# SplitFlap

About

- small GIF of it flipping
- why we're doing this and what it does

## Getting Started

### Installation

- Install the Arduino IDE

- Arduino packages
  - ArduinoJSON
  - ArduinoHttpClient

- Setup ESP32

- Setup RPI

### Configuration
- Configs//Hardware Interface
- remember the file names!!!! (or maybe the code should be changed to take them in)

## Roadmap

- [x] Make split flap display flip
- [x] Follow a hardcoded schedule
- [x] Migrate code from Arduino to ESP
- [x] Support multiple split flap displays
  - [x] destination 
  - [x] stopping pattern 
  - [x] time
- [x] Setup server for selecting timetable
- [x] Setup ESP32 communication with server
- [ ] Webscrape for live delays and cancelations
- [ ] Fix initial index of motor mismatch
- [ ] Configure code for control box

## References
1. [Kintetsu Corporation Timetables][kintetsu-timetable]
2. [Kintetsu Corporation Livetracking][kintetsu-livetracking]
3. [Helpful SplitFlap Reverse Engineering Blog Post][splitflap-blog]
4. [Japanese Holidays][japan-holidays]

[kintetsu-timetable]: https://eki.kintetsu.co.jp/english/T1
[kintetsu-livetracking]: https://tid.kintetsu.co.jp/LocationWeb/
[splitflap-blog]: https://web.archive.org/web/20230327031600/https://ohararp.com/split-flap-reverse-engineering-part-1/
[japan-holidays]: https://www.timeanddate.com/holidays/japan/