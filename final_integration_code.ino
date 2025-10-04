#define BLYNK_TEMPLATE_ID "TMPL3aCUYisPZ"
#define BLYNK_TEMPLATE_NAME "Nonchalant Tamper"
#define BLYNK_AUTH_TOKEN    "iZVw5TVW_2dFNC-LNMcJ3CQPPSqcTCOl"


#include <Wire.h>
#include <MPU6050_light.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>


// ----- BLYNK -----
char auth[] = "iZVw5TVW_2dFNC-LNMcJ3CQPPSqcTCOl";   // replace with your Blynk Auth Token
char ssid[] = "Airtel_3997";          // replace with your WiFi SSID
char pass[] = "9538723997";      // replace with your WiFi Password


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


// ----- BLYNK VIRTUAL PINS -----
// Match these with your dashboard widgets
#define VPIN_LDR          V0
#define VPIN_LDR_LED      V1
#define VPIN_JERK         V2
#define VPIN_MOTION       V3
#define VPIN_REED_STATE   V4
#define VPIN_BUZZER       V5
#define VPIN_INTRUSION    V6


void setup() {
  Serial.begin(115200);


  // WiFi + Blynk
  Blynk.begin(auth, ssid, pass);


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
  Blynk.run();  // keep Blynk connected
  intrusion_alert = false;


  // ----- LDR -----
  int analogValue = analogRead(LDR_AO_PIN);
  Serial.print("📈 Analog LDR: ");
  Serial.println(analogValue);
  Blynk.virtualWrite(VPIN_LDR, analogValue);


  if (analogValue < ldrThreshold) {
    Serial.println("🌞 Outside light detected!");
    digitalWrite(LED_LDR_PIN, HIGH);
    Blynk.virtualWrite(VPIN_LDR_LED, 1);
    intrusion_alert = true;
  } else {
    digitalWrite(LED_LDR_PIN, LOW);
    Blynk.virtualWrite(VPIN_LDR_LED, 0);
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
  Blynk.virtualWrite(VPIN_JERK, jerk);


  if (jerk > jerkThreshold) {
    Serial.println("💥 Jerk Detected!");
    digitalWrite(LED_MPU_PIN, HIGH);
    Blynk.virtualWrite(VPIN_MOTION, 1);
    intrusion_alert = true;
    delay(200);
    digitalWrite(LED_MPU_PIN, LOW);
    Blynk.virtualWrite(VPIN_MOTION, 0);
  }


  // ----- Reed Switch -----
  int reedState = digitalRead(REED_PIN);
  Blynk.virtualWrite(VPIN_REED_STATE, reedState);
  if (reedState == LOW) {
    Serial.println("✅ Reed Closed (Magnet near)");
    digitalWrite(REED_PIN_LED, LOW);
  } else {
    Serial.println("🚨 Reed Open (Tamper)!");
    digitalWrite(REED_PIN_LED, HIGH);
    intrusion_alert = true;
  }


  // ----- Buzzer & Intrusion -----
  if(true == intrusion_alert) {
    digitalWrite(BUZZER_PIN, HIGH);
    Blynk.virtualWrite(VPIN_BUZZER, 1);
    Blynk.virtualWrite(VPIN_INTRUSION, 1);
    delay(300);
    digitalWrite(BUZZER_PIN, LOW);
    Blynk.virtualWrite(VPIN_BUZZER, 0);
  } else {
    Blynk.virtualWrite(VPIN_INTRUSION, 0);
  }


  delay(200); // small delay for loop pacing
}




/* # #include <Wire.h>
#include <MPU6050_light.h>

// --- Pin Definitions ---
#define LDR_PIN 34
#define REED_PIN 15
#define BUZZER_PIN 14

// --- LDR Settings ---
const int ldrThreshold = 1950; // Adjust after testing
const int sampleCount = 10;    // For averaging

// --- MPU6050 Settings ---
MPU6050 mpu(Wire);
const float jerkThreshold = 1.8; // g-change needed to trigger
const int avgWindow = 10;        // For display smoothing
float accelBuffer[avgWindow];
int bufferIndex = 0;
bool bufferFilled = false;
float prevTotalAccel = 0;

void setup() {
  Serial.begin(115200);
  delay(500);

  // --- I2C + MPU6050 ---
  Wire.begin(21, 22);
  Wire.setClock(100000);
  delay(100);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  pinMode(LDR_PIN, INPUT);
  pinMode(REED_PIN, INPUT); // LM393 module handles pull-up

  byte status = mpu.begin();
  if (status != 0) {
    Serial.println("❌ MPU6050 init failed");
  } else {
    mpu.calcOffsets();
    Serial.println("✅ MPU6050 ready");
  }
}

void loop() {
  bool tamperDetected = false;

  // --- 1️⃣ LDR Stabilized Reading ---
  long total = 0;
  for (int i = 0; i < sampleCount; i++) {
    total += analogRead(LDR_PIN);
    delay(5); // small delay for stable reading
  }
  int avgLDR = total / sampleCount;

  Serial.print("📈 LDR Value (avg): ");
  Serial.println(avgLDR);

  if (avgLDR > ldrThreshold) {
    Serial.println("🔦 Outside light detected — possible tamper");
    tamperDetected = true;
  }

  // --- 2️⃣ Reed Switch Detection ---
  int reedState = digitalRead(REED_PIN);
  if (reedState == LOW) {
    Serial.println("✅ Magnet detected — lid closed");
  } else {
    Serial.println("🚨 Magnet NOT detected — lid opened");
    tamperDetected = true;
  }

  // --- 3️⃣ MPU6050 Aggressive Hit Detection ---
  mpu.update();
  float ax = mpu.getAccX();
  float ay = mpu.getAccY();
  float az = mpu.getAccZ();
  float totalAccel = sqrt(ax * ax + ay * ay + az * az);
  float jerk = abs(totalAccel - prevTotalAccel);
  prevTotalAccel = totalAccel;

  // Store in buffer for averaging display
  accelBuffer[bufferIndex] = totalAccel;
  bufferIndex = (bufferIndex + 1) % avgWindow;
  if (bufferIndex == 0) bufferFilled = true;

  if (jerk > jerkThreshold) {
    Serial.println("💥 Aggressive impact detected!");
    tamperDetected = true;
  }

  // Optional: print smoothed accel + jerk
  if (bufferFilled || bufferIndex > 0) {
    float sum = 0;
    int count = bufferFilled ? avgWindow : bufferIndex;
    for (int i = 0; i < count; i++) sum += accelBuffer[i];
    float avgAccel = sum / count;

    Serial.print("📊 Avg Accel (g): ");
    Serial.print(avgAccel, 2);
    Serial.print(" | Jerk: ");
    Serial.println(jerk, 2);
  }

  // --- 4️⃣ Buzzer Response ---
  if (tamperDetected) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(300); // short buzz
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(50); // main loop delay
}
*/