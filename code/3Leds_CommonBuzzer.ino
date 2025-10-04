#include <Wire.h>
#include <MPU6050_light.h>

// ----- LDR -----
#define LDR_AO_PIN   34
#define LED_LDR_PIN  25
const int ldrThreshold = 2000; // LED ON when analogValue > threshold

// ----- MPU6050 -----
MPU6050 mpu(Wire);
#define LED_MPU_PIN  26
const float jerkThreshold = 0.3;
float prevAccel = 0;

// ----- Reed Switch -----
#define REED_PIN     33
#define BUZZER_PIN   14 
const bool REED_IS_MODULE = true;
char buzzer_output = false;
char intrusion_alert = false;

#define REED_PIN_LED  13

void setup() {
  Serial.begin(115200);

  // LDR
  pinMode(LED_LDR_PIN, OUTPUT);

  // MPU6050
  Wire.begin(21, 22);
  pinMode(LED_MPU_PIN, OUTPUT);
  byte status = mpu.begin();
  if (status != 0) {
    Serial.println("❌ MPU6050 init failed");
  } else {
    mpu.calcOffsets();
    Serial.println("✅ MPU6050 ready");
  }

  // Reed Switch
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(REED_PIN_LED, OUTPUT);
  pinMode(REED_PIN, REED_IS_MODULE ? INPUT : INPUT_PULLUP);
}

void loop() {
  intrusion_alert = false;
  // ----- LDR -----
  int analogValue = analogRead(LDR_AO_PIN);
  Serial.print("📈 Analog LDR: ");
  Serial.println(analogValue);
  if (analogValue < ldrThreshold) {
    Serial.println("🌞 Outside light detected!");
    digitalWrite(LED_LDR_PIN, HIGH);
    intrusion_alert = true;
  } else {
    digitalWrite(LED_LDR_PIN, LOW);
  }

  // ----- MPU6050 -----
  mpu.update();
  float ax = mpu.getAccX();
  float ay = mpu.getAccY();
  float az = mpu.getAccZ();
  float totalAccel = sqrt(ax * ax + ay * ay + az * az);
  float jerk = abs(totalAccel - prevAccel);
  prevAccel = totalAccel;

  Serial.print("📊 Jerk: ");
  Serial.println(jerk);
  if (jerk > jerkThreshold) {
    Serial.println("💥 Jerk Detected!");
    digitalWrite(LED_MPU_PIN, HIGH);
    intrusion_alert = true;
    delay(200);
    digitalWrite(LED_MPU_PIN, LOW);
  }

  // ----- Reed Switch -----
  int reedState = digitalRead(REED_PIN);
  if (reedState == LOW) {
    Serial.println("✅ Reed Closed (Magnet near)");
    digitalWrite(REED_PIN_LED, LOW);
  } else {
    Serial.println("🚨 Reed Open (Tamper)!");
    digitalWrite(REED_PIN_LED, HIGH);
    intrusion_alert = true;
    //digitalWrite(BUZZER_PIN, HIGH);
    //delay(300);
    //digitalWrite(BUZZER_PIN, LOW);
  }

  if(true == intrusion_alert) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(300);
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(200); // small delay for loop pacing
}
