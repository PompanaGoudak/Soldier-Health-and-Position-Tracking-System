# Soldier Health and Position Tracking System

## 1. Introduction

The **Soldier Health and Position Tracking System** is an IoT-based project designed to monitor a soldier's basic health information and location.

Soldiers working in border areas, forests, mountains, and other remote locations may face dangerous situations. During emergencies, it can be difficult for the control room to know their health condition and location.

To help address this problem, this project uses an ESP32 microcontroller to collect sensor information, monitor health conditions, and send data to the ThingSpeak cloud platform through Wi-Fi.

The system also includes an SOS emergency button. When the soldier presses this button, an emergency alert is sent through email. An LED and buzzer are also activated to indicate the emergency.

This project is developed and tested using the **Wokwi online simulator**. It is a simulation-based prototype and does not require a physical ESP32 board.

## 2. Objectives

The main objectives of this project are:

- To monitor the soldier's temperature and humidity using a DHT22 sensor.
- To simulate heart rate readings using a potentiometer.
- To simulate the soldier's GPS location using latitude and longitude values.
- To display sensor readings and system status on an LCD.
- To send health and location information to the ThingSpeak cloud platform.
- To provide an SOS emergency button for emergency situations.
- To send an email notification when the SOS button is activated.
- To activate an LED and buzzer during an emergency.
- To demonstrate an IoT-based soldier monitoring system using Wokwi.

## 3. Components Used

| Component | Purpose |
|---|---|
| ESP32 DevKit V4 | Main microcontroller |
| DHT22 Sensor | Measures temperature and humidity |
| Potentiometer | Simulates heart rate input |
| LCD 1602 I2C | Displays sensor readings and status |
| Push Button | Activates and resets SOS mode |
| Red LED | Indicates emergency condition |
| 220Ω Resistor | Limits LED current |
| Buzzer | Produces an emergency sound |
| Wi-Fi | Sends data to the cloud |
| ThingSpeak | Stores and displays sensor data |
| Wokwi | Simulates the complete circuit |

## 4. Software and Technologies Used

- **Arduino IDE / Wokwi:** Used to write and simulate the ESP32 program.
- **Embedded C++:** Used for programming the ESP32.
- **IoT:** Used to transfer sensor information through the internet.
- **Wi-Fi:** Used to connect the ESP32 simulation to the internet.
- **ThingSpeak Cloud:** Used to store and monitor sensor data.
- **ThingSpeak Alerts API:** Used to send SOS email notifications.

## 5. Block Diagram

```text
             DHT22 SENSOR
                  |
                  |
             POTENTIOMETER
                  |
                  |
             SOS PUSH BUTTON
                  |
                  |
                  v
          +------------------+
          |                  |
          |       ESP32      |
          |                  |
          +------------------+
             |     |      |
             |     |      |
             v     v      v
            LCD   LED   BUZZER
             |
             |
             v
          Wi-Fi Connection
             |
             |
             v
       THINGSPEAK CLOUD
             |
             |
       +-----+------+
       |            |
       v            v
  Data Monitoring  SOS Email
                   Notification
```

## 6. Circuit Connections

The following table shows the connections used in the Wokwi simulation.

| Component | ESP32 Pin |
|---|---|
| DHT22 Data | GPIO 15 |
| Potentiometer Signal | GPIO 34 |
| Red LED | GPIO 25 |
| Buzzer Positive | GPIO 26 |
| SOS Push Button | GPIO 27 |
| LCD SDA | GPIO 21 |
| LCD SCL | GPIO 22 |

### Power Connections

- DHT22 VCC is connected to 3.3V.
- DHT22 GND is connected to GND.
- Potentiometer VCC is connected to 3.3V.
- Potentiometer GND is connected to GND.
- LCD VCC is connected to 5V.
- LCD GND is connected to GND.
- The LED is connected through a 220Ω resistor.
- The buzzer negative terminal is connected to GND.
- One terminal of the push button is connected to GPIO 27, and the other is connected to GND.

## 7. Working Principle

The ESP32 is the main controller of this project. It receives input from the connected sensors and processes the information.

### 7.1 Temperature and Humidity Monitoring

The DHT22 sensor measures the surrounding temperature and humidity.

The ESP32 reads these values and displays them on the LCD and Serial Monitor.

If the temperature crosses the programmed limit, the system identifies it as a health alert.

### 7.2 Heart Rate Monitoring

In this simulation, a potentiometer is used to represent heart rate input.

The ESP32 reads the analog value from the potentiometer and converts it into a heart rate value in BPM (Beats Per Minute).

The simulated heart rate is mapped between 60 and 100 BPM.

**Note:** The potentiometer is used only for simulation. It is not an actual heart rate sensor.

### 7.3 GPS Location Monitoring

The project uses latitude and longitude values to represent the soldier's location.

The location values are simulated in the program and can be sent to the ThingSpeak cloud platform.

The initial coordinates used in the program are:

- Latitude: 12.9716
- Longitude: 77.5946

**Note:** This project does not use a physical GPS module. The location values are simulated for demonstration.

### 7.4 LCD Display

The LCD1602 I2C display is used to show important system information.

It displays sensor readings and the current system status, such as normal operation or emergency condition.

### 7.5 Health Alert System

The ESP32 continuously checks the temperature and simulated heart rate.

The programmed health alert conditions are:

- Temperature greater than 38°C.
- Heart rate below 60 BPM.
- Heart rate above 100 BPM.

If any of these conditions occur, the system activates the health alert indication using the LED and buzzer.

These are demonstration thresholds used in the simulation, not medical diagnostic limits.

### 7.6 SOS Emergency System

An SOS push button is provided for emergency situations.

When the soldier presses the button:

1. The ESP32 detects the button press.
2. The SOS status becomes active.
3. The red LED turns ON.
4. The buzzer produces an emergency sound.
5. The emergency status is displayed on the LCD and Serial Monitor.
6. An SOS email notification is requested through the ThingSpeak Alerts API.
7. The SOS status is uploaded to ThingSpeak.

When the button is pressed again, the SOS status is reset.

### 7.7 ThingSpeak Cloud Monitoring

ThingSpeak is an IoT cloud platform used to store and monitor sensor information.

The ESP32 connects to the Wokwi-GUEST Wi-Fi network and sends data to ThingSpeak.

The project uploads information every 20 seconds.

The following six fields are used in the ThingSpeak channel:

| Field | Information |
|---|---|
| Field 1 | Temperature |
| Field 2 | Humidity |
| Field 3 | Heart Rate |
| Field 4 | Latitude |
| Field 5 | Longitude |
| Field 6 | SOS Status |

The SOS status is represented as:

- `0` – SOS is not active.
- `1` – SOS is active.

The uploaded data can be viewed using the ThingSpeak channel dashboard.

### 7.8 SOS Email Notification

The project uses the ThingSpeak Alerts API to request an emergency email notification.

When the SOS button is activated, the ESP32 sends an HTTPS request containing the emergency message and simulated location coordinates.

The email notification helps inform the registered recipient about the emergency.

## 8. ThingSpeak Channel Details

The project uses ThingSpeak to store sensor information.

**Channel Name:** Soldier Health and Position Tracking System

**Channel ID:** 3560788

The channel contains six fields for temperature, humidity, heart rate, latitude, longitude, and SOS status.

Channel dashboard:

https://thingspeak.mathworks.com/channels/3560788/private_show

The channel is private, so access may require permission from the account owner.

## 9. Simulation Procedure

The project can be tested using the Wokwi online simulator.

1. Open the Wokwi project.
2. Start the ESP32 simulation.
3. Check the LCD display.
4. Change the potentiometer value to observe the simulated heart rate.
5. Change the DHT22 temperature and humidity values.
6. Observe the health alert indication when the programmed conditions are reached.
7. Press the SOS button to activate emergency mode.
8. Observe the LED and buzzer.
9. Check the SOS status in ThingSpeak.
10. Check the registered email for the emergency notification.
11. Press the SOS button again to reset the emergency status.

## 10. Advantages

- Provides continuous monitoring of simulated health information.
- Displays important information using an LCD.
- Stores sensor readings in the cloud.
- Provides an SOS emergency alert feature.
- Uses an LED and buzzer for emergency indication.
- Allows remote monitoring through ThingSpeak.
- Can be tested without purchasing physical hardware.
- Demonstrates the use of IoT in soldier safety applications.

## 11. Limitations

- The project is developed as a Wokwi simulation.
- Heart rate is simulated using a potentiometer.
- GPS coordinates are generated by the program and are not received from a real GPS module.
- A physical ESP32 board and sensors are not used in this prototype.
- The system requires an internet connection for cloud updates and email notifications.
- The prototype is for educational demonstration and is not a certified medical or military safety device.

## 12. Future Enhancements

The project can be improved by adding:

- A real heart rate sensor to measure the soldier's pulse.
- A GPS module to obtain actual location coordinates.
- An SpO₂ sensor to monitor blood oxygen levels.
- A body temperature sensor for more suitable body temperature measurement.
- A rechargeable battery for portable operation.
- A mobile application for remote monitoring.
- A control room dashboard to monitor multiple soldiers.
- Improved communication using LoRa for remote areas.
- A real-time alert system for emergency situations.

## 13. Conclusion

The Soldier Health and Position Tracking System demonstrates how IoT technology can be used to monitor health-related information and location data.

The ESP32 collects sensor inputs, processes the information, and sends it to the ThingSpeak cloud platform using Wi-Fi. The LCD displays the system information, while the LED and buzzer provide emergency indications.

The SOS button allows the user to activate an emergency alert, and the ThingSpeak Alerts API is used to request an email notification.

The complete prototype is developed and tested using Wokwi. This project provides a basic understanding of embedded systems, IoT communication, cloud monitoring, and emergency alert systems.

## 14. Project Link

**Wokwi Simulation:**

https://wokwi.com/projects/479387238993497089

**GitHub Repository:**

https://github.com/PompanaGoudak/Soldier-Health-and-Position-Tracking-System

## 15. Author

**Project Title:** Soldier Health and Position Tracking System

**Platform:** ESP32 with Wokwi Simulation

**Cloud Platform:** ThingSpeak

**Programming Language:** Embedded C++

