/* ========================================

Main Header Files
Capstone A9

======================================== */

#ifndef CapstoneA9_H
#define CapstoneA9_H

// ===== Import Necessary Library/Packages =====

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// ===== User-Defined Constant =====

// Define Data Communication State
#define ESP_TX D1
#define ESP_RX D2
#define RTDB_TX 200
#define RTDB_RX 202


// ===== User-Defined Constant =====

// Insert your network credentials
#define WIFI_SSID "Muafi"
#define WIFI_PASSWORD "pwnya11ya"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCCac6HO8_6z6UloSV_YNJwGE-iJjIlrQM"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "admin@capstonea9.id"
#define USER_PASSWORD "nilaicapstonea"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://monitoring-web-app-8d4fd-default-rtdb.asia-southeast1.firebasedatabase.app/"


// ===== User-Defined Object =====

// Define Firebase objects
extern FirebaseData fbdo;
extern FirebaseAuth auth;
extern FirebaseConfig config;

// Define Firebase JSON objects
extern FirebaseJson json;

// Create Software Serial Object
extern SoftwareSerial espSS;

// Define NTP Client to get time
extern WiFiUDP ntpUDP;
extern NTPClient timeClient;


// ===== User-Defined Variables =====

// ===== Variables to save database paths

extern String   uid,        // Variable to save USER UID
                parentPath; // Parent Node (to be updated in every loop)

// Database main path (to be updated in setup with the user UID)
extern String databasePath;

// Database child nodes
extern String tmpPath,
              lvlPath,
              flwPath,
              turPath,
              dirPath,
              empPath,
              autPath,
              clnPath,
              fllPath;

// Data Logging Variables
extern String   dataLogPath,
                tmpLog,
                lvlLog,
                flwLog,
                turLog,
                timePath;

// Variable to save current epoch time
extern int timestamp;

// ===== Variables Related to System Communication

// System Readings Data Variables
extern float  temperature,
              water_level,
              water_flow;
extern int turbidity;
extern bool automation,
            dirty_state,
            empty_state;

// User State Data Variables
extern bool auto_mode,
            cleaning_state_user,
            fill_state_user;

// Initial state
extern int state;

// Data Transfer Variables
extern String dataIn,
              temp,
              level,
              turb,
              flow,
              dirty,
              empty;

// Data Parsing Variables
extern uint8_t  indexOfA,
                indexOfB,
                indexOfC,
                indexOfD,
                indexOfE,
                indexOfF;

// Timer variables
extern unsigned long  sendDataPrevMillis,
                      timerDelay, // send new readings every minute
                      prev;


// ===== User-Defined Functions Declarations =====

// Create function to state the LED
void stateLED(const long interval, int period);


#endif