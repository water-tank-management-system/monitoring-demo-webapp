/* ========================================

Main Function Files
Capstone A9

======================================== */


#include <CapstoneA9.h>


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
