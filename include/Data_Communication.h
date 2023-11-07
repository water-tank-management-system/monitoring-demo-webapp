/* ========================================

Header Files
Related to Data Communication

======================================== */


#include "CapstoneA9.h"


// ===== User-Defined Functions Declarations =====

// Write Float values to the database
void sendFloat(String path, float value);

// Write Int values to the database
void sendInt(String path, int value);

// Write Boolean values to the database
void sendBool(String path, bool value);

// Write Data Log in JSON to the database
void sendDataLog();

// Receive Boolean values from the database
bool receiveBool(String path);

// Parse the Data from master
void parsingData();