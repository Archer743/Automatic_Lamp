#define LED_PIN_1 13
#define PHS_PIN 12
#define LED_PIN_2 9
#define RELAY_PIN 7
#define PIR_MS_PIN 3


int pirMsCalibrationTime = 30; // в секунди
boolean sys = true; // дали системата е включена
boolean light = false; // дали лампата свети
int lightTime = 40;  // в секунди; колко време ще свети лампата при необходимите условия

String input_string = "";


void setup() {
  Serial.begin(9600);
  
  pinMode(PHS_PIN, INPUT); // фоторезистор
  pinMode(PIR_MS_PIN, INPUT); // сензор за движение
  
  pinMode(LED_PIN_1, OUTPUT); // LED за движение
  pinMode(LED_PIN_2, OUTPUT); // LED за системата
  pinMode(RELAY_PIN, OUTPUT); // Реле - то пуска или спира тока; работи като ключ в схемата
  
  digitalWrite(RELAY_PIN, HIGH); // държи релето вдигнато
  digitalWrite(PIR_MS_PIN, LOW); // първоначално датчикът за движение не засича движение
  digitalWrite(LED_PIN_1, LOW); // светодиодът свети, когато е засечено движение
  digitalWrite(LED_PIN_2, HIGH); // светодиодът свети, с което показва, че системата е активна

  pir_calibration(); // сензорът за движение се нуждае от време, за да се ориентира в околната среда
}

void loop() {
   if (digitalRead(PIR_MS_PIN) == HIGH && digitalRead(PHS_PIN) == HIGH && sys == true) {  // ако е засечено движение + е тъмно + системата е включена
      Serial.println("Motion detected!");
      
      digitalWrite(LED_PIN_1, HIGH); // светва диода
      
      digitalWrite(RELAY_PIN, LOW); // спуска релето и лампата светва (затваря се ключът на ел. схема)
      light = true;
      
      for (int i = 1; i <= lightTime; i++) {
        input_string = Serial.readString();
        commands();
        if (sys != true) break;
        delay(1000);
      }
      
      digitalWrite(RELAY_PIN, HIGH); // вдига се релето и лампата изгасва
      digitalWrite(LED_PIN_1, LOW); // диодът изгасва
      light = false;
      delay(100);
    }
    
    input_string = Serial.readString();
    commands();
}

void pir_calibration() {
  delay(5000);
  
  Serial.print("Calibrating Motion Sensor ");
  
  for (int i = 1; i <= pirMsCalibrationTime; i++) {
    if (i % 5 == 0) Serial.print("_");
    else Serial.print(".");
    delay(1000);
  }
  
  Serial.println("");
  Serial.println("DONE!");
  delay(50);
}

void commands() {
    if (input_string != "") sysOnOff();

    if (input_string != "") ltChange();

    if (input_string != "") lampMode();

    if (input_string != "") sysInfo();

    if (input_string != "") help();
}

void sysOnOff() {
  if (input_string.startsWith("sys on") == true) {
      if (sys != true) {
        sys = true;
        if (light == true) {
          digitalWrite(RELAY_PIN, HIGH);
          light = false;
        }
        digitalWrite(LED_PIN_2, HIGH);
        Serial.println("System ON");
      }else Serial.println("System Already ON");
      input_string = "";
      
    }else if (input_string.startsWith("sys off") == true) {
      if (sys != false) {
        sys = false;
        if (light == true) {
          digitalWrite(RELAY_PIN, HIGH);
          light = false;
        }
        digitalWrite(LED_PIN_2, LOW);
        Serial.println("System OFF");
      }else Serial.println("System Already OFF");
      input_string = "";
    }
}

void ltChange() {
  if (input_string.startsWith("sys new lt ") == true) {
      input_string.remove(0, 11);  // removes "sys new lt "
      input_string.replace(" ", "");  // removes whitespaces

      if (input_string == "default" || input_string == "d") {
        if (lightTime != 40) {
          lightTime = 40;
          Serial.print("Done! New Light Time: ");
          Serial.print(lightTime);
          Serial.println(" seconds");
        }else Serial.println("Light Time Is Already Set To 40 seconds!");
        
      }else if (input_string.endsWith("s") == true) {
        input_string.remove(input_string.length()-1);  // removes "s"
        int seconds = abs(input_string.toInt());  // if input_string is not a valid number => returns 0
        if (seconds != 0 && seconds >= 20 && seconds <= 900) {  // граници -> от 20 секунди до 15 минути включително
          lightTime = seconds;
          Serial.print("Done! New Light Time: ");
          Serial.print(lightTime);
          Serial.println(" seconds");
        }else Serial.println("Incorrect input! Check if the value is between 20 and 900 seconds");
        
      }else if(input_string.endsWith("m") == true) {
        input_string.remove(input_string.length()-1);  // removes "m"
        int seconds = abs(input_string.toInt()) * 60;  // if input_string is not a valid number => returns 0
        if (seconds != 0 && seconds >= 20 && seconds <= 900) {  // граници -> от 20 секунди до 15 минути включително
          lightTime = seconds;
          Serial.print("Done! New Light Time: ");
          Serial.print(lightTime/60);
          Serial.println(" minutes");
        }else Serial.println("Incorrect input! Check if the value is no more than 15 minutes");
        
      }else Serial.println("Incorrect input! Make sure you follow this pattern: sys new lt [integer][s/m]");
      input_string = "";
    }
}

void lampMode() {
  if (input_string.startsWith("sys lamp on") == true) {
      if (sys == false && light == false) {
        digitalWrite(RELAY_PIN, LOW);
        light = true;
        Serial.print("Done! Light ON!");
      }else Serial.println("This feature works when the system is stopped and the lamp does not light!");
      input_string = "";
      
    }else if (input_string.startsWith("sys lamp off") == true) {
      if (sys == false && light == true) {
        digitalWrite(RELAY_PIN, HIGH);
        light = false;
        Serial.print("Done! Light OFF!");
      }else Serial.println("This feature works when the system is stopped and the lamp lights!");
      input_string = "";
    }
}

void sysInfo() {
  if (input_string.startsWith("sys info") == true) {
    Serial.println("INFO");
    
    Serial.print("-> System: ");
    if (sys) Serial.println("Active");
    else Serial.println("Inactive");

    Serial.print("-> Lamp: ");
    if (light) Serial.println("ON");
    else Serial.println("OFF");

    Serial.print("-> Light time: ");

    int s = lightTime % 60;
    int m = ((lightTime - s)/60) % 60;
    
    Serial.print(m);
    Serial.print(":");
    if (s <= 9) Serial.print("0");
    Serial.print(s);
    if (m < 2) Serial.println(" minute");
    else Serial.println(" minutes");
    
    input_string = "";
  }
}

void help() {
  if (input_string.startsWith("sys help") == true || input_string.startsWith("help") == true) {
    Serial.println("HELP - COMMANDS");
    Serial.println("sys [on/off] <-> turns the system on or off\n");
    Serial.println("sys new lt [integer][s/m] <-> sets new light time; [s/m] - seconds or minutes\n");
    Serial.println("sys lamp [on/off] <-> turns the lamp on or off\n");
    Serial.println("sys info <-> provides information about the system\n");
    Serial.println("sys help; help <-> displays this message\n");
    Serial.println("You don't have to write [ ]");
    input_string = "";
  }
}
