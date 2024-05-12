#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <SoftwareSerial.h>
#include <RCSwitch.h>

SoftwareSerial SoftSerial(2, 3);       //piny 2 a 3 // předělat na SoftSerial
Adafruit_SSD1306 display(128,64, &Wire, -1);
DHT dht(11, DHT11);       //pin 11
RCSwitch mySwitch = RCSwitch();
RCSwitch mySwitchR = RCSwitch();
int t=0;
int radio=0;
int radio2=0;
int radio3=0;
int bits=0;
int bits2=0;
int bits3=0;
int radiocount=0;
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
  mySwitchR.enableReceive(13);
  dht.begin();
  SoftSerial.begin(57600);
  zastavit();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    //Serial.println("OLED displej není nalezen!");
    while (true);
  }
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.display();
  display.setCursor(0, 0);
  display.print("Kigtdert");
  display.setCursor(0, 16);
  display.print("robotics");
  display.display();
  digitalWrite(4, HIGH);
  delay(150);
  digitalWrite(4, LOW);
  delay(150);
  digitalWrite(4, HIGH);
  delay(150);
  digitalWrite(4, LOW);
  while (!SoftSerial.available()) {
}
    String zprava3 = SoftSerial.readString();
    display.clearDisplay();
    display.setCursor(0,0);
    display.print("IP adresa: ");
    display.setCursor(0,16);
    display.print(zprava3);   //ip adresa serveru
    display.display();
    delay(5000);
   
}

void loop() {
while (!SoftSerial.available()) {
    int x = digitalRead(5); //kapacitní tlačítko
  if (x == 1)
  {
    if(t<=100){
      t++;
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
      display.print(baterie/128 + "V");
      if (baterie<700){
        digitalWrite(4, HIGH);
        digitalWrite(10, HIGH);
        delay(100);
        digitalWrite(4, LOW);
        digitalWrite(10, LOW);
        delay(100);
      }
      display.display();
  }
  else if(t==200){
    t=0;
  }
  else{
  t++;
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
  }
}
switch (SoftSerial.parseInt()) {
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
    while (!SoftSerial.available()) {
      } 
    String text = SoftSerial.readString();
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print(text);
    display.display();
    display.setTextSize(2);
    delay(10000);
    break;
  case 9:
      while (!mySwitchR.available()) {
        if (SoftSerial.parseInt()==3){
          break;
        }
        } 
      
    if (radiocount==0){
      radio = mySwitchR.getReceivedValue();
      bits = mySwitchR.getReceivedBitlength();
      radiocount = 1;
    }
    else if(radiocount==1){
      radio2 = mySwitchR.getReceivedValue();
      bits2 = mySwitchR.getReceivedBitlength();
      radiocount = 2;
    }
    else if(radiocount==2){
      radio3 = mySwitchR.getReceivedValue();
      bits3 = mySwitchR.getReceivedBitlength();
      radiocount = 0;
    }
    digitalWrite(4,HIGH);
    delay(200);
    digitalWrite(4,LOW);
    mySwitch.resetAvailable();
     break;
  case 901:   
      mySwitch.send(radio, bits); 
    break;
  case 902:   
      mySwitch.send(radio2, bits2); 
    break;
  case 903:   
      mySwitch.send(radio3, bits3); 
    break;
}
}
