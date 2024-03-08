# Smart-RoomGuard

The Smart RoomGuard project is a microcontroller-based system designed to enhance room security and provide timely notifications in case of emergencies. This system features an automatic door unlock mechanism for emergency situations, along with notifications sent to a mobile device with a link to view real-time temperature and humidity graphs of the room.

## Components Used

- DHT sensor (DHT22 or DHT11) for temperature and humidity monitoring
- Gas sensor (e.g., MQ135) for detecting hazardous gases
- ESP32 microcontroller for processing data and controlling peripherals
- Buzzer for audible alerts
- Solenoid lock for door security
- Relay for controlling external devices
- Battery for power

## Functionality

The Smart RoomGuard offers the following functionality:

- **Automatic Door Unlock:** In case of an emergency such as a fire detection (detected by the gas sensor), the solenoid lock is automatically disengaged to allow easy exit from the room.
- **Notification System:** Upon detecting an emergency, the system sends a notification to a mobile device via Wi-Fi or GSM (using services like Twilio or Pushbullet). The notification includes details of the emergency along with a link.
- **Real-time Monitoring:** The link provided in the notification leads to a web page displaying real-time temperature and humidity graphs of the room. This allows users to monitor the conditions remotely and take appropriate action.

## Usage

To set up and use the Smart RoomGuard:

1. Connect the DHT sensor, gas sensor, buzzer, solenoid lock, relay, and battery to the ESP32 microcontroller.
2. Upload the provided Arduino code to the ESP32 microcontroller.
3. Configure the notification system to send alerts to your mobile device in case of emergencies.
4. Power on the system and monitor the environment.
5. In case of an emergency, the door will automatically unlock, and a notification will be sent to your mobile device with a link to view real-time temperature and humidity graphs.

## License

This project is licensed under the [License Name]. See the [LICENSE](LICENSE) file for details.

## Acknowledgements

Special thanks to the creators of the libraries and components used in this project, as well as the open-source community for their contributions.
