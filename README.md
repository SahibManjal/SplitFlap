[![License](https://img.shields.io/github/license/SahibManjal/SplitFlap?color=darkgreen&labelColor=grey&style=flat-circle)](https://github.com/SahibManjal/SplitFlap/blob/readme/LICENSE)
# SplitFlap

## Setup

### Prerequisites

1. Arduino toolchain (for instance, the official [IDE][arduino-ide] or [CLI][arduino-cli]) and the [ArduinoJSON][arduino-json] and [ArduinoHttpClient][arduino-httpclient] packages
2. ESP32 microcontroller
3. Destination and stop pattern split-flap displays
3. Circuitry connecting the ESP32 to the split-flap displays

### ESP32 Configuration

Fill in the `flippers` array in [`Flipper_Config.cpp`](ESP32/Flipper_Config.cpp) (look in [`Flipper_Config.h`](ESP32/Flipper_Config.h) for type `Flipper`):
```cpp
Flipper flippers[FLIPPER_AMOUNT] = {
  {DESTINATION, <in1>, <in2>, <enable>, <home>, <flipState>, <flipAmount>, <flapPosition>},
  {STOP_PATTERN, <in1>, <in2>, <enable>, <home>, <flipState>, <flipAmount>, <flapPosition>},
};
```
Each entry corresponds to a split-flap display and includes its `FlipperType` and the GPIO pins on the ESP32 responsible for controlling it.

There are five possible split-flap types: 
- `DESTINATION` displays the last stop of a train
- `STOP_PATTERN` indicates all stations a train stops at
- `HOUR` shows the hour of trains departure
- `TENS_MINUTE` and `ONE_MINUTE` show the tens and ones digit of the minute of a trains departure

**REQUIRED**: the `flippers` array must include a `DESTINATION` and `STOP_PATTERN` split-flap!

There are four pins used to control each split-flap display:
- `in1` and `in2` are for rotating the motor
- `enable` is brought `HIGH` to allow the motor to move
- `home` is brought `HIGH` when the split-flap is at the "home" position

Set the `FLIPPER_AMOUNT` and `LATCH_TIME` macros in [`Flipper_Config.h`](ESP32/Flipper_Config.h):
```cpp
// Milliseconds between consecutive flips for a single split-flap display.
// REQUIRED: must be >= 75 to function properly
#define LATCH_TIME -1
// the number of entries in the flippers array
#define FLIPPER_AMOUNT -1
```
To achieve speeds similar to <flipper.gif>, set `LATCH_TIME=120`.

Fill in your WIFI information and server IP address in [`Network_Config.h`](ESP32/Network_Config.h):
```cpp
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define SERVER_ADDRESS ""
```

### Timetable

A *timetable* is a schedule that dictates which flap the split-flaps display at a given time. In our case, it's a JSON array of the form
```json
[
  {
    "location": "Blank",
    "hour": 5,
    "minutes": 8,
    "destinationFlap": 0,
    "stopFlap": 0
  },
  {
    "location": "L-Haibara",
    "hour": 5,
    "minutes": 38,
    "destinationFlap": 31,
    "stopFlap": 39,
    "comment": "Actual end is Yamato-Asakura"
  },
  ...
]
```
Each entry is a train stopping at the station, whose details are displayed on the split-flap displays.
- `location` is the final destination
- `hour` and `minute` indicate the departure time
- `destinationFlap` and `stopFlap` indicate the flap position for the `DESTINATION` and `STOP_PATTERN` split-flaps respectively

More specifically, timetables should match this [schema](Server/timetable/schema.json) and entries must satisfy the following constraints:
- `destinationFlap` and `stopFlap` indicate flap positions via the number of flips from their "home" flap
- `hour` must be between 0 and 23
- `minutes` must be between 0 and 59

In [`Server/timetable/`](Server/timetable/), create two timetables for weekdays and weekends/holidays called `weekday.json` and `weekend.json` respectively.

### Install

Clone this repository on a server of your choice (RPi, AWS, locally) and run [`start.sh`](Server/start.sh). Then upload the files in [`ESP32/`](ESP32) on to the ESP32.

## Previous Versions

**WARNING**: parts of the setup instructions no longer apply. You should still be able to read the code to fill in the appropriate details. 

- Version [c2916b6](https://github.com/SahibManjal/SplitFlap/tree/c2916b6ef02799e14cf0fc13f984a5d2cce181c2) for no server

## Roadmap

- [x] Make split-flap display flip
- [x] Follow a hardcoded schedule
- [x] Migrate code from Arduino to ESP
- [x] Support multiple split-flap displays
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

## Contact

Encountered a bug you'd like to report? Fill out a GitHub issue. Need help/support? Have an idea for a new feature? Make a GitHub Discussion.

[kintetsu-timetable]: https://eki.kintetsu.co.jp/english/T1
[kintetsu-livetracking]: https://tid.kintetsu.co.jp/LocationWeb/
[splitflap-blog]: https://web.archive.org/web/20230327031600/https://ohararp.com/split-flap-reverse-engineering-part-1/
[japan-holidays]: https://www.timeanddate.com/holidays/japan/
[arduino-ide]: https://docs.arduino.cc/software/ide/
[arduino-cli]: https://docs.arduino.cc/arduino-cli/
[arduino-json]: https://arduinojson.org/
[arduino-httpclient]: https://docs.arduino.cc/libraries/arduinohttpclient/