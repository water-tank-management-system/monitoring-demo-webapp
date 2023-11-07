/* ========================================

Function Files
Related to Networking

======================================== */


#include "Network.h"


// ===== User-Defined Object =====

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Define Firebase JSON objects
FirebaseJson json;

// Create Software Serial Object
SoftwareSerial espSS(ESP_RX, ESP_TX);

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");


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

// Write Data Log in JSON to the database
void sendDataLog()
{
  // Get current timestamp
  timestamp = getTime();
  Serial.print("time: ");
  Serial.println(timestamp);

  parentPath = dataLogPath + "/" + String(timestamp);

  json.set(tmpLog.c_str(), String(temperature));
  json.set(flwLog.c_str(), String(water_flow));
  json.set(lvlLog.c_str(), String(water_level));
  json.set(turLog.c_str(), String(turbidity));
  json.set(timePath, String(timestamp));

  Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
}

// Receive Boolean values from the database
bool receiveBool(String path)
{
  int val = 0;

  if (Firebase.RTDB.getBool(&fbdo, path.c_str()))
  {
    if (fbdo.dataType() == "bool")
    {
      val = fbdo.boolData();
    }
  }
  else
  {
    Serial.println(fbdo.errorReason());
  }
  return val;
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

  temp = dataIn.substring(0, indexOfA);
  level = dataIn.substring(indexOfA + 1, indexOfB);
  turb = dataIn.substring(indexOfB + 1, indexOfC);
  flow = dataIn.substring(indexOfC + 1, indexOfD);
  dirty = dataIn.substring(indexOfD + 1, indexOfE);
  empty = dataIn.substring(indexOfE + 1, indexOfF);
}