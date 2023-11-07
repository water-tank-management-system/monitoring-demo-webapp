/* ========================================

Main Header Files
Capstone A9

======================================== */


// ===== Import Necessary Library/Packages =====

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"


// ===== User-Defined Constant =====

// Define Data Communication State
#define ESP_TX D1
#define ESP_RX D2
#define RTDB_TX 200
#define RTDB_RX 202


// ===== User-Defined Variables =====

// ===== Variables to save database paths

String uid;        // Variable to save USER UID
String parentPath; // Parent Node (to be updated in every loop)

// Database main path (to be updated in setup with the user UID)
String databasePath;

// Database child nodes
String tmpPath;
String lvlPath;
String flwPath;
String turPath;
String dirPath;
String empPath;
String autPath;
String clnPath;
String fllPath;

// Data Logging Variables
String dataLogPath;
String tmpLog = "/temperature";
String lvlLog = "/water-level";
String flwLog = "/water-flow";
String turLog = "/turbidity";
String timePath = "/timestamp";

// Variable to save current epoch time
int timestamp;

// ===== Variables Related to System Communication

// System Readings Data Variables
float temperature = 0,
      water_level = 0,
      water_flow = 0;
int   turbidity = 0;
bool  automation = false,
      dirty_state = false,
      empty_state = false;

// User State Data Variables
bool auto_mode = true,
     cleaning_state_user = false,
     fill_state_user = false;

// Initial state
int state = ESP_RX;

// Data Transfer Variables
String  dataIn,
        temp,
        level,
        turb,
        flow,
        dirty,
        empty;

// Data Parsing Variables
uint8_t indexOfA,
        indexOfB,
        indexOfC,
        indexOfD,
        indexOfE,
        indexOfF;

// Timer variables
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 20000; // send new readings every minute

// timer for LED
unsigned long prev = 0;


// ===== User-Defined Functions Declarations =====

// Create function to state the LED
void stateLED(const long interval, int period);
