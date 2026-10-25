#include <Arduino.h>

#define BUTTON_PIN 4
#define DEBOUNCE_MS 50

volatile uint32_t debouncedCount = 0;
volatile uint32_t rawEdgeCount = 0;
volatile unsigned long lastInterruptTime = 0;
volatile bool newPress = false;

void IRAM_ATTR handleInterrupt() {
  rawEdgeCount++;
  unsigned long now = millis();
  
  if (now - lastInterruptTime >= DEBOUNCE_MS) {
    debouncedCount++;
    lastInterruptTime = now;
    newPress = true;
  }
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("\n\n=== BAI 2: Chong doi (Debounce) nut nhan ===");
  Serial.printf("Cua so chong doi: %d ms\n", DEBOUNCE_MS);
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleInterrupt, FALLING);
}

void loop() {
  if (newPress) {
    newPress = false;
    Serial.println("------------------------------------");
    Serial.printf("So suon xuong THO (raw) : %u\n", rawEdgeCount);
    Serial.printf("So lan nhan THUC (loc)  : %u\n", debouncedCount);
  }
  delay(10);
}