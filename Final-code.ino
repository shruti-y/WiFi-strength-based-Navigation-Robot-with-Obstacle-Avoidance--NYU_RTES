
#include <Servo.h>
#include <ESP8266WiFi.h>
#define WIFI_SSID "OnePlus 6"
#define WIFI_PASSWORD "123456789"

/*
//Rohan Shah
//Shruti Yeravadekar
*/

//===============================================================// 
//#define trigPin 2    // Trigger
#define commPin 16    // Echo
long duration_l, duration_c, duration_r, inches_l, inches_c, inches_r, cm_l, cm_c, cm_r;
#define f 4
#define e 5
#define d 2
#define AIN1 0
#define AIN2 12 
#define BIN1 13
#define BIN2 15 

//================================================================//


int fread1,fread2,fread3,fread4,fread5,fread6,bread1,bread2,bread3,bread4,bread5,bread6,lread1,lread2,lread3,lread4,lread5,lread6,rread1,rread2,rread3,rread4,rread5,rread6;
int favg,bavg,lavg,ravg;
int rssinow;
//flags:
int flag1 = 0;
int flag_cal = 0;

int WIFIRSSI(){
  if (WiFi.status() != WL_CONNECTED)
    return -1;
  int dbm = WiFi.RSSI();
  //Serial.printf("RSSI:\t%d dBm\r\n", dbm);
  return dbm;
}

float average(int a, int b, int c, int k, int l, int m)
{
  float avg;
  return avg = (a+b+c+k+l+m)/6;
}
void pick_a_dir()
{
  //move forward from a starting point
  delay(3000);
  
  
  //goahead();
  Serial.println("Start going Forward from initial pose");
  //analogWrite(PWMA, speed_car);
  //analogWrite(PWMB, speed_car);
goAhead();
  fread1 = WIFIRSSI();
  delay(250);
  fread2 = WIFIRSSI();
  delay(250);
  fread3 = WIFIRSSI();
  delay(250);
  fread4 = WIFIRSSI();
  delay(250);
  fread5 = WIFIRSSI();
  delay(250);
  fread6 = WIFIRSSI();
  delay(250);
  favg= average(fread1,fread2,fread3,fread4,fread5,fread6);
  Serial.println(favg);
  
  //Come back to same point

  delay(1250);
stopcar();
  delay(1000);
  
  
  //goBack();
  //analogWrite(PWMA, speed_car);
  //analogWrite(PWMB, speed_car);

  turnRight();

  delay(250);
  
  //goahead()in Right dir
goAhead();
  rread1 = WIFIRSSI();
  delay(250);
  rread2 = WIFIRSSI();
  delay(250);
  rread3 = WIFIRSSI();
  delay(250);
  rread4 = WIFIRSSI();
  delay(250);
  rread5 = WIFIRSSI();
  delay(250);
  rread6 = WIFIRSSI();
  delay(250);
  ravg= average(rread1,rread2,rread3,rread4,rread5,rread6);
  Serial.println(ravg);
  //Come back to same point
goBack();
  delay(1250);
stopcar();
  delay(1000);
  
  //goBack() i.e. left_dir;
goAhead();
  lread1 = WIFIRSSI();
  delay(250);
  lread2 = WIFIRSSI();
  delay(250);
  lread3 = WIFIRSSI();
  delay(250);
  lread4 = WIFIRSSI();
  delay(250);
  lread5 = WIFIRSSI();
  delay(250);
  lread6 = WIFIRSSI();
  delay(250);
  lavg= average(lread1,lread2,lread3,lread4,lread5,lread6);
  Serial.println(lavg);
  //Come back to same point
goBack();
  delay(1250);
stopcar();
  delay(1000);
  
 
  int max_fb, max_lr, m;
  max_fb = favg > ravg ? favg : ravg;
  //max_lr = ravg > lavg ? ravg : lavg;
  m = max_fb < lavg ? max_fb :lavg;
  if(m == favg )
  {turnLeft();
  Serial.println("Front");
  delay(300);
  }
  
  else if(m == ravg )
  {
    Serial.println("right.");
    delay(250);
    stopcar();
    }
  else if(m == lavg )
  {
    turnRight();
    delay(350);
    stopcar();
    delay(250);
    turnRight();
    delay(350);
    stopcar();
    delay(250);
    Serial.println("left");
    
    }
  flag_cal = 1;
  Serial.println(m);
  Serial.println("I calibrated.");
}  

//------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------SETUP-----------------------------------------------------
//------------------------------------------------------------------------------------------------------------------


void setup() {
  //Serial Port begin
  Serial.begin(9600);
  //Define inputs and outputs
  //pinMode(trigPin, OUTPUT);
  pinMode(commPin, INPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  Serial.println("Setup done");
  pinMode(f, OUTPUT); //C
  pinMode(e, OUTPUT); //B
  //digitalWrite(b, HIGH);
  pinMode(d, OUTPUT); //A
 WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  
  }

//------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------LOOP------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------

void loop() {
//Snippet to pick a direction from 4 directions.
//flag1 for the starting position.
rssinow = WIFIRSSI();


 
if(rssinow>50)
{
 stopcar();
 delay(4000);
}

else if (rssinow<50)
{
 
  pick_a_dir();
  updateDistance("r");
  updateDistance("l");
  updateDistance("c");
  //Center
  if(cm_c <=60){
    stopcar();
    updateDistance("r");
    updateDistance("l");
    updateDistance("c");
    if(cm_c <= 15){
      goBack();
      delay(750);
      stopcar();
    }

    else if(cm_l < 50 && cm_r >= 50){
    turn_Right_35();
    Serial.println("Turning right...");
    delay(250);
    }
    else if(cm_r <50 && cm_l >=50){
      turn_Left_35();
      Serial.println("Turning left...");
      delay(250);
    }
   // updateDistance("c");
    else if(cm_l >= cm_r){
      turn_Left_35();
      Serial.println("Turning left...distance greater than right");
      delay(250);
    }
    else if(cm_r > cm_l){
      turn_Right_35();
      Serial.println("Turning right...distance greater than left");
      delay(250);
    }
    
  }
  //Left
  else if(cm_l <=40){
    updateDistance("r");
    updateDistance("l");
    updateDistance("c");
    if(cm_l <= 15){
      goBack();
      delay(350);
      stopcar();
    }

    else if(cm_c < 60 && cm_r >= 50){
    turn_Right_35();
    Serial.println("Turning right...");
    delay(250);
    }
    else if(cm_r >= cm_l){
      turn_Right_35();
      Serial.println("Turning right...distance greater than left");
      delay(250);
    }
  }

    //Right
    else if(cm_r <=40){
    updateDistance("r");
    updateDistance("l");
    updateDistance("c");
    if(cm_r <= 15){
      goBack();
      delay(350);
      stopcar();
    }
  
    else if(cm_c < 60 && cm_l >= 50){
    turn_Left_35();
    Serial.println("Turning left...");
    delay(250);
    }
    else if(cm_l >= cm_r){
      turn_Left_35();
      Serial.println("Turning left...distance greater than right");
      delay(250);
    }
    else
    {
      goAhead();
      delay(500);
    }
}
 

  
  }


}
//================================================================//
//Function defs

void goAhead(){
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  Serial.println("Go Ahead");
}
void goBack(){
  digitalWrite(AIN2, LOW);
  digitalWrite(AIN1, HIGH);
  digitalWrite(BIN2, LOW);
  digitalWrite(BIN1, HIGH);
  Serial.println("Go Back");
}
void turnLeft(){
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  Serial.println("Turning left");
  //delay(350);
  }
void turnRight(){
  digitalWrite(AIN2, HIGH);
  digitalWrite(AIN1, LOW);
  digitalWrite(BIN2, LOW);
  digitalWrite(BIN1, LOW);
  Serial.println("Turning Right");
  //delay(350);
  }
void stopcar(){
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  Serial.println("Stopped");
  
  }

void turn_Left_30() {
  
int temp_delay = 45*550/90;
 digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  delay(temp_delay);
  stopcar();
  delay(50);

}

void turn_Right_30() {

int temp_delay = 45*550/90;
 digitalWrite(AIN1,LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN2, LOW);
  digitalWrite(BIN1, LOW);
  delay(temp_delay);
  stopcar();
  delay(50);

}
void turn_Right_35() {

  int temp_delay = 310;
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  delay(temp_delay);
  stopcar();
  delay(50);

}

void turn_Left_35() {

  int temp_delay = 310;
  digitalWrite(AIN1,LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  delay(temp_delay);
  stopcar();
  delay(50);

}

//Ultrasonics with MUX
void updateDistance(String direction){
  if(direction == "l"){
    Serial.println("Sonar left...");
    
    digitalWrite(f, LOW);
    digitalWrite(e, LOW);
    digitalWrite(d, LOW);
    //trigger
    pinMode(commPin, OUTPUT);
    digitalWrite(commPin, LOW);
    delayMicroseconds(5);
    digitalWrite(commPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(commPin, LOW);
    //echo
    digitalWrite(f, HIGH);
    digitalWrite(e, HIGH);
    digitalWrite(d, LOW);
    
    pinMode(commPin, INPUT);
    duration_l = pulseIn(commPin, HIGH);

    cm_l = (duration_l/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
    inches_l = (duration_l/2) / 74;   // Divide by 74 or multiply by 0.0135
    if(cm_l>=150){cm_l=150;}
    Serial.println(cm_l);
    delay(50);
  }
  else if(direction == "c"){
    Serial.println("Sonar center...");
    pinMode(commPin, OUTPUT);
    digitalWrite(f, HIGH);
    digitalWrite(e, LOW);
    digitalWrite(d, LOW);

   //trigger
    digitalWrite(commPin, LOW);
    delayMicroseconds(5);
    digitalWrite(commPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(commPin, LOW);

    //echo
    pinMode(commPin, INPUT);
    digitalWrite(f, LOW);
    digitalWrite(e, LOW);
    digitalWrite(d, HIGH);

    
    duration_c = pulseIn(commPin, HIGH);

    cm_c = (duration_c/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
    inches_c = (duration_c/2) / 74;   // Divide by 74 or multiply by 0.0135 
    if(cm_c>=150){cm_c=150;}
    Serial.println(cm_c);
    delay(50);
  }
  else if(direction == "r"){
    Serial.println("Sonar right...");
    digitalWrite(f, LOW);
    digitalWrite(e, HIGH);
    digitalWrite(d, LOW);
    //trigger
    pinMode(commPin, OUTPUT);
    digitalWrite(commPin, LOW);
    delayMicroseconds(5);
    digitalWrite(commPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(commPin, LOW);
    //echo
    digitalWrite(f, HIGH);
    digitalWrite(e, HIGH);
    digitalWrite(d, HIGH);
    
    pinMode(commPin, INPUT);
    duration_r = pulseIn(commPin, HIGH);

    cm_r = (duration_r/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
    inches_r = (duration_r/2) / 74;   // Divide by 74 or multiply by 0.0135
    if(cm_r>=150){cm_r=150;}
    Serial.println(cm_r);
    delay(50);
  }
}
