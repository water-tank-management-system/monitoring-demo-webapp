/* ========================================

MONITORING WEB-APPS
Firebase Project

======================================== */


// ===== Import Necessary Library/Packages =====

#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"


// ===== User-Defined Constant =====

// Insert your network credentials
#define WIFI_SSID "Muafi"
#define WIFI_PASSWORD "pwnya11ya"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCCac6HO8_6z6UloSV_YNJwGE-iJjIlrQM"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "nmuafi1@gmail.com"
#define USER_PASSWORD "naufal153"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://monitoring-web-app-8d4fd-default-rtdb.asia-southeast1.firebasedatabase.app/"

// Define Data Communication State
#define ESP_TX D1
#define ESP_RX D2
#define RTDB_TX 200
#define RTDB_RX 202


// ===== User-Defined Object =====

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Create Software Serial Object
SoftwareSerial espSS(ESP_RX, ESP_TX);


// ===== User-Defined Variables =====

// create counter
int count = 0;

int randNum = 0;

// Data Parameters
float temperature = 0;
float water_level = 0;
float water_flow = 0;
int   turbidity = 0;
bool  automation = false;
bool  dirty_state = false;
bool  empty_state = false;

// Variable to save USER UID
String uid;

// Variables to save database paths
String databasePath;
String tmpPath;
String lvlPath;
String flwPath;
String turPath;
String autPath;
String dirPath;
String empPath;

// Serial Data Communication Variables
bool  auto_mode = true,
      cleaning_state_user = false,
      fill_state_user = false;

int state = ESP_RX;

String  dataIn,
        temp,
        level,
        turb,
        flow,
        dirty,
        empty;

uint8_t indexOfA,
        indexOfB,
        indexOfC,
        indexOfD,
        indexOfE,
        indexOfF;

// Timer variables
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 60000; // send new readings every minute

// timer for LED
unsigned long prev = 0;


// ===== User-Defined Functions Declarations =====

// Initialize WiFi Connection
void initWiFi();

// Initialize Firebase Configuration
void initFirebase();

// Write Float values to the database
void sendFloat(String path, float value);

// Write Int values to the database
void sendInt(String path, int value);

// Write Boolean values to the database
void sendBool(String path, bool value);

// Receive Boolean values from the database
bool receiveBool(String path);

// Parse the Data from master
void parsingData();

// Create function to state the LED
void stateLED(const long interval);


// ===== Let's Go! =====

void setup()
{
  // Baudrate for ESP-PC Communication
  Serial.begin(9600);

  // Baudrate for ESP - Arduino Communication
  espSS.begin(9600);

  // init Random seed for random number
  randomSeed(analogRead(0));

  // BUILTIN_LED initialize
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);

  // Initialize the WiFi Connection
  initWiFi();

  // Initialize the Firebase 
  initFirebase();
}

void loop()
{
  switch(state)    
  {
    case ESP_RX:
      while (espSS.available() > 0)
      {
        dataIn = espSS.readStringUntil('\n');
        parsingData();
        Serial.println("Data recieved");
        Serial.println("temp = " + temp);
        Serial.println("level = " + level);
        Serial.println("turb = " + turb);
        Serial.println("flow = " + flow);
        Serial.println("dirty = " + dirty);
        Serial.println("empty = " + empty);
        Serial.println("=====================");

        state = ESP_TX;
      }
      
      break;
    
    case RTDB_TX:
      // Send new readings to database
      if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0))
      {
        sendDataPrevMillis = millis();

        temperature = temp.toFloat();
        water_level = level.toFloat();
        water_flow = flow.toFloat();
        turbidity = turb.toFloat();
        dirty_state = dirty.toInt();
        empty_state = empty.toInt();

        // Send values to database:
        sendFloat(tmpPath, temperature);
        sendFloat(lvlPath, water_level);
        sendFloat(flwPath, water_flow);
        sendInt(turPath, turbidity);
        sendBool(dirPath, dirty_state);
        sendBool(empPath, empty_state);

        stateLED(2000);
      }

      count++;
      state = RTDB_RX;
      break;
    
    case RTDB_RX:
      Serial.println("Data from RTDB Received");
      break;
    
    case ESP_TX:
      Serial.println("Data transmitted: ");
      espSS.print(auto_mode); espSS.print("A");
      Serial.println(auto_mode);

      espSS.print(cleaning_state_user); espSS.print("B");
      Serial.println(cleaning_state_user);

      espSS.print(fill_state_user); espSS.print("C");
      Serial.println(fill_state_user);

      espSS.print("\n");
      Serial.println("=====================");
      delay(1000);

      state = ESP_RX;
      break;
  }
}


// ===== Body of User-Defined Function =====

// Initialize WiFi Connection
void initWiFi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Initialize Firebase Configuration
void initFirebase()
{
  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "")
  {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid;

  // Update database path for sensor readings
  tmpPath = databasePath + "/temperature"; // --> UsersData/<user_uid>/temperature
  turPath = databasePath + "/turbidity";   // --> UsersData/<user_uid>/turbidity
  lvlPath = databasePath + "/water-level"; // --> UsersData/<user_uid>/water-level
  flwPath = databasePath + "/water-flow";  // --> UsersData/<user_uid>/water-flow
  autPath = databasePath + "/automation";  // --> UsersData/<user_uid>/automation
  dirPath = databasePath + "/dirty-state"; // --> UsersData/<user_uid>/dirty-state
  empPath = databasePath + "/empty-state"; // --> UsersData/<user_uid>/empty-state
}

// Write Float values to the database
void sendFloat(String path, float value)
{
  if (Firebase.RTDB.setFloat(&fbdo, path.c_str(), value))
  {
    Serial.print("Writing value: ");
    Serial.print(value);
    Serial.print(" on the following path: ");
    Serial.println(path);
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}

// Write Int values to the database
void sendInt(String path, int value)
{
  if (Firebase.RTDB.setInt(&fbdo, path.c_str(), value))
  {
    Serial.print("Writing value: ");
    Serial.print(value);
    Serial.print(" on the following path: ");
    Serial.println(path);
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}

// Write Boolean values to the database
void sendBool(String path, bool value)
{
  if (Firebase.RTDB.setBool(&fbdo, path.c_str(), value))
  {
    Serial.print("Writing value: ");
    Serial.print(value);
    Serial.print(" on the following path: ");
    Serial.println(path);
    Serial.println("PASSED");
    Serial.println("PATH: " + fbdo.dataPath());
    Serial.println("TYPE: " + fbdo.dataType());
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + fbdo.errorReason());
  }
}

// Receive Boolean values from the database
bool receiveBool(String path)
{
  if (Firebase.RTDB.getBool(&fbdo, path.c_str()))
  {
    if (fbdo.dataType() == "bool")
    {
      int val = fbdo.boolData();

      return val;
    }
  }
  else
  {
    Serial.println(fbdo.errorReason());
  }
}

// Parse the Data from master
void parsingData()
{
  indexOfA = dataIn.indexOf("A");
  indexOfB = dataIn.indexOf("B");
  indexOfC = dataIn.indexOf("C");
  indexOfD = dataIn.indexOf("D");
  indexOfE = dataIn.indexOf("E");
  indexOfF = dataIn.indexOf("F");

  temp  = dataIn.substring(0, indexOfA);
  level = dataIn.substring(indexOfA + 1, indexOfB);
  turb  = dataIn.substring(indexOfB + 1, indexOfC);
  flow  = dataIn.substring(indexOfC + 1, indexOfD);
  dirty = dataIn.substring(indexOfD + 1, indexOfE);
  empty = dataIn.substring(indexOfE + 1, indexOfF);
}

// Create function to state the LED
void stateLED(const long interval)
{
  unsigned long curr = millis();

  if (curr - prev >= interval)
  {
    prev = curr;

    if (digitalRead(BUILTIN_LED) == LOW)
      digitalWrite(BUILTIN_LED, HIGH);
    else
      digitalWrite(BUILTIN_LED, LOW);
  }
}