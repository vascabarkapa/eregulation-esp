# eRegulation ESP Microcontroller
#### NOTE: This is a test version. Project under development. 
The application is an integral part of the subject "Designing systems with an Embedded computer - Embedded Systems" at the Master's studies of the Faculty of Electrical Engineering, University of East Sarajevo. The project also includes a mobile application [eRegulation Mobile App](https://github.com/vascabarkapa/eregulation-mobile) and web application [eRegulation Mobile Web](https://github.com/vascabarkapa/eregulation-web).

This project uses an ESP8266 microcontroller to measure temperature and humidity using a DHT11 sensor and control heating and cooling systems based on predefined thresholds. It communicates with a MQTT broker to receive control commands and send status updates.

## Prerequisites

Before you begin, make sure you have the following components and requirements in place:

- ESP8266 microcontroller
- DHT11 temperature and humidity sensor
- A stable Wi-Fi network
- Access to a MQTT broker server
- Arduino IDE installed with ESP8266 board support

## Installation

1. Clone or download the repository to your local machine.
2. Open the Arduino IDE.
3. Install the necessary libraries:
   - `ESP8266WiFi` - for connecting to Wi-Fi.
   - `PubSubClient` - for MQTT communication.
   - `DHT` - for reading data from the DHT11 sensor.
4. Update the following configuration parameters in the code to match your setup:
   - `ssid` and `password` - your Wi-Fi network credentials.
   - `mqtt_server`, `mqtt_username`, and `mqtt_password` - MQTT broker information.
   - `root_ca` - if you're using a secure connection (HTTPS), add the root certificate here.
   - Adjust temperature and humidity thresholds (`min_temp`, `max_temp`, `min_hum`, `max_hum`) as needed.
5. Upload the code to your ESP8266 device.

## Usage

1. Power up your ESP8266 device.
2. The device will attempt to connect to the Wi-Fi network and the MQTT broker.
3. Once connected, it will subscribe to MQTT topics: `eregulation/arduino` and `eregulation/android`.
4. You can send MQTT messages to control the system:
   - To turn on temperature control, publish a message to `eregulation/arduino` with the content `t-on`.
   - To turn off temperature control, publish a message to `eregulation/arduino` with the content `t-off`.
   - To set custom temperature thresholds, publish a message to `eregulation/arduino` in the format `t-XX-YY`, where `XX` is the minimum temperature and `YY` is the maximum temperature.
   - To turn on humidity control, publish a message to `eregulation/arduino` with the content `h-on`.
   - To turn off humidity control, publish a message to `eregulation/arduino` with the content `h-off`.
   - To set custom humidity thresholds, publish a message to `eregulation/arduino` in the format `h-XX-YY`, where `XX` is the minimum humidity and `YY` is the maximum humidity.
   - To receive the current system status, send a message with the content `welcome` to `eregulation/arduino`.

## Acknowledgments

- [Arduino](https://www.arduino.cc/)
- [ESP8266WiFi Library](https://github.com/esp8266/Arduino)
- [PubSubClient Library](https://github.com/knolleary/pubsubclient)
- [DHT Library](https://github.com/adafruit/DHT-sensor-library)
