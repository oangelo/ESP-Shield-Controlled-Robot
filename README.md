# ESP-Shield-Controlled-Robot

## BOM (bill of materials)

* NodeMCU (ESP8266)
* ESP motor shield
* Robot structure with at least one DC motor on each side
* Batteries and a battery holder to power the shield

## Installation 

1. Install the ESP8266 board
2. Build and upload the code from the terminal using the following command: `arduino --upload robot.ino --port /dev/ttyUSB*`
3. Use [ESP8266fs](https://github.com/esp8266/arduino-esp8266fs-plugin) to upload the HTML file located in the `data` folder to the ESP.

## Usage

1. Disable any 4G connection.
2. Connect to the Wi-Fi network `robot`.
3. Open the URL `http://192.168.4.1`.
4. Use the on-screen buttons to control the robot.

