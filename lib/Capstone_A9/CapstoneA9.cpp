/* ========================================

Main Function Files
Capstone A9

======================================== */


#include <CapstoneA9.h>


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
