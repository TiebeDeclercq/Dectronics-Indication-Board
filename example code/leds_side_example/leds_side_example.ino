// Pin definitions for LED control
#define LED1_PIN        2    // LED1
#define LED2_PIN        0    // LED2
#define LED3_PIN        4    // LED3

// Optional, only if you have configured the correct solder pads
#define LED4_PIN     16    // LED5/RX data indicator
#define LED5_PIN     17    // LED6/TX data indicator

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  pinMode(LED4_PIN, OUTPUT);
  pinMode(LED5_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED3_PIN, LOW);
  delay(200);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, HIGH);
  delay(200);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, HIGH);
  delay(200);
}
