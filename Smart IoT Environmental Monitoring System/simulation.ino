#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>

const int GAS_PIN    = A0;   
const int TEMP_PIN   = A1;   
const int HUM_PIN    = A2;   
const int LED_GREEN  = 3;   
const int LED_YELLOW = 4;   
const int LED_RED    = 5;   

Adafruit_LiquidCrystal lcd(0); 

unsigned long lastRefresh = 0; 

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  lcd.begin(16, 2);
  lcd.setBacklight(1);
  
  lcd.setCursor(0, 0);
  lcd.print("SYSTEM STARTING");
  delay(2000); 
  lcd.clear();
}

void loop() {
  // 1. FAST SENSOR READS
  int gasValue = analogRead(GAS_PIN);
  int tempRaw = analogRead(TEMP_PIN);
  float voltage = tempRaw * (5.0 / 1023.0);
  float tempC = (voltage - 0.5) * 100;
  int humidity = map(analogRead(HUM_PIN), 0, 1023, 0, 100);

  // 2. REFRESH LCD EVERY 500ms
  if (millis() - lastRefresh >= 500) {
    lastRefresh = millis();

    lcd.setCursor(0, 0);
    lcd.print("T:"); 
    lcd.print((int)tempC); 
    lcd.print("C  "); 
    
    lcd.setCursor(8, 0);
    lcd.print("H:"); 
    lcd.print(humidity); 
    lcd.print("%   ");

    lcd.setCursor(0, 1);
    lcd.print("AirQ: ");
    lcd.print(gasValue);
    lcd.print("    "); 
  }

  // 3. LED LOGIC - FIXED: Now passing all 3 required arguments
  updateLEDs(gasValue, tempC, humidity);
  
  // 4. SERIAL MONITOR (Validation)
  Serial.print("T:"); Serial.print(tempC, 1);
  Serial.print(" H:"); Serial.print(humidity);
  Serial.print(" G:"); Serial.println(gasValue);
}

// Function to update LEDs based on any sensor threshold
void updateLEDs(int gas, float temp, int hum) {
  bool gasDanger  = (gas > 700);
  bool tempDanger = (temp > 45.0); 
  bool humDanger  = (hum > 85);    

  bool gasWarn    = (gas > 400);
  bool tempWarn   = (temp > 35.0); 
  bool humWarn    = (hum > 70);    

  if (gasDanger || tempDanger || humDanger) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, LOW);
  } 
  else if (gasWarn || tempWarn || humWarn) {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_GREEN, LOW);
  } 
  else {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, HIGH);
  }
}