#include <Wire.h> 
#include "LiquidCrystal_I2C.h"
#include <LedControl.h>
#include "dht11.h"
#define Soil_Pin                    A0
#define Light_Pin                   A1
#define Temp_Pin                    A2
#define Motor                       A3
#define Human_Pin                   13
#define Shake_Pin                   11
#define MP3_A1                      8
#define MP3_A2                      7
#define MP3_A3                      6
#define MP3_A4                      5
#define MP3_A5                      4
#define MP3_BY                      3   
#define interrupt_run               2
#define interrupt_stop              1

LiquidCrystal_I2C lcd(0x27,16,2);
 
dht11 DHT11;
int DIN = 12;
int CS =  10;
int CLK = 9;

LedControl lc=LedControl(DIN,CLK,CS,4);   
  
float Temperature = 25.0;
boolean T = 1;
float SoilMoisture = 0.0;
boolean S = 1;
int Moveable = 0;    
int waterlessflag = 0;
int overwater = 0;

void setup() {
  SayIt(0,0,0,0,0);//boot sound 
  delay(2000);
  smile();
  lc.shutdown(0,false);       //启动时，MAX72XX处于省电模式
  lc.setIntensity(0,8);       //将亮度设置为最大值
  lc.clearDisplay(0);         //清除显示 
  pinMode(Light_Pin,INPUT);
  pinMode(Temp_Pin,OUTPUT);
  pinMode(Soil_Pin,INPUT);
  pinMode(Human_Pin,INPUT);
  pinMode(Shake_Pin,INPUT);
  pinMode(MP3_A1,OUTPUT);
  pinMode(MP3_A2,OUTPUT);
  pinMode(MP3_A3,OUTPUT);
  pinMode(MP3_A4,OUTPUT);
  pinMode(MP3_A5,OUTPUT);
  pinMode(MP3_BY,INPUT);
  pinMode(interrupt_run,OUTPUT);
  pinMode(Motor,OUTPUT);
  lcd.init();
  lcd.backlight();
  digitalWrite(interrupt_run,LOW);
  digitalWrite(interrupt_stop,LOW);
  digitalWrite(Motor,LOW);
  Serial.begin(9600);

}

void loop() {
  //Serial.print("LightValue:");
  //Serial.println(LightValue());
  if(LightValue()<150){
     Run();
     SayIt(1,1,0,1,0);//lookforshadow     
     sad ();
     delay(2000);
     
     
        }
  else if(LightValue()>330&&LightValue()<500){
    Run();
     SayIt(1,0,0,1,0);//lookforsun
     sad ();
     delay(2000);
           
  }
  else{
    Stop();
  //  smile();
  }
  
  if (!IsHumanDetect()){
     waterlessflag = 0;
     overwater = 0;
  }
  
  Temperature = GetTemperature();
 //Serial.println(Temperature);
  SoilMoisture = GetSoilMoisture();
  LcdDisplayData(SoilMoisture,Temperature);   //display soil moisture
  if(Temperature!=0){
  if (Temperature >= 30.0){
    digitalWrite(Motor,HIGH);
       sad();
       SayIt(1,1,0,0,0);//hot
      // Run();
       delay(1000);
  }
  else if(Temperature <= 18.0){
    digitalWrite(Motor,LOW);
       sad();
       SayIt(1,1,1,1,0);//cold
     //  Run();

  }
   else{
    // stop();
    //  smile();
    }
  }
  
  if (SoilMoisture <= 15.0 && IsHumanDetect()){
      sad();  
   if(waterlessflag<3){
        waterlessflag++;
        SayIt(1,0,1,0,0);//water
        delay(2000);
        sad(); 
        }
    }
  else if (SoilMoisture <= 15.0)
  {
      sad();
      SayIt(1,0,1,0,0);//water 
      delay(2000);
    }
  else{
     // smile();
    }


  if (SoilMoisture >= 40.0 && IsHumanDetect()){
      sad();
      if (overwater = 0){
            SayIt(0,1,1,1,0);//overwater
            overwater = 1;
      }      
  }else if(SoilMoisture >= 40.0){

     sad();
     SayIt(0,1,1,1,0);//overwater
     delay(2000);
  }
    
  else {
     // smile();
    }

  if(IsShakeDetect()){
    SayIt(1,0,1,1,0);//earthquake
  }
 if((LightValue()>150&&LightValue()<330)&&(Temperature>18.0&&Temperature<30.0)&&(SoilMoisture>15.0&&SoilMoisture<40.0)){
   smile();
 }
 else if((LightValue>500)&&(Temperature>18.0&&Temperature<30.0)&&(SoilMoisture>20.0&&SoilMoisture<40.0)){
   smile();
 }
 else{
   sad();
  }
}
//*********************************************************************************

//Return value
  float GetTemperature(){
    int chk = DHT11.read(Temp_Pin);
    return  (float)DHT11.temperature;
  }
 

//Return 1 is Detected

  int LightValue(){
    return analogRead(Light_Pin);
  }

//Return value

  float GetSoilMoisture(){
    float data = analogRead(Soil_Pin);
    return (1-(data/1023))*100;
}

//Return 1 is Detected

  int IsHumanDetect(){
    return digitalRead(Human_Pin);
  }


//Return 1 is Shaking
 
  int IsShakeDetect(){
  
    int value = 1;
    value = digitalRead(Shake_Pin);
    if (value == 0){
      return 1;
    }else{
      return 0;
    }
  
}

  void LcdDisplayData( float data1,float data2) {
   
      lcd.setCursor(0, 0);
      lcd.print("Hum is:");
      lcd.print(data1, 2);
      lcd.print("%");
      lcd.setCursor(0, 1);
      lcd.print("Tem is:");
      lcd.print(data2, 2);
      lcd.print("C");
      delay(200);
}
//***********************SmileAndCry
  void printByte(byte character [])

{
    int i = 0;
    for(i=0;i<8;i++)

  {
      lc.setRow(0,i,character[i]);
  }

}

  void smile()
{
    byte smile[8]=   {0x00,0x00,0x44,0xAA,0x00,0x00,0x44,0x38};//笑脸
    printByte(smile);//显示笑脸
}

  void sad()
{
     byte sad[8]=   {0x00,0x00,0xEE,0x00,0x00,0x38,0x44,0x00};//哭脸
     printByte(sad);//显示哭脸
}

/*
 * BIT1 BIT2 BIT3 BIT4 BIT5
 *  1    0    0    0    0   ->1th  mp3 file
 *  1    0    0    0    1   ->2th  mp3 file
 *  ...
 *  1    1    1    1    1   ->15th mp3 file
 *  0    0    0    0    0   ->16th mp3 file
 *  0    0    0    0    1   ->17th mp3 file
 *  ...
 */
  void SayIt(int BIT1,int BIT2,int BIT3,int BIT4,int BIT5){
    
    while(!(digitalRead(MP3_BY))){
    }
    
    digitalWrite(MP3_A1,HIGH);  
    digitalWrite(MP3_A2,HIGH);  
    digitalWrite(MP3_A3,HIGH);  
    digitalWrite(MP3_A4,HIGH);  
    digitalWrite(MP3_A5,HIGH);  
    delay(100);

    if(BIT1 == 1){
      digitalWrite(MP3_A1,HIGH);  
    }else{
      digitalWrite(MP3_A1,LOW);  
    }

    if(BIT2 == 0){
      digitalWrite(MP3_A2,HIGH);  
    }else{
      digitalWrite(MP3_A2,LOW);  
    }  

    if(BIT3 == 0){
      digitalWrite(MP3_A3,HIGH);  
    }else{
      digitalWrite(MP3_A3,LOW);  
    } 
   
    if(BIT4 == 0){
      digitalWrite(MP3_A4,HIGH);  
    }else{
      digitalWrite(MP3_A4,LOW);  
    } 

    if(BIT5 == 0){
      digitalWrite(MP3_A5,HIGH);  
    }else{
      digitalWrite(MP3_A5,LOW);  
    }   
    
    delay(100);
}
//**********************************************************************************

void Run(){
  digitalWrite(interrupt_stop,LOW);
  digitalWrite(interrupt_run,HIGH);
}
void Stop(){
  digitalWrite(interrupt_run,LOW);
  digitalWrite(interrupt_stop,HIGH);
}
