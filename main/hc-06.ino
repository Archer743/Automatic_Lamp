int LED = 13;
int value;

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    value = Serial.read();
    if (value == '1') {
      digitalWrite(LED, HIGH);
      Serial.println("Light ON");
    }
    else if (value == '0') {
      digitalWrite(LED, LOW);
      Serial.println("Light OFF");
    }
  }
}
