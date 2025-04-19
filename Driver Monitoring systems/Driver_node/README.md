# Arduino-projects
## Driver Monitoring System using ESP32 and Firebase
This project is an **Arduino-based driver monitoring system** using an ESP32. It collects real-time data such as **alcohol level**, **SpO2**, **heart rate**, and **sleep detection**, then logs that data to **Firebase Realtime Database**. 

## Features 
Alcohol Detection via MQ3


Sleep Detection via IR Sensor


Heart Rate and SpO2 Monitoring


Real-time Data Logging to Firebase


Logs organized by timestamp 

## Technologies Used 
ESP32


Firebase Realtime Database


Arduino Framework


Serial Monitor for Debugging

## Project Structure 
The data is saved to Firebase in two places: 


1. **Current status path** 


2. **Logs path (with timestamp)** 


Each log entry includes:


Alcohol Level, SpO2, Heart Rate, Alcohol Message, Sleep Message, Health Message, Latitude & Longitude and Timestamp 

## Functions Overview 
  getAlcoholReading() Reads analog value from the alcohol sensor, converts it to percentage, and sets a status message. 
 
  
  getSleepDetection() Detects if the driver is possibly asleep using a digital IR sensor.  
  
  
  loop() Every few seconds:  Checks WiFi connection - Updates sensor values - Sends data to Firebase 
  
  
  Firebase Data Logging `cpp Firebase.RTDB.setInt(&fbdo, databasePath + "/alcoholLevel", alcoholLevel); Firebase.RTDB.setString(&fbdo, databasePath + "/alcohol_message", alcohol_message); ... 


## Requirements
ESP32 Board


Alcohol Sensor (analog)


IR Sensor (digital)


WiFi Connection


Firebase Realtime Database credentials

## Setup Instructions
Connect your ESP32 to the alcohol and IR sensors.


Set up Firebase and get your database URL and API key.


Add your WiFi credentials and Firebase configs in the Arduino sketch.


Upload the code to the ESP32 using Arduino IDE.


Open the Serial Monitor to view logs.

### Sample Output
  Alcohol Level: 3.21% 

  SPO2(%): 97


  HeartRate (BPM): 77


  Alcohol Message: Normal


  Sleep Message: Normal


  Health Message: ---------


  Longitude: 2.33432


  Latitude: 3.22322
  
  
  Data Logged Successfully at 2024-04-19 15:34:12 


## License
This project is open-source and free to use under the MIT license.
