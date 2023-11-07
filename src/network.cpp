/* ========================================

Function Files
Related to Networking

======================================== */


#include "Network.h"
#include "Data_Communication.h"


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

// Function that gets current epoch time
unsigned long getTime()
{
  timeClient.update();
  unsigned long now = timeClient.getEpochTime();
  return now;
}