#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <SoftwareSerial.h>
SoftwareSerial Serial2(2, 3);

#define OLED_WIDTH 128  // Šířka displeje v pixelech
#define OLED_HEIGHT 64  // Výška displeje v pixelech
#define OLED_RESET -1   // Reset pin pro displej
#define DHT_PIN 11      // Pin pro DHT11
#define DHT_TYPE DHT11  // Typ senzoru

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHT_PIN, DHT_TYPE);

void dopredu() {
  digitalWrite(8, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  digitalWrite(9, LOW);
  Serial.println("dopredu");
}
void dozadu() {
  digitalWrite(8, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(9, HIGH);
  Serial.println("dozadu");
}
void doprava() {
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
  Serial.println("doprava");
}
void doleva() {
  digitalWrite(7, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
  Serial.println("doleva");
}
void zastavit() {
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  // zastavit
}
void setup() {
  Serial.begin(9600);
  pinMode(12, OUTPUT);  //piezo
  pinMode(10, OUTPUT);  //červená ledka
  pinMode(6, OUTPUT);   //motory
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(5, INPUT);    //tlacitko vedle displaye
  pinMode(A3,INPUT);    //napětí baterie
  pinMode(A2,INPUT);    // termistor 1
  pinMode(A1,INPUT);    // termistor 2
  pinMode(A0,INPUT);    // termistor 3
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED displej není nalezen!");
    while (true);
  }
  
  dht.begin();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  Serial2.begin(9600);
  zastavit();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Kigtdert");
  display.setCursor(0, 16);
  display.print("robotics");
  display.display();
  digitalWrite(12, HIGH);
  delay(250);
  digitalWrite(12, LOW);
  delay(250);
  digitalWrite(12, HIGH);
  delay(250);
  digitalWrite(12, LOW);
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int x = digitalRead(5);
  if (x == 1)
  {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("zapnuto: ");
      display.setCursor(0, 16);
      int t = millis() / 60000;
      display.print(t);
      display.setCursor(24, 16);
      display.print("min");
      display.setCursor(0, 34);
      display.print("baterie: ");
      display.setCursor(0, 50);
      int baterie = analogRead(A3);
      if (baterie<700){
        display.print("LOW");
        digitalWrite(12, HIGH);
        digitalWrite(10, HIGH);
        delay(100);
        digitalWrite(12, LOW);
        digitalWrite(10, LOW);
        delay(100);
      }
      else{
        display.print(baterie/128 + "V");
      }
      display.display();
  }
  else{
    display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Teplota:");
  display.setCursor(0, 16);
  display.print(temperature);
  display.write(247);
  display.print("C");
  display.setCursor(0, 34);
  display.print("vlhkost: ");
  display.setCursor(0, 50);
  display.print(humidity);
  display.print("%");
  display.display();
  }
  
  float napeti1 = analogRead(A1);
  float napeti2 = analogRead(A2);
  float napeti3 = analogRead(A0); 

  while (napeti1>800 or napeti2>800 or napeti3>800)      //hodnota napeti se musí otestovat podle realných teplot a odporů
  {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Overheat");
    display.setCursor(0, 16);
    display.print("error");
    display.display();
    digitalWrite(10,HIGH);
    delay(500);
    digitalWrite(10,LOW);
    delay(500);
    float napeti1 = analogRead(A1);
    float napeti2 = analogRead(A2);
    float napeti3 = analogRead(A0);
    }
  

while (!Serial2.available()) {

}
    String zprava = Serial2.readString();
    String zprava2 = zprava.substring(0,1);
    int motory1=zprava2.toInt();
    Serial.println(motory1);
    
    switch (motory1) {
  case 1:
    Serial.println("dopredu");
    dopredu();
    break;
  case 2:
    Serial.println("dozadu");
    dozadu();
    break;
   case 3:
    Serial.println("doprava");
    doprava();
    break;
  case 4:
    Serial.println("doleva");
    doleva();
    break;
  case 5:
    Serial.println("stop");
    zastavit();
    break;
   case 6:
    Serial.println("beep");
    digitalWrite(12,HIGH);
    delay(200);
    digitalWrite(12,LOW);
    break;
  case 7:
    String text = zprava.substring(1);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(text);
    display.display();
    delay(5000);
    break;
}
}
