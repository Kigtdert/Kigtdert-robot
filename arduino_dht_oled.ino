#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <SoftwareSerial.h>
#include <RCSwitch.h>

SoftwareSerial Serial2(2, 3);       //piny 2 a 3
Adafruit_SSD1306 display(128,64, &Wire, -1);
DHT dht(11, DHT11);       //pin 11
RCSwitch mySwitch = RCSwitch();

void dopredu() {
  digitalWrite(8, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  digitalWrite(9, LOW);
}
void dozadu() {
  digitalWrite(8, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(9, HIGH);
}
void doprava() {
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
}
void doleva() {
  digitalWrite(7, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
}
void zastavit() {
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
}
void setup() {
  pinMode(4, OUTPUT);   //piezo
  pinMode(10, OUTPUT);  //červená ledka
  pinMode(6, OUTPUT);   //motory
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(5, INPUT);    //tlacitko vedle displaye
  pinMode(A3,INPUT);    //napětí baterie
  mySwitch.enableTransmit(12); //433Mhz vysílač
  
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    //Serial.println("OLED displej není nalezen!");
    while (true);
  }
  
  dht.begin();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  Serial2.begin(9600);
  zastavit();
  display.clearDisplay();
  display.display();
  display.setCursor(0, 0);
  display.print("Kigtdert");
  display.setCursor(0, 16);
  display.print("robotics");
  display.display();
  digitalWrite(4, HIGH);
  delay(250);
  digitalWrite(4, LOW);
  delay(250);
  digitalWrite(4, HIGH);
  delay(250);
  digitalWrite(4, LOW);
  while (!Serial2.available()) {

}
    String zprava3 = Serial2.readString();
    String zprava4 = zprava3.substring(0,1);
    if (zprava4=="1"){
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("IP adresa: ");
    display.setCursor(0,16);
    display.print(zprava3);   //ip adresa serveru
    display.display();
    delay(10000);
    }
}

void loop() {
  int x = digitalRead(5); //kapacitní tlačítko
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
        digitalWrite(4, HIGH);
        digitalWrite(10, HIGH);
        delay(100);
        digitalWrite(4, LOW);
        digitalWrite(10, LOW);
        delay(100);
      }
      else{
        display.print(baterie/128 + "V");
      }
      display.display();
  }
  else{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
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
  
  

while (!Serial2.available()) {

}
    String zprava1 = Serial2.readString();
    String zprava2 = zprava1.substring(0,1);
    int motory1=zprava2.toInt();
    
switch (motory1) {
  case 1:
  //omezení ip adresou
    break;
  case 2: 
    doleva();
    break;
  case 3: 
    zastavit();
    break;
  case 4:   
    doprava();
    break;
  case 5:   
    dozadu();
    break;
  case 6:   
    digitalWrite(4,HIGH);
    delay(200);
    digitalWrite(4,LOW);
    break;
  case 7:
    dopredu();
    break;
  case 8:   
    String text = zprava1.substring(1);
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print(text);
    display.display();
    display.setTextSize(2);
    delay(10000);
    break;
  case 9:   
    String radio = zprava1.substring(1);
    if(radio == "25"){
      mySwitch.send(5393, 24); 
        }
    else if(radio == "28"){
      mySwitch.send(5398, 24); 
        }
    break;
}
}
