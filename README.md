# SplitFlap
## Setup
### Hardware Interface
 - Input1 and Input2 are for rotating the motor and its magnet
 - When enable pin is HIGH, the motor can move 
 - Home pin corresponds to Hall sensor being at Flipper position 0

The byte flipState provides an impulse to an h-bridge to initiate an index out of the motor, consequently the next index of the motor is initiated by an impulse of reverse polarity from the previous.

The byte latchTime is the time needed for the h-bridge impulse go through, this is the minimum time required for it to flip correctly.

### TimeTable Format


## ESP32


## Arduino UNO


## TODO
- [ ] Webscraping for live delays and Equinoxes
- [ ] Raspberry Pi Server Setup
- [ ] ESP32 Communication with Raspberry Pi
- [ ] LCD/LED Button Control
  - [ ] Ignore Timetable and Stay at a Particular Flap
  - [ ] Choosing Timetables

## References
1. Kintetsu Corporation Timetables - https://eki.kintetsu.co.jp/english/T1

2. Helpful SplitFlap Reverse Engineering Blog Post - https://web.archive.org/web/20230327031600/https://ohararp.com/split-flap-reverse-engineering-part-1/
