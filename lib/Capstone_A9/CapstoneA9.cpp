/* ========================================

Main Function Files
Capstone A9

======================================== */


#include <CapstoneA9.h>


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


// ===== User-Defined Variables =====

// ===== Variables to save database paths

String  uid,        // Variable to save USER UID
        parentPath; // Parent Node (to be updated in every loop)

// Database main path (to be updated in setup with the user UID)
String databasePath;

// Database child nodes
String  tmpPath,
        lvlPath,
        flwPath,
        turPath,
        dirPath,
        empPath,
        autPath,
        clnPath,
        fllPath;

// Data Logging Variables
String  dataLogPath,
        tmpLog = "/temperature",
        lvlLog = "/water-level",
        flwLog = "/water-flow",
        turLog = "/turbidity",
        timePath = "/timestamp";

// Variable to save current epoch time
int timestamp;

// ===== Variables Related to System Communication

// System Readings Data Variables
float temperature = 0,
      water_level = 0,
      water_flow = 0;
int   turbidity = 0;
bool automation = false,
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
unsigned long sendDataPrevMillis = 0,
              timerDelay = 20000, // send new readings every minute
              prev = 0;


// ===== User-Defined Functions =====

// Create function to state the LED
void stateLED(const long interval, int period)
{
  for (int i = 0; i <= period; i++)
  {
    digitalWrite(BUILTIN_LED, HIGH);
    delay(interval);
    digitalWrite(BUILTIN_LED, LOW);
    delay(interval);
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

  temp = dataIn.substring(0, indexOfA);
  level = dataIn.substring(indexOfA + 1, indexOfB);
  turb = dataIn.substring(indexOfB + 1, indexOfC);
  flow = dataIn.substring(indexOfC + 1, indexOfD);
  dirty = dataIn.substring(indexOfD + 1, indexOfE);
  empty = dataIn.substring(indexOfE + 1, indexOfF);
}

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
