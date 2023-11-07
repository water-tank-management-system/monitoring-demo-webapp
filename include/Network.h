/* ========================================

Header Files
Related to Networking

======================================== */


#include "CapstoneA9.h"


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


// ===== User-Defined Functions Declarations =====

// Initialize WiFi Connection
void initWiFi();

// Initialize Firebase Configuration
void initFirebase();

// Function that gets current epoch time
unsigned long getTime();