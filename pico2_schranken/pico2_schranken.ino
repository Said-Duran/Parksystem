#include <Arduino.h>

#define Pico1Serial Serial1

const int PIN_SCHRANKE_EIN = 2;  
const int PIN_SCHRANKE_AUS = 3;  

void setup() {
  Serial.begin(115200);       // USB zum PC (Kamera-Simulation UI)
  Pico1Serial.begin(9600);    // UART zu Pico 1

  pinMode(PIN_SCHRANKE_EIN, OUTPUT);
  pinMode(PIN_SCHRANKE_AUS, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    String simCmd = Serial.readStringUntil('\n');
    simCmd.trim();

    if (simCmd.startsWith("ENTRY:")) {
      String plate = simCmd.substring(6);
      openBarrier(PIN_SCHRANKE_EIN);
    } 
    else if (simCmd.startsWith("EXIT:")) {
      String plate = simCmd.substring(5);
      
      // Bezahlstatus bei Pico 1 nachfragen
      Pico1Serial.println("CHECK_PAID:" + plate);
      
      unsigned long startWait = millis();
      String response = "";
      while (millis() - startWait < 1000) {
        if (Pico1Serial.available()) {
          response = Pico1Serial.readStringUntil('\n');
          response.trim();
          break;
        }
      }

      if (response == "STATUS:PAID") {
        openBarrier(PIN_SCHRANKE_AUS);
        Serial.println("BARRIER_OPENED");
      } else {
        Serial.println("BARRIER_REMAIN_CLOSED");
      }
    }
  }
}

void openBarrier(int pin) {
  digitalWrite(pin, HIGH);
  delay(3000); //3 Sekunden offen lassen
  digitalWrite(pin, LOW);
}