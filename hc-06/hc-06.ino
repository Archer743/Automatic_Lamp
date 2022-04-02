#define LED_PIN 13
#define PHS_PIN 12
#define RELAY_PIN 7
#define PIR_MS_PIN 3
#define RESET_PIN 2


int pirMsCalibrationTime = 30; // в секунди
bool sys = true; // дали системата е включена
bool light = false; // дали лампата свети
int lightTime = 20;  // в секунди; колко време ще свети лампата при необходимите условия

String input_string = "";


void setup() {
  Serial.begin(9600);
  
  pinMode(PHS_PIN, INPUT);
  pinMode(PIR_MS_PIN, INPUT);
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  
  digitalWrite(RELAY_PIN, HIGH); // държи релето вдигнато
  digitalWrite(PIR_MS_PIN, LOW); // първоначално датчикът за движение не засича движение
  digitalWrite(LED_PIN, HIGH); // светодиодът свети, с което показва, че системата е активна
  digitalWrite(RESET_PIN, HIGH); // ако стане LOW, Ардуиното ще се рестартира

  pir_calibration();
}

void loop() {
  if(Serial.available()) {
    input_string = Serial.readString();
    
    if (input_string.startsWith("sys on") == true) {
      if (sys != true){
        sys = true;
        if (light == true) {
          digitalWrite(RELAY_PIN, HIGH);
          light = false;
        }
        digitalWrite(LED_PIN, HIGH);
        Serial.println("System ON");
      }else Serial.println("System Already ON");
    }else if (input_string.startsWith("sys off") == true) {
      if (sys != false) {
        sys = false;
        if (light == true) {
          digitalWrite(RELAY_PIN, HIGH);
          light = false;
        }
        digitalWrite(LED_PIN, LOW);
        Serial.println("System OFF");
      }else Serial.println("System Already OFF");
    }

    if (input_string.startsWith("sys new lt ") == true){
      input_string.remove(0, 11);  // removes "sys new lt "
      input_string.replace(" ", "");  // removes whitespaces

      if (input_string == "default" || input_string == "d"){
        if (lightTime != 20) {
          lightTime = 20;
          Serial.print("Done! New Light Time: ");
          Serial.print(lightTime);
          Serial.println(" seconds");
        }else Serial.println("Light Time Is Already Set To 20 seconds!");
      }else if (input_string.endsWith("s") == true) {
        input_string.remove(input_string.length()-1);
        int seconds = abs(input_string.toInt());  // if input_string is not a valid number => returns 0
        if (seconds != 0) {
          lightTime = seconds;
          Serial.print("Done! New Light Time: ");
          Serial.print(lightTime);
          Serial.println(" seconds");
        }else Serial.println("Incorrect input!");
      }else if(input_string.endsWith("m") == true) {
        input_string.remove(input_string.length()-1);
        int seconds = abs(input_string.toInt()) * 60;  // if input_string is not a valid number => returns 0
        if (seconds != 0) {
          lightTime = seconds;
          Serial.print("Done! New Light Time: ");
          Serial.print(lightTime/60);
          Serial.println(" minutes");
        }else Serial.println("Incorrect input!");
      }else Serial.println("Incorrect input!");
    }

    if (input_string.startsWith("sys lamp on") == true){
      if (sys == false && light == false) {
        digitalWrite(RELAY_PIN, LOW);
        light = true;
        Serial.print("Done! Light ON!");
      }else Serial.println("This feature works when the system is stopped and the lamp does not light!");
    }else if (input_string.startsWith("sys lamp off") == true){
      if (sys == false && light == true) {
        digitalWrite(RELAY_PIN, HIGH);
        light = false;
        Serial.print("Done! Light OFF!");
      }else Serial.println("This feature works when the system is stopped and the lamp lights!");
    }

    if (sys == true) {
      if (digitalRead(PHS_PIN) == HIGH && digitalRead(PIR_MS_PIN) == HIGH){
        Serial.print("> Motion detected at: ");
        Serial.print(millis()/1000);
        Serial.println("s");

        digitalWrite(RELAY_PIN, LOW); // спуска релето и лампата светва (затваря се ключа на ел. схема)
        delay(lightTime*1000); // seconds -> milliseconds
        
        digitalWrite(RELAY_PIN, HIGH); // вдига се релето и лампата изгасва
        delay(100);
        
        Serial.print("> Motion ended at: ");
        Serial.print(millis()/1000);
        Serial.println("s");
      }    
    }
  }
}

void pir_calibration() {
  delay(5000);
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
