# eRegulation ESP Microcontroller
#### NOTE: This is a test version. Project under development. 
The application is an integral part of the subject "Designing systems with an Embedded computer - Embedded Systems" at the Master's studies of the Faculty of Electrical Engineering, University of East Sarajevo. The project also includes a mobile application [eRegulation Mobile App](https://github.com/vascabarkapa/eregulation-mobile) and web application [eRegulation Mobile Web](https://github.com/vascabarkapa/eregulation-web).

## Overview
This project implements an IoT device using the ESP8266 microcontroller to regulate temperature, humidity, and light conditions in a controlled environment. The device connects to a MQTT broker to receive commands and sends sensor data periodically. It uses the DHT11 sensor for temperature and humidity measurements and an LDR (Light Dependent Resistor) for light detection. The device can be controlled remotely through MQTT messages.
## Hardware Requirements
- ESP8266 microcontroller
- DHT11 sensor
- LDR (Light Dependent Resistor)
- 3 Relays for controlling heating, cooling, and light
- Power supply
- Resistors and wiring as per the circuit diagram

## Software Dependencies
- ESP8266WiFi library
- PubSubClient library
- WiFiClientSecure library
- DHT library

## Configuration
- Replace `ssid` and `password` with your Wi-Fi credentials.
- Update `mqtt_server`, `mqtt_username`, and `mqtt_password` with MQTT broker details.
- Adjust the pin definitions (`dhtPin`, `ldrPin`, `relayPinH`, `relayPinC`, `relayPinL`) based on your hardware connections.
- Customize the `root_ca` certificate if a secure connection is required.

## MQTT Topics
- `eregulation/arduino`: Commands and messages from the IoT device.
- `eregulation/android`: Commands and messages to/from the Android application.
- `eregulation/web`: Commands and messages to/from a web interface.

## Functionality
1. **Reconnect Function (`reconnect`):** Handles MQTT reconnection logic.
2. **MQTT Callback (`callback`):** Processes incoming MQTT messages and performs corresponding actions.
3. **Sensor Reading Function (`ReadDHTValues`):** Reads temperature and humidity from the DHT11 sensor.
4. **Light Check Function (`CheckLight`):** Reads light intensity using the LDR.
5. **Relay Control Function (`RelayControl`):** Controls the relays based on heating, cooling, and light conditions.
6. **Setup Function (`setup`):** Initializes the hardware, connects to Wi-Fi, sets up MQTT, and initializes sensors.
7. **Loop Function (`loop`):** Main program loop where MQTT connection is maintained, sensor readings are taken, and control logic is executed.

## MQTT Commands
- `t-on`: Enable temperature regulation.
- `t-off`: Disable temperature regulation.
- `h-on`: Enable humidity regulation.
- `h-off`: Disable humidity regulation.
- `tnn-mm`: Set temperature range (nn: minimum, mm: maximum).
- `hnn-mm`: Set humidity range (nn: minimum, mm: maximum).
- `l-on`: Turn on the light.
- `l-off`: Turn off the light.
- `l-auto`: Set light mode to auto.
- `welcome`: Send device information to Android application.
- `ping`: Send periodic status updates.

## Note
- Make sure to secure your MQTT connection using SSL certificates if required.
- Adjust the relay control logic and sensor thresholds based on your specific use case.

## Customization
- Modify the relay control logic in the `RelayControl` function to suit your specific hardware configuration.
- Adjust the temperature and humidity threshold values (`min_temp`, `max_temp`, `min_hum`, `max_hum`) based on your environmental requirements.
- Update the MQTT topics and commands according to your application's needs.

## Security Considerations
- Ensure the MQTT broker's security settings align with the device's credentials (`mqtt_username` and `mqtt_password`).
- If security is a concern, enable SSL/TLS by uncommenting the line `wifiClient.setCACert(root_ca);` and providing the appropriate root certificate.

## Acknowledgments

- [Arduino](https://www.arduino.cc/)
- [ESP8266WiFi Library](https://github.com/esp8266/Arduino)
- [PubSubClient Library](https://github.com/knolleary/pubsubclient)
- [DHT Library](https://github.com/adafruit/DHT-sensor-library)

## License

This project is licensed under the [MIT License](LICENSE). You can find more details in the [LICENSE](LICENSE) file.
