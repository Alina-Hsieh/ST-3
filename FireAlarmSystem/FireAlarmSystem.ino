/* smoke -> LED+buzzer -> flame -> DHT ->LCD  */
#include <MQ2.h>
#include <Wire.h>
#include <DHT.h>
#include <DHT_U.h> 

int Analog_Input = A0;
int lpg, co, smoke;
int temperature;
int ledPin = 2;
int buzzer = 8 ;
int ledState = LOW;
unsigned long previousMillis = 0; 
const long interval = 100;
int FLAME_DO = 7 ;
int FLAME_AO = A5;
int pin = 3;

MQ2 mq2(Analog_Input);
DHT dht (pin, DHT11);

void setup(){
  Serial.begin(9600);
  mq2.begin();
  dht.begin();
  pinMode(ledPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(FLAME_DO, INPUT);
  pinMode(FLAME_AO, INPUT);
}

void loop(){
//smoke
  Serial.print("Air quality: ");
  float* values= mq2.read(true); //set it false if you don't want to print the values in the Serial
  lpg = mq2.readLPG();  //lpg = values[0];
  co = mq2.readCO();  //co = values[1];
  smoke = mq2.readSmoke();  //smoke = values[2];

// flame
  int fire_DO = digitalRead(FLAME_DO);    //從感測器 DO  AO 兩端分別讀入數位訊號及類比訊號
  int fire_AO = analogRead (FLAME_AO);

//DHT
  temperature = dht.readTemperature();
  Serial.print("Temperature now is: ");
  Serial.print(temperature);       
  Serial.println("C");

// 判斷
  if(fire_DO == LOW || co >= 10 || temperature >= 35) //當數位訊號 = 0 的時候，啟動蜂鳴器，點亮 LED
  {
    Serial.println("EMERGENCY! Please check the fire alarm!");
    Serial.println("");
   
    tone(buzzer, 144);
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
      digitalWrite(ledPin, ledState);
    }
  }
  else{
    Serial.println("");
    noTone(buzzer);
    digitalWrite(ledPin, LOW);
  }
}
