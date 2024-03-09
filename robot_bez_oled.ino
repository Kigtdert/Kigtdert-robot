// Knihovny pro OLED a DHT11
#include <Wire.h>
#include <DHT.h>
#include <string.h>
#include <SoftwareSerial.h>
SoftwareSerial SoftSerial(2, 3);

#define DHT_PIN 11 // Pin pro DHT11
#define DHT_TYPE DHT11 // Typ senzoru
DHT dht(DHT_PIN, DHT_TYPE);

void dopredu() {
  digitalWrite(8, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  digitalWrite(9, LOW);
  digitalWrite(12,HIGH);  
}
void dozadu() {
  digitalWrite(8, LOW);
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(9, HIGH);
  digitalWrite(12,HIGH); 
}
void doprava() {
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
  digitalWrite(12,HIGH);  
}
void doleva() {
  digitalWrite(7, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
  digitalWrite(12,HIGH);
  
}
void zastavit() {
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(12,LOW);
  // zastavit
}
void setup() {
  // Inicializace sériové komunikace
  pinMode(12, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(5, INPUT);
  pinMode(A2,INPUT);
  dht.begin();
  SoftSerial.begin(9600);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
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
while (!SoftSerial.available()) {
  int baterka=analogRead(A2);
  if (baterka<700){
    digitalWrite(12, HIGH);
    digitalWrite(10, HIGH);//červená ledka
    delay(100);
    digitalWrite(12, LOW);
    digitalWrite(10, LOW);
    delay(100);
  }
}
    String zprava1 = SoftSerial.readString();
    String zprava2 = zprava1.substring(0,1);
    int motory1=zprava2.toInt();
    
    switch (motory1) {
  case 1:
    dopredu();
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
    digitalWrite(12,HIGH);
    delay(200);
    digitalWrite(12,LOW);
    break;
    case 7:   
    String text = zprava1.substring(1);
    //display.print(text);
    break;
} // Čekání jedné sekundy před dalším měřením
}
