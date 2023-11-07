/* ========================================

Header Files
Related to Networking

======================================== */


#ifndef Network_H
#define Network_H


#include <CapstoneA9.h>


// ===== User-Defined Functions Declarations =====

// Initialize WiFi Connection
void initWiFi();

// Initialize Firebase Configuration
void initFirebase();

// Function that gets current epoch time
unsigned long getTime();

#endif