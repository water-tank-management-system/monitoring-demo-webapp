/* ========================================

MONITORING WEB-APPS
Firebase Project

======================================== */


#include <CapstoneA9.h>
#include <Network.h>
#include <Data_Communication.h>


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
