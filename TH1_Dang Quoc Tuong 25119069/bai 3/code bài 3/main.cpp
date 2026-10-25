#include <Arduino.h>

// Tích hợp hàm PWM để chạy trực tiếp trên Wokwi
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

#define ADC_PIN 34
#define LED_PIN 18
#define N 8

int buf[N];
int idx = 0;

int loc(int m) {
  buf[idx] = m;
  idx = (idx + 1) % N;
  long t = 0;
  for(int i = 0; i < N; i++) {
    t += buf[i];
  }
  return t / N;
}

void setup() { 
  Serial.begin(115200);
  delay(200);
  Serial.println("\n\n=== BAI 3: ADC + Loc Nhieu + LEDC ===");
  
  setup_pwm(LED_PIN, 0, 5000, 8);
  
  for(int i=0; i<N; i++) {
    buf[i] = 0;
  }
}

void loop() {
  int tho = analogRead(ADC_PIN);
  int l = loc(tho);
  
  int pwm_val = map(l, 0, 4095, 0, 255);
  write_pwm(LED_PIN, 0, pwm_val);
  
  Serial.print("Tho:"); Serial.print(tho);
  Serial.print(",");
  Serial.print("Loc:"); Serial.println(l);
  
  delay(20);
}