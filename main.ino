#define PHS_PIN 12
#define LED_PIN 13
#define RELAY_PIN 7


// PHS_PIN - Photosensor pin
void setup(){
  //Serial.begin(9600);
  
  pinMode(PHS_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  
  digitalWrite(RELAY_PIN, HIGH); // държи релето вдигнато
}

void loop(){
  //Serial.print("Here");
  if (digitalRead(PHS_PIN) == HIGH){
    //Serial.print("HIGH: LIGHT ON - ");
    digitalWrite(LED_PIN, HIGH); // Светка светодиод
    delay(500);
    digitalWrite(LED_PIN, LOW); // Загасва светодиод
    delay(500);
    //Serial.println("LIGHT OFF");
    //Serial.println();

    digitalWrite(RELAY_PIN, LOW); // спуска релето и лампата светва (затваря се ключа на ел. схема)
    while(1){
      delay(1000);
      if (digitalRead(PHS_PIN) == LOW) { // ако стане светло
        digitalWrite(RELAY_PIN, HIGH); // вдига се релето и лампата изгасва
        delay(100);
        break; // излизаме от цикъла
      }
    }
    
  }else {
    //Serial.println("LOW: NOPE");
  }
}
