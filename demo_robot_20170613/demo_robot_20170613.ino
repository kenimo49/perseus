#include <Servo.h> 

//====================================================================================================================
//Parameters
//====================================================================================================================
#define SERV_NUM 4                  //number of servo for array
#define FRAME 20                    //interval time from current step to next step: 20msecc

int current_angle[SERV_NUM];
int target_angle[SERV_NUM+1];
float rotating_angle[SERV_NUM];      //rotating angle on each frame: calcurated by (target_angle - current_angle)/number of steps
int servo_trim[SERV_NUM];            //trim to adjust each servo motors's angle to center

Servo servo[SERV_NUM];
#define CENTER 4
#define RIGHT 5
#define LEFT 6
#define NECK 7
#define RIGHTLIGHT 12
#define LEFTLIGHT 13
#define HUMANSENSOR A0
#define DELAY 50

//====================================================================================================================
//Motion
//====================================================================================================================
//motion1 look around
int look_around[5][SERV_NUM+1] ={{0,0,0,0,200},{0,0,0,60,300},{0,0,0,0,300},{0,0,0,-60,300},{0,0,0,0,300}};

//motion2 move forward
int move_forward[6][SERV_NUM+1] ={{35,0,0,0,200},{35,-15,-15,0,200},{0,-15,-15,0,600},{-35,-15,-15,0,200},{-35,15,15,0,400},{0,15,15,0,600}};

//motion3 move back
int move_back[6][SERV_NUM+1] ={{-35,0,0,0,200},{-35,-15,-15,0,200},{0,-15,-15,0,600},{35,-15,-15,0,200},{35,15,15,0,400},{0,15,15,0,600}};

//motion4 home position
int home_position[1][SERV_NUM+1] ={{0,0,0,0,300}};

//motion5 turn right
int turn_right[7][SERV_NUM+1] ={{35,0,0,0,200},{35,-15,0,0,200},{0,-15,0,0,600},{-35,-15,0,0,200},{-35,-15,-15,0,200},{-35,0,0,0,200},{0,0,0,0,600}};

//motion6 turn left
int turn_left[7][SERV_NUM+1] ={{-35,0,0,0,200},{-35,0,15,0,200},{0,0,15,0,600},{35,0,15,0,200},{35,15,15,0,200},{35,0,0,0,200},{0,0,0,0,600}};

//====================================================================================================================
//Servo Method
//====================================================================================================================
void initServo(){
    
  //attach pins to each servo
  servo[0].attach(CENTER);
  servo[1].attach(RIGHT);
  servo[2].attach(LEFT);
  servo[3].attach(NECK);
  
  //set trim
  int tmp_trim[SERV_NUM]={0,3,3,0};      //set trim to each servo
  for(int i=0;i<SERV_NUM;i++){
    servo_trim[i]=tmp_trim[i];
  }
  
  //rotate all servos to center position
  setCenterToServo();
}

void initLED(){
  pinMode(RIGHTLIGHT, OUTPUT);
  pinMode(LEFTLIGHT, OUTPUT);
}

void initHumanSensor(){
  pinMode(HUMANSENSOR,INPUT);
}
int LedStatusLeft = LOW;
int LedStatusRight = LOW;

void setCenterToServo(){
  for(int i=0;i<SERV_NUM;i++){
    servo[i].write(90 + servo_trim[i]);
    current_angle[i] = 0;
    target_angle[i] = 0;
  }
}

//move to next position
void moveToNextPosition(){
  
  //check limit
   for(int i=0;i<SERV_NUM;i++){
     if(target_angle[i]>90){
       target_angle[i]=90;
     }else if(target_angle[i]<-90){
       target_angle[i]=-90;
     }
   }
  
  int numberOfStep = target_angle[SERV_NUM]/FRAME;    //total number of steps to move to next position
  
  for(int i=0;i<SERV_NUM;i++){
    rotating_angle[i]=((float)target_angle[i]-(float)current_angle[i])/(float)numberOfStep;
  }
  
  int next_timing = millis() + FRAME;
  int current_time;
  float tmp_angle[SERV_NUM];
  
  for(int i=0;i<SERV_NUM;i++){
    tmp_angle[i]=(float)current_angle[i];
  }
  
  while(numberOfStep){
    current_time=millis();
    if(current_time>next_timing){
      for(int i=0;i<SERV_NUM;i++){
        tmp_angle[i] += rotating_angle[i];
            
        if(rotating_angle[i]<0){
          if(current_angle[i]>target_angle[i]){
            current_angle[i] = (int)tmp_angle[i];
          }
        }else if(rotating_angle[i]>0){
          if(current_angle[i]<target_angle[i]){
            current_angle[i] = (int)tmp_angle[i];
          }else{
            current_angle[i]=target_angle[i];
          }
        }
        servo[i].write(current_angle[i]+servo_trim[i]+90);        
      }
      next_timing = next_timing + FRAME;
      numberOfStep--;
    }
  }
  
  //adjust current_angle
  for(int i=0;i<SERV_NUM;i++){
    if(rotating_angle[i]<0 && current_angle[i]>target_angle[i]){
      current_angle[i] = target_angle[i];
      servo[i].write(current_angle[i]+servo_trim[i]+90);
    }else if(rotating_angle[i]>0 && current_angle[i]<target_angle[i]){
      current_angle[i] = target_angle[i];
      servo[i].write(current_angle[i]+servo_trim[i]+90);
    }else if(rotating_angle[i]<0 && current_angle[i]<target_angle[i]){
      current_angle[i] = target_angle[i];
      servo[i].write(current_angle[i]+servo_trim[i]+90);
    }else if(rotating_angle[i]>0 && current_angle[i]>target_angle[i]){
      current_angle[i] = target_angle[i];
      servo[i].write(current_angle[i]+servo_trim[i]+90);
    }
  }
}

//call moveToNextPosition() method continuously
void playMotion(int motion[][SERV_NUM+1], int numberOfMotion){

  for(int i=0;i<numberOfMotion;i++){
    for(int j=0;j<SERV_NUM+1;j++){
      target_angle[j]=motion[i][j];
    }
    moveToNextPosition();
  }

}


//====================================================================================================================
//Main
//====================================================================================================================

void setup(){
  initServo();
  Serial.begin(9600);
  initLED();
  initHumanSensor();
}
int val=0;
void loop(){
        if (digitalRead(HUMANSENSOR)){
          
          if(LedStatusLeft==LOW){
            digitalWrite(12, HIGH);
            LedStatusLeft=HIGH;
            Serial.println("ON");
          }
          if(LedStatusRight==LOW){
            LedStatusRight=HIGH;
            digitalWrite(13, HIGH);
            Serial.println("ON");
          }
        }else{
          
          if(LedStatusLeft==HIGH){
            LedStatusLeft=LOW;
            digitalWrite(12, LOW);
            Serial.println("OFF");
          }
          if(LedStatusRight==HIGH){
            LedStatusRight=LOW;
            digitalWrite(13, LOW);
            Serial.println("OFF");
          }
        }


  if(Serial.available()){
    char cmd = Serial.read();
    
    switch (cmd){  
      case 'w':
        playMotion(move_forward, 6);
        break; 
      case 's':
        playMotion(move_back, 6);
        break;
      case 'a':
        playMotion(turn_left, 7);
        break;
      case 'd':
        playMotion(turn_right, 7);
        break;      
      case 'j':
        playMotion(look_around, 5);
        break;
      case 'k':
        playMotion(home_position, 1);
        break;
      case 'l':
        digitalWrite(12, HIGH);
        LedStatusLeft=HIGH;
        break;
      case 'r':
        digitalWrite(13, HIGH);
        LedStatusRight=HIGH;
        break;
      case 't':
        digitalWrite(13, LOW);
        digitalWrite(12, LOW);
        LedStatusLeft=LOW;
        LedStatusRight=LOW;
        break;
    }
  }
  delay(100);
}



