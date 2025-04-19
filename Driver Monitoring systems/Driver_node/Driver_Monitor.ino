#include <WiFi.h>
#include <Wire.h>
#include <Firebase_ESP_Client.h>
#include "time.h"
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
String dateTime;

#define USER_EMAIL "theravabot@gmail.com"
#define USER_PASSWORD "1122334455"
#define API_KEY "AIzaSyAmhaIhOWWlPYceSdtJScVIIuZP0Rb71hM"
#define DATABASE_URL "https://project-iot-51262-default-rtdb.firebaseio.com/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
String uid;
int updates = 1;
int settings = 0;
String duid = "uniib_elect";
String databasePath = duid + "/driver_monitor/";
String parentPath;
FirebaseJson json;

const char* ntpServer = "pool.ntp.org";
const char* Ssid = "DRIVER MONITOR";
const char* Password = "1122334455";

int lunch = 1;
int setting;
unsigned long Ptime = 0;
unsigned long Ctime = 0;
unsigned long runtime = 0;
int data_count;
float alcoholLevel;
int heartRate = 74;
int spo2 = 96;
const float alcoholThreshold = 50.0;
const int sleepThreshold = 300;
#define HR_THRESHOLD 120
#define SPO2_THRESHOLD 90

String alcohol_message, sleep_message, health_message, longi, lati;


const int ALCOHOL_SENSOR_PIN = 32;
const int IR_SENSOR_PIN = 33;

int count;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  //  analogSetAttenuation(ADC_11db);
  pinMode(ALCOHOL_SENSOR_PIN, INPUT);
  pinMode(IR_SENSOR_PIN, INPUT);

  connectToWiFi();
  configTime(3600, 0, ntpServer);

  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);
  config.token_status_callback = tokenStatusCallback;
  config.max_token_generation_retry = 5;

  Firebase.begin(&config, &auth);

  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);
  health_message = "---";
}

//String getTimestamp() {
//  time_t now = time(nullptr);
//  struct tm* timeinfo = localtime(&now);
//  char timeString[20];
//  strftime(timeString, sizeof(timeString), "%Y%m%d%H%M%S", timeinfo);
//  return String(timeString);
//}

void connectToWiFi() {
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to ");
  Serial.println(Ssid);
  WiFi.begin(Ssid, Password);
  delay(1000);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


String generateRandomKey(int length) {
  String key = "";
  const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
  int charsetSize = sizeof(charset) - 1;

  for (int i = 0; i < length; i++) {
    key += charset[random(charsetSize)];
  }

  return key;
}

void getUpdates() {
  databasePath = duid + "/driver_monitor/esp";
  if (Firebase.ready()) {
    if (Firebase.RTDB.getJSON(&fbdo, databasePath)) {
      FirebaseJson json = fbdo.jsonObject();  // Retrieve JSON object
      // Create FirebaseJsonData to hold retrieved data
      FirebaseJsonData jsonData;

      //      String alcohol_message, sleep_message, health_message;

      json.get(jsonData, "longi");
      longi = jsonData.stringValue;

      json.get(jsonData, "lati");
      lati = jsonData.stringValue;
    }
  }
}


void server() {

  getUpdates();
  if (Firebase.ready()) {

    time_t now = time(nullptr);
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    char timeBuffer[30];
    strftime(timeBuffer, sizeof(timeBuffer), "%Y%m%d%H%M%S", &timeinfo);
    String datetimestamp = String(timeBuffer); // Convert to String


    //    String Dates =  generateRandomKey(5);

    Serial.println(datetimestamp);
    databasePath = duid + "/driver_monitor/esp";
    Firebase.RTDB.setInt(&fbdo, databasePath + "/alcoholLevel", alcoholLevel);
    Firebase.RTDB.setInt(&fbdo, databasePath + "/spo2", spo2);
    Firebase.RTDB.setInt(&fbdo, databasePath + "/heartRate", heartRate);
    Firebase.RTDB.setString(&fbdo, databasePath + "/alcohol_message", alcohol_message);
    Firebase.RTDB.setString(&fbdo, databasePath + "/sleep_message", sleep_message);
    Firebase.RTDB.setString(&fbdo, databasePath + "/health_message", health_message);
    Firebase.RTDB.setString(&fbdo, databasePath + "/datetime", datetimestamp);


    databasePath = duid + "/driver_monitor/";
    String logPath = databasePath + "/logs/" + datetimestamp;
    //    Serial.println(logPath);
    Firebase.RTDB.setInt(&fbdo, logPath + "/alcoholLevel", alcoholLevel);
    Firebase.RTDB.setInt(&fbdo, logPath + "/spo2", spo2);
    Firebase.RTDB.setInt(&fbdo, logPath + "/heartRate", heartRate);
    Firebase.RTDB.setString(&fbdo, logPath + "/alcohol_message", alcohol_message);
    Firebase.RTDB.setString(&fbdo, logPath + "/sleep_message", sleep_message);
    Firebase.RTDB.setString(&fbdo, logPath + "/health_message", health_message);
    Firebase.RTDB.setString(&fbdo, logPath + "/datetime", datetimestamp);
    Firebase.RTDB.setString(&fbdo, logPath + "/longi", lati);
    Firebase.RTDB.setString(&fbdo, logPath + "/lati", longi);

    Serial.println("Data Logged Successfully at " + datetimestamp);
  }
}
//
//void getupdates() {
//  if (Firebase.ready()) {
//    if (Firebase.RTDB.getJSON(&fbdo, databasePath)) {
//      FirebaseJson json = fbdo.jsonObject();  // Retrieve JSON object
//      // Create FirebaseJsonData to hold retrieved data
//      FirebaseJsonData jsonData;
//
//
//      json.get(jsonData, "settings");
//      updates = jsonData.intValue;
//    }
//  }
//}
//

void loop() {

  if (!WiFi.isConnected()) {
    connectToWiFi();
  }
  count++;

  if (count >= 5) {
    //    Ptime = currentMillis;
    //    runtime = (currentMillis - Ptime) / 1000;
    getSleepDetection();
    getAlcoholReading();
    spo2 = random(97, 100);
    heartRate = random(64, 84);
    server();
    Serial.println("DATA LOGGED");
    count = 0;
  }
  Serial.println("RUNNING");
  delay(1000);
}

void getAlcoholReading() {
  float alcoholContent = analogRead(ALCOHOL_SENSOR_PIN);
  alcoholLevel = (alcoholContent / 4095.0) * 100.0;
  Serial.printf("Alcohol Level: %.2f%%\n", alcoholLevel);

  if (alcoholLevel > alcoholThreshold) {
    alcohol_message = "Alcohol Threshold Exceeded";
  } else {
    alcohol_message = "Normal";
  }
  Serial.println(alcohol_message);
}

void getSleepDetection() {
  int sensorVal = digitalRead(IR_SENSOR_PIN);
  bool sleepDetected = (sensorVal == LOW);
  Serial.print("Is sleep detected?: ");
  //  Serial.println(sleepDetected ? "yes" : "no");

  if (sleepDetected) {
    sleep_message = "Sleep Detected";
  } else {
    sleep_message = "Normal";
  }
  Serial.println(sleep_message);
}
