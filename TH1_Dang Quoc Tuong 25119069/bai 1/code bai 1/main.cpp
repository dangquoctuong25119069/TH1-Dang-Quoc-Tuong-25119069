#include <Arduino.h>

// Tích hợp hàm PWM để chạy trực tiếp trên Wokwi không cần file phụ
void setup_pwm(uint8_t pin, uint8_t channel, uint32_t freq, uint8_t res) {
#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
    ledcAttach(pin, freq, res);
#else
    ledcSetup(channel, freq, res);
    ledcAttachPin(pin, channel);
#endif
}

void write_pwm(uint8_t pin, uint8_t channel, uint32_t duty) {
#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 0, 0)
    ledcWrite(pin, duty);
#else
    ledcWrite(channel, duty);
#endif
}

#define BUTTON_PIN 4
#define LED_PIN 2

#define LED_FREQ 5000
#define LED_RES_BITS 8
#define LED_CHANNEL 0

volatile bool buttonPressed = false;

void IRAM_ATTR handleInterrupt() {
  buttonPressed = true;
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("\n\n=== BAI 1: Ngat ngoai + PWM LED (Khong Block) ===");
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  setup_pwm(LED_PIN, LED_CHANNEL, LED_FREQ, LED_RES_BITS);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleInterrupt, FALLING);
  
  Serial.println("-> San sang! Nhan nut de ngat qua trinh fading.");
}

void loop() {
  static int brightness = 0;
  static int step = 1;
  
  if (buttonPressed) {
    buttonPressed = false;
    Serial.println(">>> Nhan nut (Ngat)!");
    
    write_pwm(LED_PIN, LED_CHANNEL, 0);   delay(80);
    write_pwm(LED_PIN, LED_CHANNEL, 255); delay(80);
    write_pwm(LED_PIN, LED_CHANNEL, 0);   delay(80);
  }
  
  brightness += step;
  if (brightness <= 0 || brightness >= 255) {
    step = -step;
  }
  
  write_pwm(LED_PIN, LED_CHANNEL, brightness);
  delay(10);
}