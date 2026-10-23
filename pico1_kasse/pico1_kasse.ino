#include <Arduino.h>

struct Ticket {
  String plate;
  unsigned long entryMillis;
  bool isPaid;
};

const int MAX_CARS = 20;
Ticket tickets[MAX_CARS];
int ticketCount = 0;

#define Pico2Serial Serial1

void setup() {
  Serial.begin(115200);       
  Pico2Serial.begin(9600);    
}

void loop() {
  if (Pico2Serial.available()) {
    String cmd = Pico2Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd.startsWith("CHECK_PAID:")) {
      String plate = cmd.substring(11);
      bool paid = checkPaidStatus(plate);
      Pico2Serial.println(paid ? "STATUS:PAID" : "STATUS:UNPAID");
    }
  }

 
  if (Serial.available()) {
    String pcCmd = Serial.readStringUntil('\n');
    pcCmd.trim();
    handleKasseCommand(pcCmd);
  }
}

bool checkPaidStatus(String plate) {
  for (int i = 0; i < ticketCount; i++) {
    if (tickets[i].plate == plate) {
      return tickets[i].isPaid;
    }
  }
  return false;
}

void handleKasseCommand(String cmd) {
  
}