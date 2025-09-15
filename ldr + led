#define LDR_AO_PIN 34
#define LED_PIN    25
const int ldrThreshold = 2000;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int analogValue = analogRead(LDR_AO_PIN);
  Serial.print("📈 Analog LDR: ");
  Serial.println(analogValue);

  // LED ON when light is strong (value > 3000)
  digitalWrite(LED_PIN, analogValue < ldrThreshold ? HIGH : LOW);

  delay(300);
}

