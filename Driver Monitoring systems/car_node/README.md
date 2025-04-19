# Arduino-projects
# Driver Monitoring System (ESP32 + Firebase)
This project monitors a driver's physiological and behavioral state using an ESP32 board. It collects data from multiple sensors and logs it to Firebase Realtime Database. The system checks for alcohol levels, heart rate, SpO2, and signs of sleepiness.

## Features 
**Alcohol Detection:** Uses an analog alcohol sensor and logs the percentage content.


**Sleep Detection:** IR sensor detects lack of motion indicating sleep.


**SpO2 and Heart Rate Simulation:** Uses random values for demonstration.


**Firebase Integration:** Data is logged in real-time to Firebase Realtime Database.


**GPS Data (Simulated):** Logs longitude and latitude for driver tracking. 

## Requirements
ESP32 board


IR sensor


Alcohol sensor (analog)


WiFi connection


Firebase Realtime Database setup 

## Firebase Data Structure 
    ## Parameter to be measured


alcoholLevel <float>


spo2 <int>


heartRate <int> 


alcohol_message <string> 


sleep_message <string> 


health_message <string>


datetime <string>


 ## Logged data


alcoholLevel <float>


spo2 <int>


heartRate <int> 


alcohol_message <string>


sleep_message <string> 


health_message <string>


datetime <string>


longi <string>


lati <string> 

## How It Works
On every loop, the device checks the WiFi connection.


Every 5 cycles (approximately 5 seconds), it: 


Reads alcohol sensor and interprets the message.


Checks IR sensor for sleep detection.


Generates simulated SpO2 and heart rate values.


Sends all data to Firebase.


Console logs display real-time monitoring and Firebase status.

## Customization
**Alcohol Threshold:** float alcoholThreshold = 20.0; // Set your desired threshold 


**GPS Integration:** Replace simulated GPS values (longi, lati) with actual GPS module values.

## Notes
Ensure Firebase credentials and WiFi configuration are correctly set.


Replace placeholder sensor pins like ALCOHOL_SENSOR_PIN and IR_SENSOR_PIN with actual GPIO numbers.


Uncomment and expand the getupdates() function to fetch Firebase updates if needed.

## License
This project is licensed under the MIT License.
