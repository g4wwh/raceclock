#include <Wire.h>

int buzzer=2;
int redPin=5;
int bluePin=6;
int greenPin=7;
int latchPin = 8; 
int clockPin = 12;
int dataPin = 11;
int count = 0;
int tcount = 1;
int del=10;
int sw=0;
int rs=0;
int ss=0;
int mm=00;
int hh=00;
int D1;
int D2;
int D3;
int D4;
int D5;
int D6;
int dp;
int dp1;
int dp2;
int newsec;
int oldsec;
int start;
long Tstart;
long Tnow;
long Trace;
int Rh;
int Rm;
int Rs;
int reset;
int redState;
int greenState;
int blueState;
int redPushed=0;
int greenPushed=0;
int bluePushed=0;
const int seg[8] = {1,2,4,8,16,32,64,128};
int num[11];
int test[10];
int stp=0;
int clockMode=0;
int justPushed=0;
int hours;
int minutes;
int seconds;
int setHours=0;
int secMinutes=0;
int setSeconds=0;
long raceTime=0;
int stopClock=0;
int resetClock=0;
int subSec;
int fraction;
unsigned long msec;
unsigned long milli0;
void setup () {
  
  //start chronodot bits
  Wire.begin();
  Serial.begin(9600);
  pinMode(buzzer,OUTPUT);
  
  Wire.beginTransmission(0x68);
  Wire.write(0x0E);
  Wire.write(0b00011100);
  Wire.endTransmission();
  //delay(100);

  //initChrono();
  //end chronodot bits
  
  pinMode(greenPin, INPUT);
  pinMode(redPin, INPUT);
  pinMode(bluePin, INPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT); 
  
  
  bannerDisplay();
 
// define the numerical digits by segments

num[0] = seg[0]+seg[1]+seg[2]+seg[3]+seg[5]+seg[6];
num[1] = seg[0]+seg[1];
num[2] = seg[1]+seg[2]+seg[3]+seg[4]+seg[6];
num[3] = seg[0]+seg[1]+seg[2]+seg[4]+seg[6];
num[4] = seg[0]+seg[1]+seg[4]+seg[5];
num[5] = seg[0]+seg[2]+seg[4]+seg[5]+seg[6];
num[6] = seg[0]+seg[2]+seg[3]+seg[4]+seg[5]+seg[6];
num[7] = seg[0]+seg[1]+seg[2];
num[8] = seg[0]+seg[1]+seg[2]+seg[3]+seg[4]+seg[5]+seg[6];
num[9] = seg[0]+seg[1]+seg[2]+seg[4]+seg[5]+seg[6];
num[10] = 0;

showHello();
showReady();
//delay(1000);

int pMode=getMode();

int line[]={30,30,16,pMode,30,30};
     writeDigits(line);
     waitSwitch(bluePin);
     
if (pMode == 5){
  setClock();
  delay(500);
  pMode=getMode();
}

if (pMode == 1){
  countDown();
  delay(500);
  }

if (pMode == 2){
  stopClock=1;
  delay(500);
  }

if (pMode == 3){
  buzzer=1;
  countDown();
  delay(500);
  }


}

void loop() {
  
  
  oldsec=seconds;
  getTime();
    
    if (start == 0 || reset == 1) {
      Tstart=seconds+(minutes*60+hours*3600);
      start=1;
      reset=0;
    }
    Tnow=seconds+(minutes*60+hours*3600);
    //Trace=Tnow-Tstart;
    Trace=raceTime;
    
    
    long Rh=Trace/3600;
    long Rm=(Trace-3600*Rh)/60;
    long Rs=Trace-3600*Rh-60*Rm;
    
    
  dp1=seg[7];
  dp2=seg[7];

  //oldsec=ss;
  
  ss=seconds;  
  mm=minutes;
  hh=hours;
  
  justPushed=0;
  
  if (bluePushed == 1 && clockMode == 0){
  
  clockMode=1;
  justPushed=1;
  bluePushed=0;
  delay(10);
  }
  
  if (bluePushed == 1 && clockMode == 1 && justPushed == 0){
  
  clockMode=0;
  justPushed=1;
  bluePushed=0;
  delay(10);
  }
  
  if (clockMode == 0){
    ss=Rs;
    mm=Rm;
    hh=Rh;
  }
  
  
  newsec=0;
  if (seconds != oldsec and (clockMode == 1 || stopClock != 1)) {
    newsec=1;
    buzz(10);
    milli0=millis();
  }
  
  msec=millis()-milli0;
  
  D2=int(ss/10);
  D1=ss-(10*D2);
  
  //D2=int(subSec/10);
  //D1=subSec-(10*D2);
  //delay(100);
  
  
  
  D4=int(mm/10);
  D3=mm-(10*D4);
  
  D6=int(hh/10);
  D5=hh-(10*D6);
  //D1=7;
  
/*  if ( D6 == 0 ) {
    D6 = 10;
    //dp1=0;
  }
  if (D6 == 10 && D5 == 0) {
    D5 = 10;
    dp1=0;
  }
  
 if (D4 == 0 && D5 == 0 && D6 == 0) {
    D4 = 10;
 }
 
 */
    dp=seg[7];
    
    
    if (redPushed == 1 && clockMode == 0) {
      buzz(10);
      stopClock=1;
      fraction=subSec;    
    }
    if (greenPushed == 1 && clockMode == 0) {
      buzz(10);
      stopClock=0;    
    }
    

    if (redState == 1 && greenState == 1 && clockMode ==0){
      raceTime=0;
    }
    
    int oldRedState=redState;
    int oldGreenState=greenState;
    int oldBlueState=blueState;
    redPushed=0;
    greenPushed=0;
    bluePushed=0;
    redState = digitalRead(redPin);
    greenState = digitalRead(greenPin);
    blueState = digitalRead(bluePin);
    
    if (redState > oldRedState){
      redPushed=1;
    }
    if (greenState > oldGreenState){
      greenPushed=1;
    }
    if (blueState > oldBlueState){
      bluePushed=1;
    }
    
  
    writeTime(num[D1],num[D2],num[D3],num[D4],num[D5],num[D6]);
   
    if (newsec == 1 && stopClock !=1) {
      raceTime+=1;
      buzz(10);     
    }
  
  
  if (redState==1 && greenState==1 && blueState==1){
    softReset();
  }
  
  subSec+=1;
}

void initChrono(){
  
  set_time();
  set_date();
}

void set_time(){
  int ssec=0;
  int smin=37;
  int shrs=21;
  Wire.beginTransmission(0x68);
  Wire.write(0);
  Wire.write(decToBcd(ssec));
  Wire.write(decToBcd(smin));
  Wire.write(decToBcd(shrs));
  Wire.endTransmission();
  
}

void set_date(){
  int sday=1;
  int sdate=28;
  int smonth=4;
  int syear=13;
  Wire.beginTransmission(0x68);
  Wire.write(3);
  Wire.write(decToBcd(sday));
  Wire.write(decToBcd(sdate));
  Wire.write(decToBcd(smonth));
  Wire.write(decToBcd(syear));
  Wire.endTransmission();
}
    
byte decToBcd (byte val){
  return ( (val/10*16) + (val%10) );
}

void bannerDisplay(){
  
  int top[]={20,20,20,20,20,20};
  int mid[]={30,30,30,30,30,30};
  int bot[]={40,40,40,40,40,40};
  
  //line sweep
  for (int i=0; i<5; i++){
    writeDigits(top);
    delay(50);
    writeDigits(mid);
    delay(50);
    writeDigits(bot);
    delay(50);
    writeDigits(mid);
    delay(50);
  }
   
   
   
}
  
  
void countDown(){
  
  //countdown from 5
   int c5[]={5,98,98,98,98,98};
   int c4[]={98,4,98,98,98,98};
   int c3[]={98,98,3,98,98,98};
   int c2[]={98,98,98,2,98,98};
   int c1[]={98,98,98,98,1,98};
   int c0[]={6,0,6,0,6,0};
   int cm[]={98,98,98,98,98,98};
   int cb[]={99,99,99,99,99,99};
   
   writeDigits(c5);
   buzz(50);
   delay(950);
   writeDigits(c4);
   buzz(50);
   delay(950);
   writeDigits(c3);
   buzz(50);
   delay(950);
   writeDigits(c2);
   buzz(50);
   delay(950);
   writeDigits(c1);
   buzz(50);
   delay(950);
    
   //flash the zero
   for (int i=0;i<10;i++){
     writeDigits(c0);
     buzz(25);
     delay(25);
     writeDigits(cb);
     buzz(25);
     delay(25);
   }
}
void writeTime(int t1, int t2, int t3, int t4, int t5, int t6){
  
  // Order of digits is correct as per the shft register to 
  // digit wiring
  
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, t1);
    shiftOut(dataPin, clockPin, MSBFIRST, t2);
    shiftOut(dataPin, clockPin, MSBFIRST, t3+dp2);
    shiftOut(dataPin, clockPin, MSBFIRST, t4);
    shiftOut(dataPin, clockPin, MSBFIRST, t5+dp1);
    shiftOut(dataPin, clockPin, MSBFIRST, t6);
    digitalWrite(latchPin, HIGH);
}

void writeDigits(int a[6]){
  
  digitalWrite(latchPin, LOW);
  
  for (int i=5; i>=0; i--) {
    switch(a[i]){
      case 0:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[0]+seg[1]+seg[2]+seg[3]+seg[5]+seg[6]);
        break;
      case 1:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[0]+seg[1]);
        break;
      case 2:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[1]+seg[2]+seg[3]+seg[4]+seg[6]);
        break;
      case 3:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[0]+seg[1]+seg[2]+seg[4]+seg[6]);
        break;
      case 4:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[0]+seg[1]+seg[4]+seg[5]);
        break;
      case 5:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[0]+seg[2]+seg[4]+seg[5]+seg[6]);
        break;
      case 6:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[0]+seg[2]+seg[3]+seg[4]+seg[5]+seg[6]);
        break;
      case 7:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[0]+seg[1]+seg[2]);
        break;
      case 8:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[0]+seg[1]+seg[2]+seg[3]+seg[4]+seg[5]+seg[6]);
        break;
      case 9:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[0]+seg[1]+seg[2]+seg[4]+seg[5]+seg[6]);
        break;
      case 10:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[0]+seg[1]+seg[2]+seg[3]+seg[4]+seg[5]);
        break;
      case 11:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[0]+seg[3]+seg[4]+seg[5]+seg[6]);
        break;
      case 12:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[2]+seg[3]+seg[5]+seg[6]);
        break;
      case 13:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[0]+seg[1]+seg[3]+seg[4]+seg[6]);
        break;
      case 14:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[2]+seg[3]+seg[4]+seg[5]+seg[6]);
        break;
      case 15:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[2]+seg[3]+seg[4]+seg[5]);
        break;
      case 16:
      shiftOut(dataPin, clockPin, MSBFIRST,seg[1]+seg[2]+seg[3]+seg[4]+seg[5]);
        break;    
        case 17:
      shiftOut(dataPin, clockPin, MSBFIRST,seg[3]+seg[5]+seg[6]);
        break;
      case 18:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[3]+seg[4]+seg[5]+seg[6]);
        break;
      case 19:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[2]+seg[3]+seg[5]);
        break;
      case 20:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[2]);
        break;
      case 21:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[0]+seg[1]+seg[4]+seg[5]+seg[6]);
        break;
      case 22:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[0]+seg[1]+seg[3]+seg[4]+seg[5]);
        break;
      case 30:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[4]);
        break;
      case 40:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[6]);
        break;
      case 50:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[7]);
        break;
      case 98:
        shiftOut(dataPin, clockPin, MSBFIRST,seg[4]);
        break;
      default:
        shiftOut(dataPin, clockPin, MSBFIRST,0);
        
    }
  }
  
  digitalWrite(latchPin, HIGH);
}  

void buzz(int t){
  digitalWrite(buzzer,HIGH);
   delay(t);
   digitalWrite(buzzer,LOW);
}

int getMode() {

   int pMode=1;
   int modeOK=0;
   int change=0;
   //pinMode(redPin, INPUT);
    
   while (modeOK == 0){
     
     int line[]={5,14,18,99,16,pMode,99};
  
     writeDigits(line);
  
     int okSwitch = digitalRead(bluePin);
     if (okSwitch == 1){
       modeOK=1;
     }
     
     delay(50);
     
     if (change == 1){
       delay(500);
       change=0;
     }
     
     int switchMode = digitalRead(greenPin);
   
     if (switchMode == 1) {
       buzz(50);
       pMode+=1;
       change=1;
     }
     
     switchMode = digitalRead(redPin);
   
     if (switchMode == 1) {
       buzz(50);
       pMode-=1;
       change=1;
     }
   
     if (pMode == 6) {
       pMode=1;
     }
     
     if (pMode == 0) {
       pMode=5;
     }
   
   }
   buzz(300);
   return (pMode);
}

void waitSwitch(int swPin){
 
  pinMode(swPin, INPUT);
  
  while (digitalRead(swPin)== 0){
    
    delay(50);
  }
}

int getKeys(){
 int keyState=0;

 if(digitalRead(greenPin)==1){
   keyState+=1;
 }
 
 if(digitalRead(redPin)==1){
   keyState+=2;
 }
 if(digitalRead(bluePin)==1){
   keyState+=4;
 }
 
 return keyState;
}

void softReset(){
  
  //write "reset" to screen

  int line[]={19,14,5,14,18,99};
  
     writeDigits(line);
     
  delay(500);
  
  
  asm volatile (" jmp 0");
}

void showReady(){
  
  int line[]={19,14,10,13,21,99};
  
     writeDigits(line);
     delay(1000);
     //knightRider();
     //delay(1000);
     
  //delay(2000);
}

void showHello(){
  
  int line[]={22,14,17,17,0,99};
  
     writeDigits(line);
     delay(1000);
     //knightRider();
     //delay(1000);
     
  //delay(2000);
}
void knightRider(){
  
    for (int i=0; i<5; i++){
    int line[]={50,50,99,99,99,99};
    writeDigits(line);
    delay(100);
    int line2[]={99,50,50,99,99,99};
    writeDigits(line2);
    delay(100);
    int line3[]={99,99,50,50,99,99};
    writeDigits(line3);
    delay(100);
    int line4[]={99,99,99,50,50,99};
    writeDigits(line4);
    delay(100);
    int line5[]={99,99,99,99,50,50};
    writeDigits(line5);
    delay(100);
    int line6[]={99,99,99,50,50,99};
    writeDigits(line6);
    delay(100);
    int line7[]={99,99,50,50,99,99};
    writeDigits(line7);
    delay(100);
    int line8[]={99,50,50,99,99,99};
    writeDigits(line8);
    delay(100);
    int line9[]={50,50,99,99,99,99};
    writeDigits(line9);
    delay(100);
    
    }
     
}

void setClock(){
  
  getTime();
  
  
  int ss=0;
  int mm=minutes;
  int hh=hours;
  
  D2=int(ss/10);
  D1=ss-(10*D2);
   
  D4=int(mm/10);
  D3=mm-(10*D4);
  
  D6=int(hh/10);
  D5=hh-(10*D6);
  
  dp1=seg[7];
  dp2=seg[7];
  
  writeTime(num[D1],num[D2],num[D3],num[D4],num[D5],num[D6]);
  
  delay(500);
  while (digitalRead(bluePin)!=1){
    
    if(digitalRead(greenPin)==1){
   mm+=1;
   if ( mm == 60 ){
     mm=0;
   }
    }
     delay(150);
     
     if(digitalRead(redPin)==1){
   hh+=1;
   if ( hh == 24 ){
     hh=0;
   }
    }
     delay(150);
     
     D2=int(ss/10);
  D1=ss-(10*D2);
   
  D4=int(mm/10);
  D3=mm-(10*D4);
  
  D6=int(hh/10);
  D5=hh-(10*D6);
  
  dp1=seg[7];
  dp2=seg[7];
  
  writeTime(num[D1],num[D2],num[D3],num[D4],num[D5],num[D6]);
 }
  
  
  //delay(5000);
  
 Wire.beginTransmission(0x68);
  Wire.write(0);
  Wire.write(decToBcd(ss));
  Wire.write(decToBcd(mm));
  Wire.write(decToBcd(hh));
  Wire.endTransmission();
  
}

void getTime(){
  
  Wire.beginTransmission(0x68);
  Wire.write(0);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 3);
  
  seconds = Wire.read();
  minutes = Wire.read();
  hours = Wire.read();
    
    //convert chronodot output into decimal values we understand
    
    seconds = (((seconds & 0b11110000)>>4)*10 + (seconds & 0b00001111));
    minutes = (((minutes & 0b11110000)>>4)*10 + (minutes & 0b00001111));
    hours = (((hours & 0b11110000)>>4)*10 + (hours & 0b00001111));
  
}
  
