#define LED_PIN 13
#define RELAY_PIN 7

#define PHS_PIN 12
#define PIR_MS_PIN 3


int pirMsCalibrationTime = 30;

// PHS_PIN - Photosensor pin
// PIR_MS_PIN - PIR Motion Sensor pin

void setup(){
  Serial.begin(9600);
  
  pinMode(PHS_PIN, INPUT);
  pinMode(PIR_MS_PIN, INPUT);
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  
  digitalWrite(RELAY_PIN, HIGH); // държи релето вдигнато
  digitalWrite(PIR_MS_PIN, LOW); // първоначално датчикът за движение не засича движение

  Serial.println("========================================================");
  Serial.print("Calibrating Motion Sensor ");
  for (int i = 1; i <= pirMsCalibrationTime; i++) {
    if (i % 5 == 0) Serial.print("_");
    else Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.println("DONE!");
  Serial.println("========================================================");
  delay(50);
}

void loop(){
  //
  if (digitalRead(PHS_PIN) == HIGH && digitalRead(PIR_MS_PIN) == HIGH){
    Serial.print("> Motion detected at: ");
    Serial.print(millis()/1000);
    Serial.println("s");
    digitalWrite(LED_PIN, HIGH); // Светка светодиод
    delay(500);

    digitalWrite(RELAY_PIN, LOW); // спуска релето и лампата светва (затваря се ключа на ел. схема)
    while(digitalRead(PIR_MS_PIN) != LOW){
      delay(1000);
//      if (digitalRead(PHS_PIN) == LOW) { // ако стане светло
//        digitalWrite(RELAY_PIN, HIGH); // вдига се релето и лампата изгасва
//        delay(100);
//        break; // излизаме от цикъла
//      }
    }
    digitalWrite(RELAY_PIN, HIGH); // вдига се релето и лампата изгасва
    delay(100);
    
    digitalWrite(LED_PIN, LOW); // Загасва светодиод
    delay(500);
    Serial.print("> Motion ended at: ");
    Serial.print(millis()/1000);
    Serial.println("s");
  }
}
