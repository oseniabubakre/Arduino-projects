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
String databasePath = duid + "/driver_monitor/esp/";
String parentPath;
FirebaseJson json;

const char* ntpServer = "pool.ntp.org";
const char* Ssid = "DRIVER MONITOR";
const char* Password = "1122334455";


float alcoholLevel;
int heartRate = 74;
int spo2 = 96;

String alcohol_message, sleep_message, health_message, longi, lati;
int count;

void setup() {
  Serial.begin(115200);
  //  analogSetAttenuation(ADC_11db);

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

  longi = "2.33432";
  lati = "3.22322";
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

void server() {
    getUpdates();
  if (Firebase.ready()) {
    Firebase.RTDB.setString(&fbdo, databasePath + "/longi", longi);
    Firebase.RTDB.setString(&fbdo, databasePath + "/lati", lati);
    Serial.println("Data Logged Successfully");
  }
}

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

void getUpdates() {
  if (Firebase.ready()) {
    if (Firebase.RTDB.getJSON(&fbdo, databasePath)) {
      FirebaseJson json = fbdo.jsonObject();  // Retrieve JSON object
      // Create FirebaseJsonData to hold retrieved data
      FirebaseJsonData jsonData;

      //      String alcohol_message, sleep_message, health_message;

      json.get(jsonData, "alcoholLevel");
      alcoholLevel = jsonData.intValue;
      
      json.get(jsonData, "heartRate");
      heartRate = jsonData.intValue;

      json.get(jsonData, "spo2");
      spo2 = jsonData.intValue;

      json.get(jsonData, "alcohol_message");
      alcohol_message = jsonData.stringValue;

      json.get(jsonData, "sleep_message");
      sleep_message = jsonData.stringValue;

      json.get(jsonData, "health_message");
      health_message = jsonData.stringValue;

      Serial.println(sleep_message);
      Serial.println(health_message);
      Serial.println(alcohol_message);
      Serial.println(heartRate);
      Serial.println(spo2);
      Serial.println(lati);
      Serial.println(longi);

    }
  }
}


void loop() {

  if (!WiFi.isConnected()) {
    connectToWiFi();
  }
  count++;

  if (count >= 5) {//every 5 sec
    server();
    Serial.println("DATA LOGGED");
    count = 0;
  }
  Serial.println("RUNNING");
  delay(1000);
}
