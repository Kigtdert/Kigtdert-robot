void setup(void) {
  Serial.begin(9600);
}
void loop(void) {
  float napeti1;
  napeti1 = analogRead(A1);
  napeti1 = 1023 / napeti1 - 1;
  napeti1 = 10000 / napeti1;
  float teplota1;
  teplota1 = napeti1 / 10000;        
  teplota1 = log(teplota1);           
  teplota1 /= 3950;                   
  teplota1 += 1.0 / (25 + 273.15); 
  teplota1 = 1.0 / teplota1;            
  teplota1 -= 273.15;                  
  Serial.print(teplota1);
  /////////////////////////
   float napeti2;
  napeti2 = analogRead(A2);
  napeti2 = 1023 / napeti2 - 1;
  napeti2 = 10000 / napeti2;
  float teplota2;
  teplota2 = napeti2 / 10000;        
  teplota2 = log(teplota2);           
  teplota2 /= 3950;                   
  teplota2 += 1.0 / (25 + 273.15); 
  teplota2 = 1.0 / teplota2;            
  teplota2 -= 273.15;                  
  Serial.print(teplota2);
  //////////////////////////
  float napeti3;
  napeti3 = analogRead(A0);
  napeti3 = 1023 / napeti3 - 1;
  napeti3 = 10000 / napeti3;
  float teplota3;
  teplota3 = napeti3 / 10000;        
  teplota3 = log(teplota3);           
  teplota3 /= 3950;                   
  teplota3 += 1.0 / (25 + 273.15); 
  teplota3 = 1.0 / teplota3;            
  teplota3 -= 273.15;                  
  Serial.print(teplota3);
}
