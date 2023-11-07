/* ========================================

MONITORING WEB-APPS
Firebase Project

======================================== */


#include <CapstoneA9.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"


// ===== Let's Go! =====

void setup()
{
  // Baudrate for ESP-PC Communication
  Serial.begin(9600);

  // Baudrate for ESP - Arduino Communication
  espSS.begin(9600);  

  // Initialize the timestamp
  timeClient.begin();

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

        state = RTDB_TX;
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
        sendInt(dirPath, dirty_state);
        sendInt(empPath, empty_state);

        stateLED(500, 2);
      }
      
      state = RTDB_RX;
      break;
    
    case RTDB_RX:
      // Receive data from database
      if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0))
      {
        sendDataPrevMillis = millis();

        // Receive values from database:
        auto_mode = receiveBool(autPath);
        cleaning_state_user = receiveBool(clnPath);
        fill_state_user = receiveBool(fllPath);

        stateLED(500, 2);
      }

      state = ESP_TX;
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

  // Update database path for data logging
  dataLogPath = "/UsersData/" + uid + "/systems";

  // Update database path for sensor readings
  tmpPath = databasePath + "/temperature";    // --> UsersData/<user_uid>/temperature
  turPath = databasePath + "/turbidity";      // --> UsersData/<user_uid>/turbidity
  lvlPath = databasePath + "/water-level";    // --> UsersData/<user_uid>/water-level
  flwPath = databasePath + "/water-flow";     // --> UsersData/<user_uid>/water-flow
  dirPath = databasePath + "/dirty-state";    // --> UsersData/<user_uid>/dirty-state
  empPath = databasePath + "/empty-state";    // --> UsersData/<user_uid>/empty-state
  autPath = databasePath + "/automation";     // --> UsersData/<user_uid>/automation
  clnPath = databasePath + "/cleaning-state"; // --> UsersData/<user_uid>/automation
  fllPath = databasePath + "/filling-state";  // --> UsersData/<user_uid>/automation

  sendBool(autPath, true);
  sendBool(clnPath, false);
  sendBool(fllPath, false);
}